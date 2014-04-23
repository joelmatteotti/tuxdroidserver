/* =============== GPL HEADER =====================
 * TuxDroid.c is part of TuxDroidServer
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

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <TuxDroid.h>
#include <TuxTypes.h>
#include <TuxServer.h>
#include <TuxMessages.h>
#include <TuxDriver.h>
#include <TuxAudio.h>
#include <TuxLogger.h>
#include <TuxTime.h>

/**
 Initialize TuxDroid and TuxAudio drivers
 @return initialization state
*/
bool InitializeTuxDroid(void)
{

	/* TuxDrv_SetLogLevel(0); */ /* DEBUG */ /* TODO : debug function handle :/ */


	TuxLogger_Info("TuxDroid initialization",NULL);
	if(TuxDriver_Initialize() != E_TUXDRV_NOERROR) /* driver initialization*/
		return false;
	TuxLogger_Debug("TuxDroid has been initialized",NULL);

	TuxLogger_Info("TuxAudio initialization",NULL);
	if(TuxAudio_Initialize() != E_TUXAUDIO_NOERROR)
		return false;
	TuxLogger_Debug("TuxAudio has been initialized",NULL);


	TuxLogger_Debug("Defining events functions handles ..",NULL);
	/* Handle TuxDroid's button */
	OnTuxRemoteButtonPressed = onRemoteButtonPressed;
	OnTuxRemoteButtonReleased = onRemoteButtonReleased;
	
	OnTuxLeftButtonPressed = onLeftButtonPressed;
	OnTuxLeftButtonReleased = onLeftButtonReleased;
	
	OnTuxRightButtonPressed = onRightButtonPressed;
	OnTuxRightButtonReleased = onRightButtonReleased;
	
	OnTuxHeadButtonPressed = onHeadButtonPressed;
	OnTuxHeadButtonReleased = onHeadButtonReleased;
	
	OnTuxChargerPlugged = onChargerPlugged;
	OnTuxChargerUnPlugged = onChargerUnPlugged;
	OnTuxDongleConnected = onDongleConnected;
	OnTuxDongleDisconnected = onDongleDisconnected;
	TuxLogger_Debug("Events functions handles have been defined",NULL);

	TuxLogger_Debug("Starting driver ..",NULL);
	/* Starting driver */
	TuxDriver_Start();

	/* Waiting driver to be initialized */
	#ifdef _WIN32
	Sleep(1000);
	#else
	TuxSleep(1000);
	#endif

	return true;
}

/* Handle events */
void onRemoteButtonReleased(tux_client client)
{
	SendMsgToAll(TDSM_REMOTEBUTTON_RELEASE,NULL);
}

void onRemoteButtonPressed(tux_client client, char *button)
{
	char *cmd = (char *)malloc(sizeof(char)*(strlen(button) + 11));

	if(client != NULL && strcmp(client->username,"0"))
		sprintf(cmd,TDSM_REMOTEBUTTONMULTI_CLICKED,button,client->username);
	else
		sprintf(cmd,TDSM_REMOTEBUTTON_CLICKED,button);

	SendMsgToAll(cmd,NULL);


	if(cmd != NULL)
	{
		/*free(cmd);
		cmd=NULL;*/
	}
}

void onLeftButtonPressed(void)
{
	SendMsgToAll(TDSM_LEFTBUTTON_CLICKED,NULL);
}

void onLeftButtonReleased(void)
{
	SendMsgToAll(TDSM_LEFTBUTTON_RELEASED,NULL);
}

void onRightButtonPressed(void)
{
	SendMsgToAll(TDSM_RIGHTBUTTON_CLICKED,NULL);
}

void onRightButtonReleased(void)
{
	SendMsgToAll(TDSM_RIGHTBUTTON_RELEASED,NULL);
}

void onHeadButtonPressed(void)
{
	SendMsgToAll(TDSM_HEADBUTTON_CLICKED,NULL);
}

void onHeadButtonReleased(void)
{
	SendMsgToAll(TDSM_HEADBUTTON_RELEASED,NULL);
}

void onChargerPlugged(void)
{
	SendMsgToAll(TDSM_CHARGER_PLUGGED,NULL);
}

void onChargerUnPlugged(void)
{
	SendMsgToAll(TDSM_CHARGER_UNPLUGGED,NULL);
}

void onDongleConnected(void)
{
	SendMsgToAll(TDSM_DONGLE_CONNECTED,NULL);
}

void onDongleDisconnected(void)
{
	SendMsgToAll(TDSM_DONGLE_DISCONNECTED,NULL);
}

