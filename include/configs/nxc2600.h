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
 * 2007/07/16: initial
 *
 *
 * Author: Hardy Weng <hardy.weng@icnexus.com.tw>
 *
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#define CONFIG_MIPS32		1	/* MIPS 4K CPU core	*/
#define CONFIG_ICNEXUS_NXC2600	1

#define	CONFIG_MISC_INIT_R	1
#define CONFIG_CONS_INDEX	1
#define CONFIG_BOOTDELAY	1	/* autoboot after 5 seconds	*/

#define CONFIG_BAUDRATE		57600 

#define CFG_BAUDRATE_TABLE	{ 9600, 19200, 38400, 57600, 115200 }

#define CONFIG_TIMESTAMP		/* Print image info with timestamp */

#undef	CONFIG_BOOTARGS

#define CONFIG_PREBOOT          ""
#define CONFIG_BOOTCOMMAND      "run defaultboot"

#define CONFIG_NET_MULTI
#define CONFIG_MII              1       /* MII PHY management           */
#define CFG_FAULT_ECHO_LINK_DOWN 1
#define CONFIG_PHY_ADDR         0    /* PHY address                  */
#define CFG_RX_ETH_BUFFER       16      /* Number of ethernet rx buffers & descriptors */
#define CONFIG_PHY_RESET 1

#define CONFIG_ETHADDR         10:00:00:00:00:01
#define CONFIG_IPADDR           192.168.200.232
#define CONFIG_SERVERIP         192.168.200.213
#define CONFIG_NETMASK          255.255.255.0

#define CONFIG_DRIVER_NXC2600_I2C

#define CONFIG_COMMANDS		( CFG_CMD_PING | \
                                  CFG_CMD_DHCP | \
                                  CFG_CMD_NET  | \
                                  CFG_CMD_RUN | \
				  CFG_CMD_MII | \
				  CFG_CMD_ENV | \
				  CFG_CMD_FLASH | \
				  CFG_CMD_I2C | \
                                  CFG_CMD_MEMORY | \
				  CFG_CMD_JFFS2 | \
				  CFG_CMD_LOADB | \
				  CFG_CMD_NAND )

#include <cmd_confdefs.h>

/*
 * Miscellaneous configurable options
 */
#undef CFG_LONGHELP
#define CFG_PROMPT		"NXC2600# "		/* Monitor Command Prompt    */
#define CFG_CBSIZE		256		/* Console I/O Buffer Size   */
#define CFG_PBSIZE (CFG_CBSIZE+sizeof(CFG_PROMPT)+16)  /* Print Buffer Size */
#define CFG_MAXARGS		16		/* max number of command args*/

#define CFG_MALLOC_LEN		128*1024

#define CFG_BOOTPARAMS_LEN	128*1024

#define CFG_HZ			1000

#define CFG_SDRAM_BASE		0x80000000
#if 0
#ifndef __ASSEMBLY__
extern long int runtime_sdram_size;
#endif
#define CFG_SDRAM_SIZE		(runtime_sdram_size)
#endif

#define CFG_LOAD_ADDR		0x80500000	/* default load address */

#define CFG_MEMTEST_START	0x80000000
#define CFG_MEMTEST_END		(CFG_MEMTEST_START+gd->ram_size)

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
#if 0
#define CFG_FLASH_CFI           1
#define CFG_FLASH_CFI_DRIVER    1
#define CFG_FLASH_CFI_NO_PROBE	1
#define CFG_FLASH_CFI_DEFAULT_VENDOR	CFI_CMDSET_AMD_STANDARD
#define CFG_FLASH_CFI_DEFAULT_OFFSET	FLASH_OFFSET_CFI_ALT
#endif
#define CFG_MAX_FLASH_BANKS	1	/* max number of memory banks */
#define CFG_MAX_FLASH_SECT	(128)	/* max number of sectors on one chip */

#define PHYS_FLASH_1		0xbc000000 /* Flash Bank #1 */
#define CFG_FLASH_ADDR0         0x5555
#define CFG_FLASH_ADDR1         0x2aaa
#define CFG_FLASH_CFI_WIDTH	FLASH_CFI_16BIT

