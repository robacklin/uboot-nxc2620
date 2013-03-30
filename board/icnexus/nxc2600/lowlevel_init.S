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
 * 2007/10/30:
 *
 *	1. disable on-chip rtc.
 *
 * 2007/07/16: initial
 *
 *
 * Author: Hardy Weng <hardy.weng@icnexus.com.tw>
 *
 */

#include <config.h>
#include <version.h>
#include <asm/regdef.h>
#include <linux/autoconf.h>
#include <asm/nxc2600.h>


	.globl  lowlevel_init
lowlevel_init:

	li	t0, NXC2600_INTC_PR|IO_BASE
	li	t1, 0xffffffff
	sw	t1, 0(t0)
	nop
	nop

	li	t0, NXC2600_INTC_MSR|IO_BASE
	li	t1, 0xffffffff
	sw	t1, 0(t0)
	nop
	nop

        /* PLL */
        li      t0, NXC2600_CGM_CCR0|IO_BASE
        li      t1, NXC2600_CGM_CCR0_DEFAULT
        sw      t1, 0(t0)
        nop
	nop

        li      t0, NXC2600_CGM_PCR|IO_BASE
        li      t1, NXC2600_CGM_PCR_DEFAULT
        sw      t1, 0(t0)
        nop
        nop


	/* gpio */
	li	t0, NXC2600_GPIO_SL(0)|IO_BASE
	li	t1, NXC2600_GPIO_SL0_DEFAULT
	sw	t1, 0(t0)
	nop
	nop
	li	t0, NXC2600_GPIO_SH(0)|IO_BASE
	li	t1, NXC2600_GPIO_SH0_DEFAULT
	sw	t1, 0(t0)
	nop
	nop
	
	li      t0, NXC2600_GPIO_SL(1)|IO_BASE
        li      t1, NXC2600_GPIO_SL1_DEFAULT
        sw      t1, 0(t0)
	nop
	nop
        li      t0, NXC2600_GPIO_SH(1)|IO_BASE
        li      t1, NXC2600_GPIO_SH1_DEFAULT
        sw      t1, 0(t0)
	nop
	nop

	li      t0, NXC2600_GPIO_SL(2)|IO_BASE
        li      t1, NXC2600_GPIO_SL2_DEFAULT
        sw      t1, 0(t0)
	nop
	nop
        li      t0, NXC2600_GPIO_SH(2)|IO_BASE
        li      t1, NXC2600_GPIO_SH2_DEFAULT
        sw      t1, 0(t0)
	nop
	nop

	li      t0, NXC2600_GPIO_SL(3)|IO_BASE
        li      t1, NXC2600_GPIO_SL3_DEFAULT
        sw      t1, 0(t0)
	nop
	nop
        li      t0, NXC2600_GPIO_SH(3)|IO_BASE
        li      t1, NXC2600_GPIO_SH3_DEFAULT
        sw      t1, 0(t0)
	nop
	nop

	/* disable on-chip rtc */
	li	t0, NXC2600_RTC_CR|IO_BASE
	li	t1, 0
	sw	t1, 0(t0)
	nop
	nop

	j       ra
	nop
