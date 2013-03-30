/*
 *
 * Copyright (C) 2007 IC Nexus Co., LTD.
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 */

/*
 *   NXC2600 Registers and Operations
 *
 * 2007/07/16: initial
 *
 */

#include <config.h>
#include <common.h>
#include <command.h>
#include <asm/addrspace.h>
#include <linux/autoconf.h>
#include <asm/nxc2600.h>

DECLARE_GLOBAL_DATA_PTR;
long int initdram(int board_type)
{
	long int ram_size = 0;
#ifdef CONFIG_NAND_SPL
	timer_init();
#elif defined(CONFIG_NAND_U_BOOT)
        return get_ram_size(CFG_SDRAM_BASE|KSEG1, 0x8000000);
#endif
	if( (!nxc2600_sdram_init(2,10,13,0x8000000)) &&
            ((ram_size=get_ram_size(CFG_SDRAM_BASE|KSEG1
                        ,0x10000000))==0x10000000) )
                ;
        else
        if( (!nxc2600_sdram_init(1,10,13,0x8000000)) &&
            ((ram_size=get_ram_size(CFG_SDRAM_BASE|KSEG1
                        ,0x8000000))==0x8000000) )
                ;
        else
	if( (!nxc2600_sdram_init(2,9,13,0x8000000/2)) &&
                 ((ram_size=get_ram_size(CFG_SDRAM_BASE|KSEG1
                        , 0x8000000))==0x8000000) )
                ;
        else
        if( (!nxc2600_sdram_init(1,9,13,0x4000000)) &&
            ((ram_size=get_ram_size(CFG_SDRAM_BASE|KSEG1
                        , 0x4000000))==0x4000000) )
                ;
        else
        if( (!nxc2600_sdram_init(1,8,13,0x2000000)) &&
            ((ram_size=get_ram_size(CFG_SDRAM_BASE|KSEG1
                        , 0x2000000))==0x2000000) )
		;
	else serial_puts("init sdram failed\n");

	return ram_size;
}

#ifndef CONFIG_NAND_SPL
extern nand_info_t nand_info[];
int misc_init_r (void)
{
	char buf[20];
        sprintf(buf,"%uM",gd->ram_size/0x100000);
        setenv("memsize",buf);
	sprintf(buf,"%x",nand_info[0].size);
	setenv("nand_size",buf);
	
#ifdef CONFIG_NAND_U_BOOT
	{
        unsigned int uboot_nand_addr = *((unsigned int*)CFG_NAND_U_BOOT_DST);
        sprintf(buf,"%x",uboot_nand_addr);
        setenv("uboot_nand_addr",buf);
	if(getenv("kernel_addr")==NULL)
	{
        	sprintf(buf,"%x",uboot_nand_addr+CFG_NAND_U_BOOT_SIZE+CFG_ENV_SIZE);
        	setenv("kernel_addr",buf);
	}
	if(getenv("rootfs_addr")==NULL)
	{
        	sprintf(buf,"%x",uboot_nand_addr+CFG_NAND_U_BOOT_SIZE+CFG_ENV_SIZE+CONFIG_KERNEL_SIZE);
        	setenv("rootfs_addr",buf);
	}
	}
#else
	if(getenv("uboot_nand_addr")==NULL)
		setenv("uboot_nand_addr","0");
#endif

	return 0;
}


void gpio_init(void)
{
	REG32(NXC2600_GPIO_SL(0)) = NXC2600_GPIO_SL0_DEFAULT;
	REG32(NXC2600_GPIO_SH(0)) = NXC2600_GPIO_SH0_DEFAULT;
	REG32(NXC2600_GPIO_SL(1)) = NXC2600_GPIO_SL1_DEFAULT;
        REG32(NXC2600_GPIO_SH(1)) = NXC2600_GPIO_SH1_DEFAULT;
	REG32(NXC2600_GPIO_SL(2)) = NXC2600_GPIO_SL2_DEFAULT;
        REG32(NXC2600_GPIO_SH(2)) = NXC2600_GPIO_SH2_DEFAULT;
	REG32(NXC2600_GPIO_SL(3)) = NXC2600_GPIO_SL3_DEFAULT;
        REG32(NXC2600_GPIO_SH(3)) = NXC2600_GPIO_SH3_DEFAULT;
}

void pll_init(void)
{
	REG32(NXC2600_CGM_CCR0) = NXC2600_CGM_CCR0_DEFAULT;
	REG32(NXC2600_CGM_PCR) = NXC2600_CGM_PCR_DEFAULT;
	/* disable oh-chip rtc */
	REG32(NXC2600_RTC_CR) = 0;
}

int checkboard (void)
{
	printf("Board: IC Nexus NXC2600 DVB");
	printf("(CPU Speed %d MHz)\n", (int)CONFIG_NXC2600_CPU_CLOCK/1000000);

	return 0;
}
#endif

