/* =============== GPL HEADER =====================
 * TuxServer.h is part of TuxDroidServer
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


#ifndef __TUXSERVER_H__
#define __TUXSERVER_H__

#include <TuxVersion.h>

#define TUXSERVER_AUTHOR "Joel Matteotti" /* Initial developer (can't change !) */

#define TUXSERVER_MAINTENER "Joel Matteotti" /* Current developer (if I let this happen :P) */

#include <stdbool.h>
#include <TuxTypes.h>



/* Prototypes */
void* ReadClient(void *data);
void SendMsgToAll(char *msg, tux_client except_client);
void SendMsgToClient(tux_client client, char *message);
void ParseCommand(tux_client client, char *cmd);
void loadConfigFile(void);
void basicStart(void);
void *startServer(void *data);
void stopServer(void);
bool InitializeTuxDroid();
void onRemoteButtonPressed(tux_client client, char *button);
void onLeftButtonPressed();
void onRightButtonPressed();
void onHeadButtonPressed();
void onChargerPlugged();
void onChargerUnPlugged();
void onDongleConnected();
void onDongleDisconnected();
void *syncMouthMovements(void *data);
void remoteControl(char *button);

bool isRealyUniqueID(char *ID);
void removeUniqueID(char *ID);
void addUniqueID(char *ID);

#endif
