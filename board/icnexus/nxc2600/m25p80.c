/*
 * MTD SPI driver for ST M25Pxx flash chips
 *
 * Author: Mike Lavender, mike@steroidmicros.com
 *
 * Copyright (c) 2005, Intec Automation Inc.
 *
 * Some parts are based on lart.c by Abraham Van Der Merwe
 *
 * Cleaned up and generalized based on mtd_dataflash.c
 *
 * This code is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <config.h>
#include <common.h>
#include <command.h>
#include <asm/addrspace.h>
#include <linux/autoconf.h>
#include <asm/nxc2600.h>

DECLARE_GLOBAL_DATA_PTR;



/* NOTE: AT 25F and SST 25LF series are very similar,
 * but commands for sector erase and chip id differ...
 */

#define FLASH_PAGESIZE		256
#define FLASH_SECTORSIZE	0x1000

/* Flash opcodes. */
#define	OPCODE_WREN		6	/* Write enable */
#define	OPCODE_RDSR		5	/* Read status register */
#define	OPCODE_READ		3	/* Read data bytes */
#define	OPCODE_PP		2	/* Page program */
#define	OPCODE_SE		0xd8	/* Sector erase */
#define	OPCODE_RES		0xab	/* Read Electronic Signature */
#define	OPCODE_RDID		0x9f	/* Read JEDEC ID */

/* Status Register bits. */
#define	SR_WIP			1	/* Write in progress */
#define	SR_WEL			2	/* Write enable latch */
#define	SR_BP0			4	/* Block protect 0 */
#define	SR_BP1			8	/* Block protect 1 */
#define	SR_BP2			0x10	/* Block protect 2 */
#define	SR_SRWD			0x80	/* SR write protect */

/* Define max times to check status register before we give up. */
#define	MAX_READY_WAIT_COUNT	100000


/****************************************************************************/

/*
 * Internal helper functions
 */

int spi_write_then_read(
                const u8 *txbuf, int n_tx,
                u8 *rxbuf, int n_rx,
		int *rx )
{
	int read = -1;
	nxc2600_ssi_enable();
	if(rx!=NULL)
		*rx = 0;
	if(n_tx!=nxc2600_ssi_write(txbuf,n_tx))
		goto _failed;
	if((read=nxc2600_ssi_read(rxbuf,n_rx))<=0)
		goto _failed;
	if(rx!=NULL)
		*rx=read;
_failed:
	nxc2600_ssi_disable();
	return read;
}

/*
 * Read the status register, returning its value in the location
 * Return the status register value.
 * Returns negative if error occurred.
 */
static int read_sr(void)
{
	ssize_t retval;
	u8 code = OPCODE_RDSR;
	u8 val;

	retval = spi_write_then_read( &code, 1, &val, 1, NULL);
	if (retval < 0) {
		return retval;
	}

	return val;
}


/*
 * Set write enable latch with Write Enable command.
 * Returns negative if error occurred.
 */
static inline int write_enable(void)
{
	u8	code = OPCODE_WREN;

	return spi_write_then_read( &code, 1, NULL, 0,NULL);
}


/*
 * Service routine to read status register until ready, or timeout occurs.
 * Returns non-zero if error.
 */
static int wait_till_ready(void)
{
	int count;
	int sr;

	/* one chip guarantees max 5 msec wait here after page writes,
	 * but potentially three seconds (!) after page erase.
	 */
	for (count = 0; count < MAX_READY_WAIT_COUNT; count++) {
		if ((sr = read_sr()) < 0)
			break;
		else if (!(sr & SR_WIP))
			return 0;

		/* REVISIT sometimes sleeping would be best */
	}

	return 1;
}


/*
 * Erase one sector of flash memory at offset ``offset'' which is any
 * address within the sector which should be erased.
 *
 * Returns 0 if successful, non-zero otherwise.
 */
