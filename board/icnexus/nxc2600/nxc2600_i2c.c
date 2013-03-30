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


#include <common.h>
#include <command.h>

#ifdef CONFIG_DRIVER_NXC2600_I2C

#include <linux/autoconf.h>
#include <asm/nxc2600.h>

void
i2c_init (int speed,
	  int slaveadd)
{
	nxc2600_i2c_init( speed );
}

int
i2c_probe (uchar chip)
{
	int ret = 0;
	i2c_init(100000,chip);
	if(nxc2600_i2c_send_start(chip,1))
	{
		nxc2600_i2c_send_stop();
		if(nxc2600_i2c_send_start(chip,0))
			ret = -1;
	}
	nxc2600_i2c_send_stop();
	return ret;
}

int
i2c_read ( uchar chip,
	   uint addr,
	   int alen,
	   uchar * buffer,
	   int len )
{
	int ret = -1;

	if(alen>0)
	{
		if(nxc2600_i2c_send_start( chip , 0))
			goto _no_chip;
		if(nxc2600_i2c_write_data(chip, &addr, 1)!=1)
			goto _send_addr_fail;
		nxc2600_i2c_send_stop();
	}

	if(nxc2600_i2c_send_start( chip , 1))
		goto _no_chip;
	
	if((ret=nxc2600_i2c_read_data( chip, buffer, len))<len)
		goto _read_data_fail;
	ret = 0;

_send_addr_fail:
_no_chip:
_read_data_fail:
	nxc2600_i2c_send_stop();
	return ret;
}

int
i2c_write (uchar chip,
	   uint addr, 
	   int alen, 
	   uchar * buffer, 
	   int len)
{
	int ret = -1;

	if(alen>0)
        {
                if(nxc2600_i2c_send_start( chip , 0))
                        goto _no_chip;
                if(nxc2600_i2c_write_data(chip, &addr, 1)!=1)
                        goto _send_addr_fail;
		nxc2600_i2c_send_stop();
        }

        if(nxc2600_i2c_send_start( chip , 0))
                goto _no_chip;
        if((ret=nxc2600_i2c_write_data( chip, buffer, len))<len)
                goto _write_data_fail;
	ret = 0;

_send_addr_fail:
_no_chip:
_write_data_fail:
        nxc2600_i2c_send_stop();

	return ret;
}

#endif
