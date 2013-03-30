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

#include <common.h>

#if (CONFIG_COMMANDS & CFG_CMD_NAND)

#include <nand.h>
#include <asm/io.h>
#include <linux/autoconf.h>
#include <asm/nxc2600.h>

#ifndef CONFIG_NAND_SPL
#ifdef CONFIG_NAND_U_BOOT
int real_env_offset(void)
{
	unsigned int uboot_addr = *((unsigned int*)CFG_NAND_U_BOOT_DST);
	return CFG_NAND_U_BOOT_SIZE + uboot_addr;
}
#endif
void
nxc2600_reset_boot_bank(void)
{
	if(nxc2600_get_boot_bank()==0);
		nxc2600_nand_disable();
}

static void 
nand_hwcontrol( struct mtd_info *mtd, 
		int cmd)
{
	struct nand_chip *this = mtd->priv;

	switch (cmd) 
	{
		case NAND_CTL_SETNCE:
			nxc2600_nand_set_nce(1);
			break;
	        case NAND_CTL_CLRNCE:
			nxc2600_nand_set_nce(0);
                        break;

                case NAND_CTL_SETCLE:
                        this->IO_ADDR_W = nxc2600_nand_set_cle(1);
                        break;

                case NAND_CTL_CLRCLE:
                        this->IO_ADDR_W = nxc2600_nand_set_cle(0);
                        break;

                case NAND_CTL_SETALE:
                        this->IO_ADDR_W = nxc2600_nand_set_ale(1);
                        break;

                case NAND_CTL_CLRALE:
                        this->IO_ADDR_W = nxc2600_nand_set_ale(0);
                        break;
        }
}

static int
nand_ready( struct mtd_info *mtd )
{
        return (nxc2600_nand_is_ready()? 1:0);
}

static void
nxc2600_nand_ecc_hwctl(  struct mtd_info *mtd,
                          int mode )
{
        struct nand_chip *this = (struct nand_chip *)mtd->priv;

        switch(mode)
        {
                case NAND_ECC_WRITE:
                case NAND_ECC_READ:
                        nxc2600_nand_ecc_enable();
                        break;
        }
}

static int
nxc2600_nand_ecc_calculate( struct mtd_info *mtd,
                            const uint8_t *dat,
                            uint8_t *ecc_code)
{
        nxc2600_nand_ecc_disable();
        *ecc_code = nxc2600_nand_get_ecc();
        return 0;
}

static int
nxc2600_nand_ecc_correct( struct mtd_info *mtd,
                         uint8_t *dat,
                         uint8_t *read_ecc,
                         uint8_t *calc_ecc)
{
	if(*read_ecc == 0xff) return 0;
        return ((*read_ecc ^ *calc_ecc)? -1:0);
}

int 
board_nand_init(struct nand_chip *this)
{
	nxc2600_nand_init();

        this->IO_ADDR_R = (void __iomem *) NXC2600_NAND_DATA_PORT;
        this->IO_ADDR_W = (void __iomem *) NXC2600_NAND_DATA_PORT;
        this->hwcontrol = nand_hwcontrol;
        this->dev_ready = nand_ready;
        this->chip_delay = 100;
        this->eccsize = NXC2600_NAND_ECC_BLOCK;
        this->eccmode = NAND_ECC_HW3_256;
        this->enable_hwecc = nxc2600_nand_ecc_hwctl;
        this->calculate_ecc = nxc2600_nand_ecc_calculate;
        this->correct_data = nxc2600_nand_ecc_correct;
        this->eccbytes = 3;

	this->options = (nxc2600_nand_get_bus_width()==16)? NAND_BUSWIDTH_16:0;

	return 0;
}

#else

#define TMPBUF_BASE             (CFG_SDRAM_BASE+0x100000)

#define ECC_BLOCK		256 /* 3-bytes HW ECC per 256-bytes data */
#define ECC_POS			4   /* ECC offset to spare area */

static void 
nand_read_buf16(void *buf, int count)
{
        int i;
        u16 *p = (u16 *)buf;

        for (i = 0; i < count; i += 2)
                *p++ = REG16(NXC2600_NAND_DATA_PORT);
}

static void 
nand_read_buf8(void *buf, int count)
{
        int i;
        u8 *p = (u8 *)buf;

        for (i = 0; i < count; i++)
                *p++ = REG8(NXC2600_NAND_DATA_PORT);
}

