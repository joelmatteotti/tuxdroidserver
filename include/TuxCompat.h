/* =============== GPL HEADER =====================
 * TuxCompat.h is part of TuxDroidServer
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


/* Tweaks allowing us to compile on differents compilers on Windows */
/* MingW32 / Orwell Dev-C++ */

#ifndef __TUXCOMPAT_H__
#define __TUXCOMPAT_H__

#if !defined(_WIN32)
	#if defined(WIN32)
		#define _WIN32 WIN32
	#endif

	#if defined(__WIN32__)
		#define _WIN32 __WIN32__
	#endif
#endif

#if defined(WIN32) || defined(__WIN32__)
	#define sleep(t) Sleep(t)
	#define close(s) closesocket(s)
#endif

#endif