/* The following #defines are needed to get flash environment right */
#define CFG_MONITOR_BASE	TEXT_BASE
#define CFG_MONITOR_LEN		(192 << 10)

#define FLASH_BASE0_PRELIM      CFG_FLASH_BASE  /* FLASH bank #0        */

/* timeout values are in ticks */
#define CFG_FLASH_ERASE_TOUT	(20 * CFG_HZ) /* Timeout for Flash Erase */
#define CFG_FLASH_WRITE_TOUT	(2 * CFG_HZ) /* Timeout for Flash Write */


#define CONFIG_NR_DRAM_BANKS	2

/*-----------------------------------------------------------------------
 * Cache Configuration
 */
#define CFG_DCACHE_SIZE		16384
#define CFG_ICACHE_SIZE		16384
#define CFG_CACHELINE_SIZE	32


/*
 * NAND
 */
#define CFG_NAND0_BASE          0x14000000
#undef CFG_NAND1_BASE

#define CFG_NAND_BASE_LIST      { CFG_NAND0_BASE }
#define CFG_MAX_NAND_DEVICE     1       /* Max number of NAND devices */

/* nand timeout values */
#define CFG_NAND_PROG_ERASE_TO  3000
#define CFG_NAND_OTHER_TO       100
#define CFG_NAND_SENDCMD_RETRY  3
#undef NAND_ALLOW_ERASE_ALL     /* Allow erasing bad blocks - don't use */

/* NAND Timing Parameters (in ns) */
#define NAND_TIMING_tCH         10
#define NAND_TIMING_tCS         0
#define NAND_TIMING_tWH         20
#define NAND_TIMING_tWP         40

#define NAND_TIMING_tRH         20
#define NAND_TIMING_tRP         40
#define NAND_TIMING_tR          11123
#define NAND_TIMING_tWHR        100
#define NAND_TIMING_tAR         10

#define	CONFIG_MTD_NAND_VERIFY_WRITE	1

#define ADDR_COLUMN             1
#define ADDR_PAGE               2
#define ADDR_COLUMN_PAGE        3

#define NAND_ChipID_UNKNOWN     0x00
#define NAND_MAX_FLOORS         1
#define NAND_MAX_CHIPS          1

#define CFG_NAND_PAGE_SIZE      (2048)
#ifndef __ASSEMBLY__
#include <linux/mtd/mtd.h>
typedef struct mtd_info nand_info_t;
extern nand_info_t nand_info[];
#define CFG_NAND_BLOCK_SIZE     (nand_info[0].erasesize)
#endif
#define CFG_NAND_PAGE_COUNT     (32)
#define CFG_NAND_BAD_BLOCK_POS  (0)
#undef CFG_NAND_4_ADDR_CYCLE

#define CFG_NAND_U_BOOT_SIZE    (256*1024)
#define	CFG_NAND_U_BOOT_DST	(0x83f00000)
#define	CFG_NAND_U_BOOT_OFFS	(0x1000) /* nand boot preload code */

#define CONFIG_JFFS2_CMDLINE 
#define CONFIG_JFFS2_NAND 1


/* Address and size of Primary Environment Sector       */
#define CONFIG_ENV_OVERWRITE    1
#define CFG_ENV_ADDR            0xbc040000
#define CFG_ENV_SIZE		0x40000
#define CFG_INIT_SP_OFFSET      0x4000

#define CFG_FLASH_BASE          PHYS_FLASH_1
#define CFG_FLASH_SIZE          0x800000

#define	CONFIG_KERNEL_SIZE	0x200000

#ifndef CONFIG_NAND_U_BOOT

#define CFG_ENV_IS_IN_FLASH     1

#if CFG_FLASH_SIZE > 0x400000
#define	CFG_U_BOOT_OFFS		(CFG_FLASH_SIZE-0x400000)

#if CFG_FLASH_SIZE == 0x800000
#define	CFG_U_BOOT_ADDR		"bc400000"
#define CFG_U_BOOT_END		"bc43ffff"
#undef CFG_ENV_ADDR
#define	CFG_ENV_ADDR		0xbc440000
#define	CFG_KERNEL_ADDR		"bc480000"
#define CFG_KERNEL_END		"bc67ffff"
#define	CFG_ROOTFS_ADDR		"bc680000"
#define	CFG_ROOTFS_END		"bca7ffff"

