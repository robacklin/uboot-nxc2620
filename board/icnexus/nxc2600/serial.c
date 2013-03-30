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


#include <config.h>
#include <common.h>
#include <command.h>
#include <asm/addrspace.h>
#include <linux/autoconf.h>
#include <asm/nxc2600.h>

DECLARE_GLOBAL_DATA_PTR;

void 
serial_setbrg (void)
{
	register unsigned int baud_div;
	int baudrate = CONFIG_BAUDRATE;
#ifndef CONFIG_NAND_SPL
	baudrate = gd->baudrate;
#endif
	baud_div = CONFIG_NXC2600_EXTAL_CLOCK / 16 / baudrate;

	REG8(NXC2600_UART_LCR(CONFIG_NXC2600_DEFAULT_DEBUG_PORT)) |= NXC2600_UART_LCR_DLAB;

	REG8(NXC2600_UART_DLHR(CONFIG_NXC2600_DEFAULT_DEBUG_PORT)) = (baud_div>>8)&0xff;
	REG8(NXC2600_UART_DLLR(CONFIG_NXC2600_DEFAULT_DEBUG_PORT)) = baud_div&0xff;

	REG8(NXC2600_UART_LCR(CONFIG_NXC2600_DEFAULT_DEBUG_PORT)) &= ~NXC2600_UART_LCR_DLAB;

}

int 
serial_init (void)
{

	REG8(NXC2600_UART_IER(CONFIG_NXC2600_DEFAULT_DEBUG_PORT)) = 0;
	REG8(NXC2600_UART_FCR(CONFIG_NXC2600_DEFAULT_DEBUG_PORT)) = 0;

	/* Set both receiver and transmitter in UART mode (not SIR) */
	REG8(NXC2600_UART_ISR(CONFIG_NXC2600_DEFAULT_DEBUG_PORT)) = ~(NXC2600_UART_ISR_XMITIR_SIR|NXC2600_UART_ISR_RCVEIR_SIR);

	/* Set databits, stopbits and parity. (8-bit data, 1 stopbit, no parity) */
	REG8(NXC2600_UART_LCR(CONFIG_NXC2600_DEFAULT_DEBUG_PORT)) = NXC2600_UART_LCR_WLS(8)|NXC2600_UART_LCR_SBLS(1);

	/* Set baud rate */
	serial_setbrg();

	/* Enable UART unit, enable and clear FIFO */
	REG8(NXC2600_UART_FCR(CONFIG_NXC2600_DEFAULT_DEBUG_PORT)) = NXC2600_UART_FCR_UUE|NXC2600_UART_FCR_FME|
							     NXC2600_UART_FCR_RFRT|NXC2600_UART_FCR_TFRT;
	return 0;
}

void 
serial_putc (register const char c)
{
	int timeout = 1000;
	if (c == '\n') serial_putc ('\r');

	while ( !(REG8(NXC2600_UART_LSR(CONFIG_NXC2600_DEFAULT_DEBUG_PORT))&NXC2600_UART_LSR_THRE) ) {if(timeout--) udelay(1); else break;}

        REG8(NXC2600_UART_THR(CONFIG_NXC2600_DEFAULT_DEBUG_PORT)) = (unsigned char)c;

}

void 
serial_puts (register const char *s)
{
	while (*s) {
		serial_putc (*s++);
	}
}

#ifndef CONFIG_NAND_SPL

int 
serial_getc (void)
{

	while (!serial_tstc());

	return REG8(NXC2600_UART_RBR(CONFIG_NXC2600_DEFAULT_DEBUG_PORT));
}

int 
serial_tstc (void)
{
	return (REG8(NXC2600_UART_LSR(CONFIG_NXC2600_DEFAULT_DEBUG_PORT)) & NXC2600_UART_LSR_DRY);
}
#endif
