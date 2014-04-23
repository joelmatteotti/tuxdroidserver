/* =============== GPL HEADER =====================
 * TuxState.c is part of TuxDroidServer
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

#include <stdio.h>
#include <string.h>
#include <TuxState.h>
#include <TuxStrings.h>

SW_ID_DRIVER getStateNumber(char *stateString)
{
	if(!strcmp("SW_ID_FLIPPERS_POSITION",strtoupper(stateString)))
		return SW_ID_FLIPPERS_POSITION;
	if(!strcmp("SW_ID_FLIPPERS_REMAINING_MVM",strtoupper(stateString)))
		return SW_ID_FLIPPERS_REMAINING_MVM;
	if(!strcmp("SW_ID_SPINNING_DIRECTION",strtoupper(stateString)))
		return SW_ID_SPINNING_DIRECTION;
	if(!strcmp("SW_ID_SPINNING_REMAINING_MVM",strtoupper(stateString)))
		return SW_ID_SPINNING_REMAINING_MVM;
	if(!strcmp("SW_ID_LEFT_WING_BUTTON",strtoupper(stateString)))
		return SW_ID_LEFT_WING_BUTTON;
	if(!strcmp("SW_ID_RIGHT_WING_BUTTON",strtoupper(stateString)))
		return SW_ID_RIGHT_WING_BUTTON;
	if(!strcmp("SW_ID_HEAD_BUTTON",strtoupper(stateString)))
		return SW_ID_HEAD_BUTTON;
	if(!strcmp("SW_ID_REMOTE_BUTTON",strtoupper(stateString)))
		return SW_ID_REMOTE_BUTTON;
	if(!strcmp("SW_ID_MOUTH_POSITION",strtoupper(stateString)))
		return SW_ID_MOUTH_POSITION;
	if(!strcmp("SW_ID_MOUTH_REMAINING_MVM",strtoupper(stateString)))
		return SW_ID_MOUTH_REMAINING_MVM;
	if(!strcmp("SW_ID_EYES_POSITION",strtoupper(stateString)))
		return SW_ID_EYES_POSITION;
	if(!strcmp("SW_ID_EYES_REMAINING_MVM",strtoupper(stateString)))
		return SW_ID_EYES_REMAINING_MVM;
	if(!strcmp("SW_ID_DESCRIPTOR_COMPLETE",strtoupper(stateString)))
		return SW_ID_DESCRIPTOR_COMPLETE;
	if(!strcmp("SW_ID_RF_STATE",strtoupper(stateString)))
		return SW_ID_RF_STATE;
	if(!strcmp("SW_ID_DONGLE_PLUG",strtoupper(stateString)))
		return SW_ID_DONGLE_PLUG;
	if(!strcmp("SW_ID_CHARGER_STATE",strtoupper(stateString)))
		return SW_ID_CHARGER_STATE;
	if(!strcmp("SW_ID_BATTERY_LEVEL",strtoupper(stateString)))
		return SW_ID_BATTERY_LEVEL;
	if(!strcmp("SW_ID_BATTERY_STATE",strtoupper(stateString)))
		return SW_ID_BATTERY_STATE;
	if(!strcmp("SW_ID_LIGHT_LEVEL",strtoupper(stateString)))
		return SW_ID_LIGHT_LEVEL;
	if(!strcmp("SW_ID_LEFT_LED_STATE",strtoupper(stateString)))
		return SW_ID_LEFT_LED_STATE;
	if(!strcmp("SW_ID_RIGHT_LED_STATE",strtoupper(stateString)))
		return SW_ID_RIGHT_LED_STATE;
	if(!strcmp("SW_ID_CONNECTION_QUALITY",strtoupper(stateString)))
		return SW_ID_CONNECTION_QUALITY;
	if(!strcmp("SW_ID_AUDIO_FLASH_PLAY",strtoupper(stateString)))
		return SW_ID_AUDIO_FLASH_PLAY;
	if(!strcmp("SW_ID_AUDIO_GENERAL_PLAY",strtoupper(stateString)))
		return SW_ID_AUDIO_GENERAL_PLAY;
	if(!strcmp("SW_ID_FLASH_PROG_CURR_TRACK",strtoupper(stateString)))
		return SW_ID_FLASH_PROG_CURR_TRACK;
	if(!strcmp("SW_ID_FLASH_PROG_LAST_TRACK_SIZE",strtoupper(stateString)))
		return SW_ID_FLASH_PROG_LAST_TRACK_SIZE;
	if(!strcmp("SW_ID_TUXCORE_SYMBOLIC_VERSION",strtoupper(stateString)))
		return SW_ID_TUXCORE_SYMBOLIC_VERSION;
	if(!strcmp("SW_ID_TUXAUDIO_SYMBOLIC_VERSION",strtoupper(stateString)))
		return SW_ID_TUXAUDIO_SYMBOLIC_VERSION;
	if(!strcmp("SW_ID_FUXUSB_SYMBOLIC_VERSION",strtoupper(stateString)))
		return SW_ID_FUXUSB_SYMBOLIC_VERSION;
	if(!strcmp("SW_ID_FUXRF_SYMBOLIC_VERSION",strtoupper(stateString)))
		return SW_ID_FUXRF_SYMBOLIC_VERSION;
	if(!strcmp("SW_ID_TUXRF_SYMBOLIC_VERSION",strtoupper(stateString)))
		return SW_ID_TUXRF_SYMBOLIC_VERSION;
	if(!strcmp("SW_ID_DRIVER_SYMBOLIC_VERSION",strtoupper(stateString)))
		return SW_ID_DRIVER_SYMBOLIC_VERSION;
	if(!strcmp("SW_ID_SOUND_REFLASH_BEGIN",strtoupper(stateString)))
		return SW_ID_SOUND_REFLASH_BEGIN;
	if(!strcmp("SW_ID_SOUND_REFLASH_END",strtoupper(stateString)))
		return SW_ID_SOUND_REFLASH_END;
	if(!strcmp("SW_ID_SOUND_REFLASH_CURRENT_TRACK",strtoupper(stateString)))
		return SW_ID_SOUND_REFLASH_CURRENT_TRACK;
	if(!strcmp("SW_ID_EYES_MOTOR_ON",strtoupper(stateString)))
		return SW_ID_EYES_MOTOR_ON;
	if(!strcmp("SW_ID_MOUTH_MOTOR_ON",strtoupper(stateString)))
		return SW_ID_MOUTH_MOTOR_ON;
	if(!strcmp("SW_ID_FLIPPERS_MOTOR_ON",strtoupper(stateString)))
		return SW_ID_FLIPPERS_MOTOR_ON;
	if(!strcmp("SW_ID_SPIN_LEFT_MOTOR_ON",strtoupper(stateString)))
		return SW_ID_SPIN_LEFT_MOTOR_ON;
	if(!strcmp("SW_ID_SPIN_RIGHT_MOTOR_ON",strtoupper(stateString)))
		return SW_ID_SPIN_RIGHT_MOTOR_ON;
	if(!strcmp("SW_ID_FLASH_SOUND_COUNT",strtoupper(stateString)))
		return SW_ID_FLASH_SOUND_COUNT;

	return -1; /* error */
}