#elif CFG_FLASH_SIZE == 0x1000000
#define CFG_U_BOOT_ADDR         "bcc00000"
#define CFG_U_BOOT_END          "bcc3ffff"
#undef CFG_ENV_ADDR
#define CFG_ENV_ADDR            0xbcc40000
#define CFG_KERNEL_ADDR         "bcc80000"
#define CFG_KERNEL_END          "bce7ffff"
#define CFG_ROOTFS_ADDR         "bc000000"
#define CFG_ROOTFS_END          "bc9fffff"


#else
#error no flash map defined.
#endif

#else
#define CFG_U_BOOT_OFFS		(0)
#define	CFG_U_BOOT_ADDR		"bc000000"
#define	CFG_U_BOOT_END		"bc03ffff"
#undef CFG_ENV_ADDR
#define	CFG_ENV_ADDR		0xbc040000
#define CFG_KERNEL_ADDR         "bc080000"
#define CFG_KERNEL_END          "bc27ffff"
#define CFG_ROOTFS_ADDR         "bc280000"
#define CFG_ROOTFS_END          "bc67ffff"
#endif

#define CONFIG_EXTRA_ENV_SETTINGS                                       \
	"console=ttyS0,57600\0"				\
	"nfsroot=/nfsroot/nxc2600\0"			\
	"defaultargs=run nandargs\0"	\
        "nfsargs=setenv bootargs mem=${memsize} console=${console} root=/dev/nfs rw ip=${ipaddr} nfsroot=${serverip}:${nfsroot}\0"               \
	"nandargs=setenv bootargs mem=${memsize} console=${console} root=/dev/ram rd_start=${rootfs_load} rd_size=${rootfs_size} mtdparts=nxc2600-nand:256k@${uboot_nand_addr}(uboot)ro,256k(params),${nandparts}\0" \
	"nandparts=2048k(kernel),8m(initrd),117248k(rootfs),-(data)\0"    \
	"nfsboot=run nfsargs;bootm ${kernel_addr}\0"		\
	"defaultboot=run nandboot; bootm ${kernel_addr}\0"		\
	"nandboot=run nandargs;run nloadr;run nloadk; bootm ${kernel_load}\0" \
        "autoload=no\0"                                                 \
        "kernel_addr=" CFG_KERNEL_ADDR "\0"                             \
	"kernel_size=0x200000\0"					\
	"kernel_end=" CFG_KERNEL_END "\0"				\
	"kernel_load=0x80500000\0"					\
	"rootfs_addr=" CFG_ROOTFS_ADDR "\0"				\
	"rootfs_size=0x800000\0"				\
	"rootfs_end=" CFG_ROOTFS_END "\0"				\
	"rootfs_load=0x80800000\0"					\
	"nloadr=cp.b ${rootfs_addr} ${rootfs_load} ${rootfs_size}\0" \
        "nloadk=cp.b ${kernel_addr} ${kernel_load} ${kernel_size}\0" \
        "load_b=tftp 80500000 u-boot.bin\0"                           \
        "load_k=tftp ${kernel_load} uImage.kernel\0"                  \
	"load_r=tftp ${rootfs_load} rootfs.img\0"			\
        "update_b=run load_b;"                                        \
                "protect off " CFG_U_BOOT_ADDR " " CFG_U_BOOT_END ";"    \
                "erase " CFG_U_BOOT_ADDR " " CFG_U_BOOT_END ";" \
                "cp.b 80100000 " CFG_U_BOOT_ADDR " 40000\0"            \
        "update_k=run load_k;"                                \
		"protect off ${kernel_addr} ${kernel_end};"	\
                "erase ${kernel_addr} ${kernel_end};"        \
                "cp.b ${kernel_load} ${kernel_addr} ${kernel_size}\0"   \
	"update_r=run load_r;"                                  \
		"protect off ${rootfs_addr} ${rootfs_end};" \
		"erase ${rootfs_addr} ${rootfs_end};"	\
		"cp.b ${rootfs_load} ${rootfs_addr} ${rootfs_size}\0" \
        ""


