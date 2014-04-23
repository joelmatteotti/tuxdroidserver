/* =============== GPL HEADER =====================
 * TuxDroid.h is part of TuxDroidServer
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

#ifndef __TUXDROID_H__
#define __TUXDROID_H__

#include <stdio.h>
#include <stdbool.h>
#include <TuxTypes.h>

bool InitializeTuxDroid(void);
void onRemoteButtonPressed(tux_client client, char *button);
void onRemoteButtonReleased(tux_client client);
void onLeftButtonPressed(void);
void onLeftButtonReleased(void);
void onRightButtonPressed(void);
void onRightButtonReleased(void);
void onHeadButtonPressed(void);
void onHeadButtonReleased(void);
void onChargerPlugged(void);
void onChargerUnPlugged(void);
void onDongleConnected(void);
void onDongleDisconnected(void);


#endif
