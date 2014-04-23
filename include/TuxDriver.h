/* =============== GPL HEADER =====================
 * TuxDriver.h is part of TuxDroidServer
 * Copyleft (C) 2011-2012 - Joel Matteotti <joel _DOT_ matteotti _AT_ free _DOT_ fr>
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

#ifndef __TUXDRIVER_H__
#define __TUXDRIVER_H__

#include <TuxVersion.h>
#include <TuxTypes.h>
#include <TuxServer.h>


#define TUXDRIVER_AUTHOR "Joel Matteotti" /* Initial developer (can't change !) */

#define TUXDRIVER_MAINTENER "Joel Matteotti" /* Current developer (if I let this happen :P) */

#include <stdio.h>
#include <pthread.h>
#include <inttypes.h> /* uintptr_t */


#ifdef _WIN32
	#include <windows.h> /* DLL management */
#else
	#include <dlfcn.h> /* SO management */
#endif

typedef unsigned char byte; /* byte type */


/*
----------------------------
 FROM tux_driver.h
----------------------------
*/

#include <TuxState.h>

/**
 * Error codes
 */
#define TUX_ERROR_BEGIN 256
typedef int TuxDrvError;
typedef enum
{
	E_TUXDRV_NOERROR = 0,
	E_TUXDRV_PARSERISDISABLED = TUX_ERROR_BEGIN,
	E_TUXDRV_INVALIDCOMMAND,
	E_TUXDRV_STACKOVERFLOW,
	E_TUXDRV_FILEERROR,
	E_TUXDRV_BADWAVFILE,
	E_TUXDRV_INVALIDIDENTIFIER,
	E_TUXDRV_INVALIDNAME,
	E_TUXDRV_INVALIDPARAMETER,
	E_TUXDRV_BUSY,
	E_TUXDRV_WAVSIZEEXCEDED,

	E_TUXDRV_NOTVALID_LED, /* Invalid LED choice */
	E_TUXDRV_NOTVALID_EFFECT, /* Invalid pulse effect */
	E_TUXDRV_DRIVERNOTFOUND, /* Driver not found */

	E_TUXDRV_ALREADYINITIALIZED, /* Already initialized */
	E_TUXDRV_NOTINITIALIZED, /* Not initialized */
} tux_drv_error_t;

/**
 * Simple callback definition.
 */
typedef void(*drv_simple_callback_t)(void);
typedef void(*event_callback_t)(char *event); /*tux_sw_status.h*/


/*
 DLL handle
*/
void* DLLHANDLE;


/*
-----------------------------
  Functions' declarations
------------------------------
*/

typedef void (*TuxDrv_Stop_t)(void);
TuxDrv_Stop_t TuxDrv_Stop;


typedef void (*TuxDrv_Start_t)(void);
TuxDrv_Start_t TuxDrv_Start;

typedef TuxDrvError (*TuxDrv_SetStatusCallback_t)(event_callback_t funct);
TuxDrv_SetStatusCallback_t TuxDrv_SetStatusCallback;

typedef void (*TuxDrv_SetEndCycleCallback_t)(drv_simple_callback_t funct);
TuxDrv_SetEndCycleCallback_t TuxDrv_SetEndCycleCallback;

typedef void (*TuxDrv_SetDongleConnectedCallback_t)(drv_simple_callback_t funct);
TuxDrv_SetDongleConnectedCallback_t TuxDrv_SetDongleConnectedCallback;

typedef void (*TuxDrv_SetDongleDisconnectedCallback_t)(drv_simple_callback_t funct);
TuxDrv_SetDongleDisconnectedCallback_t TuxDrv_SetDongleDisconnectedCallback;

typedef void (*TuxDrv_ResetDongle_t)(void);
TuxDrv_ResetDongle_t TuxDrv_ResetDongle;

typedef TuxDrvError (*TuxDrv_PerformCommand_t)(double delay, const char *cmd_str);
TuxDrv_PerformCommand_t TuxDrv_PerformCommand;

typedef TuxDrvError (*TuxDrv_PerformMacroText_t)(const char *cmd_macro);
TuxDrv_PerformMacroText_t TuxDrv_PerformMacroText;

typedef TuxDrvError (*TuxDrv_PerformMacroFile_t)(const char *macro_file);
TuxDrv_PerformMacroFile_t TuxDrv_PerformMacroFile;

typedef void (*TuxDrv_ClearCommandStack_t)(void);
TuxDrv_ClearCommandStack_t TuxDrv_ClearCommandStack;

typedef void (*TuxDrv_ResetPositions_t)(void);
TuxDrv_ResetPositions_t TuxDrv_ResetPositions;

typedef TuxDrvError (*TuxDrv_GetStatusName_t)(int id, char* name);
TuxDrv_GetStatusName_t TuxDrv_GetStatusName;

typedef TuxDrvError (*TuxDrv_GetStatusId_t)(char* name, int *id);
TuxDrv_GetStatusId_t TuxDrv_GetStatusId;

typedef TuxDrvError (*TuxDrv_GetStatusState_t)(int id, char *state);
TuxDrv_GetStatusState_t TuxDrv_GetStatusState;

typedef TuxDrvError (*TuxDrv_GetStatusValue_t)(int id, char *value);
TuxDrv_GetStatusValue_t TuxDrv_GetStatusValue;

typedef void (*TuxDrv_GetAllStatusState_t)(char *state);
TuxDrv_GetAllStatusState_t TuxDrv_GetAllStatusState;

typedef TuxDrvError (*TuxDrv_SoundReflash_t)(const char *tracks);
TuxDrv_SoundReflash_t TuxDrv_SoundReflash;

typedef bool (*TuxDrv_Eyes_Off_t)(void);
TuxDrv_Eyes_Off_t TuxDrv_Eyes_Off;

