/*
 * (C) Copyright 2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>


static inline void mips_compare_set(u32 v)
{
	asm volatile ("mtc0 %0, $11" : : "r" (v));
}

static inline void mips_count_set(u32 v)
{
	asm volatile ("mtc0 %0, $9" : : "r" (v));
}


static inline u32 mips_count_get(void)
{
	u32 count;

	asm volatile ("mfc0 %0, $9" : "=r" (count) :);
	return count;
}

/*
 * timer without interrupts
 */
#ifdef CONFIG_ICNEXUS_NXC2600
#include <command.h>
#include <linux/autoconf.h>
#include <asm/nxc2600.h>

int timer_init(void)
{
	nxc2600_start_system_timer(0, 0);
        return 0;
}

void reset_timer(void)
{
	nxc2600_start_system_timer(0, 0);
}

ulong get_timer(ulong base)
{
        return ((nxc2600_get_system_timer_counter()-base)/CFG_HZ);
}

void set_timer(ulong t)
{
	nxc2600_start_system_timer(0, t);
}

void udelay (unsigned long usec)
{
        ulong tmo;
        ulong start;
	start = get_timer(0);

        tmo = ((usec * (CONFIG_NXC2600_EXTAL_CLOCK/1000000))/CFG_HZ);
        while ((ulong)((get_timer(0) - start)) < tmo);
}

unsigned long long get_ticks(void)
{
        return get_timer(0);
}

#else
int timer_init(void)
{
	mips_compare_set(0);
	mips_count_set(0);

	return 0;
}

void reset_timer(void)
{
	mips_count_set(0);
}

ulong get_timer(ulong base)
{
	return mips_count_get() - base;
}

void set_timer(ulong t)
{
	mips_count_set(t);
}

void udelay (unsigned long usec)
{
	ulong tmo;
	ulong start = get_timer(0);

	tmo = usec * (CFG_HZ / 1000000);
	while ((ulong)((mips_count_get() - start)) < tmo)
		/*NOP*/;
}

/*
 * This function is derived from PowerPC code (read timebase as long long).
 * On MIPS it just returns the timer value.
 */
unsigned long long get_ticks(void)
{
	return mips_count_get();
}
#endif
/*
 * This function is derived from PowerPC code (timebase clock frequency).
 * On MIPS it returns the number of timer ticks per second.
 */
ulong get_tbclk(void)
{
	return CFG_HZ;
}