#else
#define CFG_ENV_IS_IN_NAND	1
#ifndef __ASSEMBLY__
extern int real_env_offset(void);
#endif
#define CFG_ENV_OFFSET          (real_env_offset())

#define CONFIG_EXTRA_ENV_SETTINGS                                       \
        "console=ttyS0,57600\0"                                \
	"nfsroot=/nfsroot/nxc2600\0"				\
        "defaultargs=run nandargs\0"   \
        "nfsargs=setenv bootargs mem=${memsize} console=${console} root=/dev/nfs rw ip=${ipaddr} nfsroot=${serverip}:${nfsroot}\0"               \
	"nandargs=setenv bootargs mem=${memsize} console=${console} root=/dev/ram rd_start=${rootfs_load} rd_size=${rootfs_size} mtdparts=nxc2600-nand:256k@${uboot_nand_addr}(uboot)ro,256k(params),${nandparts}\0" \
	"nandparts=2048k(kernel),8m(initrd),117248k(rootfs),-(data)\0"	\
        "nfsboot=run nfsargs;run nloadk;bootm ${kernel_load}\0" \
        "defaultboot=run nandboot\0"  \
	"nandboot=run nandargs;run nloadr;run nloadk;bootm ${kernel_load}\0" \
        "autoload=no\0"                                                 \
        "kernel_size=0x200000\0"                                          \
        "kernel_load=0x80500000\0"                                        \
	"rootfs_size=0x800000\0"				\
	"rootfs_load=0x80800000\0"					\
	"nloadr=nand read.jffs2 ${rootfs_load} ${rootfs_addr} ${rootfs_size}\0" \
	"nloadk=nand read.jffs2 ${kernel_load} ${kernel_addr} ${kernel_size}\0" \
        "load_b=tftp 80100000 u-boot-nand.bin\0"                           \
        "load_k=tftp ${kernel_load} uImage.kernel\0"                  \
	"load_r=tftp ${rootfs_load} rootfs.img\0"		\
	"load_i=tftp ${rootfs_load} rootfs_init.img\0"		\
        "update_b=run load_b;"                                        \
                "nand erase ${uboot_nand_addr} 40000;"                              \
                "nand write 80100000 ${uboot_nand_addr} 40000\0"                  \
        "update_k=run load_k;"                                \
                "nand erase ${kernel_addr} ${kernel_size};" \
                "nand write.jffs2 ${kernel_load} ${kernel_addr} ${kernel_size}\0" \
	"update_r=run load_r;"					\
		"nand erase ${rootfs_addr} ${rootfs_size};" \
		"nand write.jffs2 ${rootfs_load} ${rootfs_addr} ${rootfs_size}\0" \
	"update_i=run load_i;"					\
		"nand erase ${rootfs_addr} ${rootfs_size};" \
		"nand write.jffs2 ${rootfs_load} ${rootfs_addr} ${rootfs_size}\0" \	
	"update_all=run update_b;run update_k;run update_r\0"	\
	"update_all_c=nand scrub ${uboot_nand_addr} ${nand_size};run update_all\0" \
	"update_all_init=run update_b;run update_k;run update_i\0"	\
	"update_all_i=nand scrub ${uboot_nand_addr} ${nand_size};run update_all_init\0" \
	""

#endif
//#define CFG_DIRECT_FLASH_TFTP

#define CFG_NAND_BASE           CFG_NAND0_BASE
#define CFG_NAND_U_BOOT_START   (CFG_NAND_U_BOOT_DST+CFG_NAND_U_BOOT_OFFS)



//#define DEBUG 3
//#define CFG_DFC_DEBUG1 /* usefull */
//#undef CFG_DFC_DEBUG2  /* noisy */
//#undef CFG_DFC_DEBUG3  /* extremly noisy  */

//#define CONFIG_MTD_DEBUG 3
//#define CONFIG_MTD_DEBUG_VERBOSE 3


#endif	/* __CONFIG_H */