typedef bool (*TuxDrv_Mouth_Off_t)(void);
TuxDrv_Mouth_Off_t TuxDrv_Mouth_Off;

typedef bool (*TuxDrv_Spinning_Off_t)(void);
TuxDrv_Spinning_Off_t TuxDrv_Spinning_Off;

typedef bool (*TuxDrv_Flippers_Off_t)(void);
TuxDrv_Flippers_Off_t TuxDrv_Flippers_Off;

typedef void (*TuxDrv_Update_Light_t)(void);
TuxDrv_Update_Light_t TuxDrv_Update_Light;

typedef char * (*TuxDrv_GetFlashSound_Infos_t)(char *p);
TuxDrv_GetFlashSound_Infos_t TuxDrv_GetFlashSound_Infos;

/*
typedef void (*TuxDrv_SetLogLevel_t)(int log_level);
TuxDrv_SetLogLevel_t TuxDrv_SetLogLevel;

typedef void (*TuxDrv_SetLogTarget_t)(int target);
TuxDrv_SetLogTarget_t TuxDrv_SetLogTarget;
*/

/*==============================================*/

/*
----------------
  DONGLE
-----------------
*/
typedef void(*OnTuxDongleConnected_t)(void);
typedef void(*OnTuxDongleDisconnected_t)(void);

OnTuxDongleConnected_t OnTuxDongleConnected;
OnTuxDongleDisconnected_t OnTuxDongleDisconnected;
/*=======================*/


/*
-------------------
  Buttons
-------------------
*/
typedef void(*OnTuxLeftButtonPressed_t)(void);
typedef void(*OnTuxLeftButtonReleased_t)(void);
typedef void(*OnTuxRightButtonPressed_t)(void);
typedef void(*OnTuxRightButtonReleased_t)(void);
typedef void(*OnTuxHeadButtonPressed_t)(void);
typedef void(*OnTuxHeadButtonReleased_t)(void);
typedef void(*OnTuxRemoteButtonPressed_t)(tux_client client, char *);
typedef void(*OnTuxRemoteButtonReleased_t)(tux_client client);


OnTuxLeftButtonPressed_t OnTuxLeftButtonPressed;
OnTuxLeftButtonReleased_t OnTuxLeftButtonReleased;
OnTuxRightButtonPressed_t OnTuxRightButtonPressed;
OnTuxRightButtonPressed_t OnTuxRightButtonReleased;
OnTuxHeadButtonPressed_t OnTuxHeadButtonPressed;
OnTuxHeadButtonPressed_t OnTuxHeadButtonReleased;
OnTuxRemoteButtonPressed_t OnTuxRemoteButtonPressed;
OnTuxRemoteButtonReleased_t OnTuxRemoteButtonReleased;
/*==============================================*/


/*
------------------
 Charger
------------------
*/
typedef void(*OnTuxChargerPlugged_t)(void);
typedef void(*OnTuxChargerUnPlugged_t)(void);

OnTuxChargerPlugged_t OnTuxChargerPlugged;
OnTuxChargerUnPlugged_t OnTuxChargerUnPlugged;
/*==============================================*/


/*
-----------------
End Cycle
-----------------
*/
typedef void(*OnTuxEndCycle_t)(void);
OnTuxEndCycle_t OnTuxEndCycle;
/*==============================================*/


void *IMPORT_FUNC(void *HANDLE, const char *func);
void TuxDriver_Stop(void);
TuxDrvError TuxDriver_Initialize(void);
void* TuxDriver_DrvStart(void *data);
const char *TuxDriver_strerror(TuxDrvError error_code);
void StatusEventCallback(char *status_str);
void Tux_DongleConnected(void);
void Tux_DongleDisconnected(void);
void Tux_ResetDongle(void);
TuxDrvError Tux_PlayFlashSound(byte sound, byte volume);
TuxDrvError Tux_SetCallbackEvent(event_callback_t funct);
void Tux_SetEndCycleCallbackEvent(drv_simple_callback_t funct);
void EndCycleEventCallback(void);
void TuxDriver_Stop(void);
void TuxDriver_Start(void);
TuxDrvError Tux_SoundReflash(const char *tracks);
TuxDrvError Tux_SetFlippersSpeed(byte speed);
TuxDrvError Tux_Flippers_Up(void);
TuxDrvError Tux_Flippers_Down(void);
TuxDrvError Tux_FlippersUpDown(int count);
TuxDrvError Tux_FlippersUpDown_OnDuration(double duration);
TuxDrvError Tux_Open_Eyes(void);
TuxDrvError Tux_Close_Eyes(void);
TuxDrvError Tux_OpenClose_Eyes(int count);
TuxDrvError Tux_OpenClose_Eyes_OnDuration(double duration);
TuxDrvError Tux_Open_Mouth(void);
TuxDrvError Tux_Close_Mouth(void);
TuxDrvError Tux_OpenClose_Mouth(int count);
TuxDrvError Tux_OpenClose_Mouth_OnDuration(double duration);
TuxDrvError Tux_SetRotationSpeed(byte speed);
TuxDrvError Tux_RotateLeft(int count);
TuxDrvError Tux_RotateLeft_OnDuration(double duration);
TuxDrvError Tux_RotateRight(int count);
TuxDrvError Tux_RotateRight_OnDuration(double duration);
TuxDrvError Tux_Led_On(leds_t LED);
TuxDrvError Tux_Led_Off(leds_t LED);
TuxDrvError Tux_BlinkLeds(leds_t LED, int count, double delay);
TuxDrvError Tux_PulseLeds(leds_t LED, double min_intensity, double max_intensity, byte count, double period, effect_type_t effect, double speed, byte step);

#endif