void nand_boot(void)
{
	register u32 i, j, k;
	register u32 cnt, num_blk, blkaddr, rowaddr, addr;
	register u32 boot_sel, tmp, ecc_bit, idx;
	register u32 pagesize, rowaddrcycle, ecccount, oobsize, ppb;
	void (*uboot)(void) = (void (*)(void))CFG_NAND_U_BOOT_START;
	void (*nand_read_buf)(void *buf, int count);
	u32 *load_addr = 0, *param_addr = 0, *databuf32 = 0;
	u32 *stored_ecc = 0, *calc_ecc = 0;
	u8 *oobbuf = 0, *databuf = 0, *tmpbuf = 0;

	serial_init();
	timer_init();
	serial_puts("\n\n");
	initdram(0);

	/*
	 * Decode the boot select
	 */
	pagesize = nxc2600_nand_get_page_size();
	rowaddrcycle = nxc2600_nand_get_rowaddr_cycle();

	ppb = (pagesize == 2048) ? 64 : 32;         /* pages per block */

	ecccount = pagesize / ECC_BLOCK;
	oobsize = pagesize / 32;

	/* Init buffers */
	load_addr = (u32 *)CFG_NAND_U_BOOT_DST;
	databuf32 = (u32 *)TMPBUF_BASE;
	databuf = (u8 *)databuf32;
	oobbuf = (u8 *)(TMPBUF_BASE + 0x1000);
	calc_ecc = (u32 *)(TMPBUF_BASE + 0x2000);

	/* Calc block number */
	num_blk = CFG_NAND_U_BOOT_SIZE / (pagesize * ppb);
	if ((num_blk * pagesize * ppb) < CFG_NAND_U_BOOT_SIZE)
		num_blk++;

	nand_read_buf = (nxc2600_nand_get_bus_width()==16)? nand_read_buf16:nand_read_buf8;


	cnt = 0;
	blkaddr = 0;

	while (cnt < num_blk) {
nand_load_block:
		/* read one block */
		rowaddr = blkaddr * ppb;
		for (i = 0; i < ppb; i++) {
			/* send READ0 command */
			nxc2600_nand_cmd(NAND_CMD_READ0);

			/* col addr cycle */
			nxc2600_nand_addr(0);
			if (pagesize == 2048)
				nxc2600_nand_addr(0);

			/* row addr cycle */
			addr = rowaddr + i;
			nxc2600_nand_addr(addr & 0xff);
			nxc2600_nand_addr((addr >> 8) & 0xff);
			if (rowaddrcycle == 3)
				nxc2600_nand_addr((addr >> 16) & 0xff);

			/* send READSTART command for 2048 ps NAND */
			if (pagesize == 2048)
				nxc2600_nand_cmd(NAND_CMD_READSTART);

			/* wait ready */
			while(!nxc2600_nand_is_ready());

			/* read page and oob data */
			tmpbuf = databuf;
			for (j = 0; j < ecccount; j++) {
				nxc2600_nand_ecc_enable();
				nand_read_buf((void *)tmpbuf, ECC_BLOCK);
				nxc2600_nand_ecc_disable();
				calc_ecc[j] = nxc2600_nand_get_ecc();
				tmpbuf += ECC_BLOCK;
			}
			nand_read_buf((void *)oobbuf, oobsize);

			/* check for bad block */
			if (i == 0) {
				if (oobbuf[0] != 0xff) {
					serial_puts("Bad block, shift to next block\n");
					blkaddr ++;
					goto nand_load_block;
				}
			}

			/* check for ECC */
			stored_ecc = (u32 *)(((u32)oobbuf) + ECC_POS);
			tmpbuf = (u8 *)databuf;
#if 0
			for (j = 0; j < ecccount; j++) {
				tmp = stored_ecc[j] ^ calc_ecc[j];
				if (tmp) { /* ECC error occurred */
					if (stored_ecc[j] == 0xffffffff) {
						serial_puts("Block containing invalid data, try again.\n");
						goto nand_load_block;
					}

					ecc_bit = 0;
					for (k = 0; k < 24; k++)
						if ((tmp >> k) & 0x01)
							ecc_bit ++;
					if (ecc_bit != 11) {
						/* Fatal error */
						serial_puts("ECC error.\n");
						blkaddr ++;
						break; //goto nand_load_block;
					} else {
						/* Correctable error */
						ecc_bit = 0;
						for (k = 12; k >= 1; k--) {
							ecc_bit <<= 1;
							ecc_bit |= ((tmp>>(k*2-1)) & 0x01);
						}
						idx = ecc_bit & 0x07;
						tmpbuf[j * ECC_BLOCK + (ecc_bit >> 3)] ^= (1 << idx);
					}
				}
			}
#endif
			/* Data is OK, transfer to target buffer */
			for (j = 0; j < ((pagesize == 2048) ? 512:128); j++)
				*load_addr++ = databuf32[j];
		}

		cnt ++;
		blkaddr ++;

	}

 nand_load_finish:
	*((uint32_t*)CFG_NAND_U_BOOT_DST) = 0;
	flush_cache_all();
	(*uboot)();

}
#endif

#endif
