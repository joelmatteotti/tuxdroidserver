/* =============== GPL HEADER =====================
 * TuxTime.c is part of TuxDroidServer
 * Copyleft (C) 2012 - Joel Matteotti <joel _DOT_ matteotti _AT_ free _DOT_ fr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
 *
 * ====================================================
*/

#include <TuxCompat.h>
#include <TuxTime.h>

#ifndef _WIN32

/* Wait during x microseconds */
int usleep(useconds_t microseconds)
{
	if(microseconds)
	{
		const useconds_t one_second = 1000000;
		struct timeval tv_delay;

		tv_delay.tv_sec = microseconds / one_second;
		tv_delay.tv_usec = microseconds % one_second;

		return select(0,NULL,NULL,NULL,&tv_delay);
	}

	return 0;
}

/* Wait during x microseconds */
int TuxSleep(int miliseconds)
{
	return usleep(miliseconds * 1000);
}

#endif

/* Get timestamp */
long Tux_TimeStamp()
{
	time_t tTime;
	time(&tTime);
	return tTime;
}
