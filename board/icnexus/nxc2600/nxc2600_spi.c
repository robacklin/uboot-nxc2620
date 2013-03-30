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
 *
 * 2008/01/16: initial
 *
 */

#include <common.h>
#include <command.h>
#include <asm/addrspace.h>
#include <asm/inca-ip.h>
#include <config.h>
#include <spi.h>
#include <linux/autoconf.h>
#include <asm/nxc2600.h>

#if (CONFIG_COMMANDS & CFG_CMD_SPI)

void 
spi_flash_chipsel(int cs)
{
	if(cs)
	{
		nxc2600_ssi_init(NXC2600_SSI_MODE_CE,
				 NXC2600_SSI_FORMAT_MOTO_SPI,
				 1,
				 8,
				 1000000);
		nxc2600_ssi_enable();
	}
	else
	{
		nxc2600_ssi_disable();
	}
}


spi_chipsel_type spi_chipsel[] = 
{
        spi_flash_chipsel,
};
int spi_chipsel_cnt = sizeof(spi_chipsel) / sizeof(spi_chipsel[0]);


int 
spi_xfer(spi_chipsel_type chipsel, 
	 int bitlen, 
	 uchar *dout, 
	 uchar *din)
{
	int bits_per_unit = 8;
        int j;
	int ret = -1;

	if(bits_per_unit==0)
		bits_per_unit = 8;

	if(bits_per_unit!=8 && bits_per_unit != 16)
		return -1;

        printf("spi_xfer: chipsel %08X dout %08X din %08X bitlen %d unit %d\n",
                (int)chipsel, dout, din, bitlen,bits_per_unit);

#if 0
        if(chipsel != NULL) {
                (*chipsel)(1);     /* select the target chip */
        }
#endif
	spi_flash_chipsel(1);

	if(bits_per_unit==8)
	{
        	for ( j= 0; j < ((bitlen + 7) / 8); j++)
		{
			if(dout!=NULL)
			if(nxc2600_ssi_write(&dout[j],1)!=1)
				goto _failed;
			if(din!=NULL)
			if(nxc2600_ssi_read(&din[j],1)!=1)
				goto _failed;
		}
		ret = 0;
	}

	ret = 0;

_failed:
#if 0
        if(chipsel != NULL) {
                (*chipsel)(0);     /* deselect the target chip */
        }
#endif
	spi_flash_chipsel(0);
        return ret;

}

#endif