static int erase_sector(u32 offset)
{
	int ret;
	uint8_t command[4];
	/* Wait until finished previous write command. */
	if (wait_till_ready())
		return 1;

	/* Send write enable, then erase commands. */
	write_enable();
	nxc2600_ssi_enable();
	/* Set up command buffer. */
	command[0] = OPCODE_SE;
	command[1] = offset >> 16;
	command[2] = offset >> 8;
	command[3] = offset;

	ret = (nxc2600_ssi_write(command, sizeof(command))!=sizeof(command))? -1:0;
	nxc2600_ssi_disable();
	return ret;
}


/****************************************************************************/

/*
 * MTD implementation
 */

/*
 * Erase an address range on the flash chip.  The address range may extend
 * one or more erase sectors.  Return an error is there is a problem erasing.
 */
int m25p80_erase(u32 addr, int len)
{
        /* now erase those sectors */
        while (len>0) {
                if (erase_sector(addr)) {
                        return -1;
                }
                addr += FLASH_SECTORSIZE;
                len -= FLASH_SECTORSIZE;
        }


        return 0;
}

/*
 * Read an address range from the flash chip.  The address range
 * may be any size provided it is within the physical boundaries.
 */
int m25p80_read(u32 from, int len,
	int *retlen, u_char *buf)
{
	uint8_t command[4];

	/* sanity checks */
	if (!len)
		return 0;

	/* Wait till previous write/erase is done. */
	if (wait_till_ready()) {
		/* REVISIT status return?? */
		return 1;
	}

	/* NOTE:  OPCODE_FAST_READ (if available) is faster... */

	/* Set up the write data buffer. */
	command[0] = OPCODE_READ;
	command[1] = from >> 16;
	command[2] = from >> 8;
	command[3] = from;

	return spi_write_then_read(command,sizeof(command),
				buf,len,retlen);
}

/*
 * Write an address range to the flash chip.  Data must be written in
 * FLASH_PAGESIZE chunks.  The address range may be any size provided
 * it is within the physical boundaries.
 */
int m25p80_write(u32 to, int len,
	int *retlen, const u_char *buf)
{
	int ret;
	uint8_t command[4];
	u32 page_offset, page_size;

	if (retlen)
		*retlen = 0;

	/* sanity checks */
	if (!len)
		return(0);

	/* Wait until finished previous write command. */
	if (wait_till_ready())
		return 1;

	write_enable();

	/* Set up the opcode in the write buffer. */
	command[0] = OPCODE_PP;
	command[1] = to >> 16;
	command[2] = to >> 8;
	command[3] = to;

	/* what page do we start with? */
	page_offset = to % FLASH_PAGESIZE;
	nxc2600_ssi_enable();
	ret = nxc2600_ssi_write(command, sizeof(command));
	if(ret!=sizeof(command))
		goto _failed;

	/* do all the bytes fit onto one page? */
	if (page_offset + len <= FLASH_PAGESIZE) {
			
		*retlen = nxc2600_ssi_write(buf,len);
		if(*retlen!=len)
			goto _failed;
	} else {
		u32 i,tmp;

		/* the size of data remaining on the first page */
		page_size = FLASH_PAGESIZE - page_offset;

		*retlen = nxc2600_ssi_write(buf,page_size);
                if(*retlen!=page_size)
                        goto _failed;
		nxc2600_ssi_disable();
		/* write everything in PAGESIZE chunks */
		for (i = page_size; i < len; i += page_size) {

			page_size = len - i;
			if (page_size > FLASH_PAGESIZE)
				page_size = FLASH_PAGESIZE;

			/* write the next page to flash */
			command[1] = (to + i) >> 16;
			command[2] = (to + i) >> 8;
			command[3] = (to + i);

			wait_till_ready();

			write_enable();
			nxc2600_ssi_enable();
			tmp = nxc2600_ssi_write(buf+i,page_size);
			nxc2600_ssi_disable();
			if(tmp!=page_size)
				goto _failed;
			*retlen += tmp;

	        }
 	}

	return 0;
_failed:
	nxc2600_ssi_disable();
	return -1;
}


/****************************************************************************/

