/* =============== GPL HEADER =====================
 * TuxState.h is part of TuxDroidServer
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

#ifndef __TUXSTATE_H__
#define __TUXSTATE_H__

/**
 * Id enumeration of high level status.
 */
typedef enum {
	SW_ID_FLIPPERS_POSITION = 0,
	SW_ID_FLIPPERS_REMAINING_MVM,
	SW_ID_SPINNING_DIRECTION,
	SW_ID_SPINNING_REMAINING_MVM,
	SW_ID_LEFT_WING_BUTTON,
	SW_ID_RIGHT_WING_BUTTON,
	SW_ID_HEAD_BUTTON,
	SW_ID_REMOTE_BUTTON,
	SW_ID_MOUTH_POSITION,
	SW_ID_MOUTH_REMAINING_MVM,
	SW_ID_EYES_POSITION,
	SW_ID_EYES_REMAINING_MVM,
	SW_ID_DESCRIPTOR_COMPLETE,
	SW_ID_RF_STATE,
	SW_ID_DONGLE_PLUG,
	SW_ID_CHARGER_STATE,
	SW_ID_BATTERY_LEVEL,
	SW_ID_BATTERY_STATE,
	SW_ID_LIGHT_LEVEL,
	SW_ID_LEFT_LED_STATE,
	SW_ID_RIGHT_LED_STATE,
	SW_ID_CONNECTION_QUALITY,
	SW_ID_AUDIO_FLASH_PLAY,
	SW_ID_AUDIO_GENERAL_PLAY,
	SW_ID_FLASH_PROG_CURR_TRACK,
	SW_ID_FLASH_PROG_LAST_TRACK_SIZE,
	SW_ID_TUXCORE_SYMBOLIC_VERSION,
	SW_ID_TUXAUDIO_SYMBOLIC_VERSION,
	SW_ID_FUXUSB_SYMBOLIC_VERSION,
	SW_ID_FUXRF_SYMBOLIC_VERSION,
	SW_ID_TUXRF_SYMBOLIC_VERSION,
	SW_ID_DRIVER_SYMBOLIC_VERSION,
	SW_ID_SOUND_REFLASH_BEGIN,
	SW_ID_SOUND_REFLASH_END,
	SW_ID_SOUND_REFLASH_CURRENT_TRACK,
	SW_ID_EYES_MOTOR_ON,
	SW_ID_MOUTH_MOTOR_ON,
	SW_ID_FLIPPERS_MOTOR_ON,
	SW_ID_SPIN_LEFT_MOTOR_ON,
	SW_ID_SPIN_RIGHT_MOTOR_ON,
	SW_ID_FLASH_SOUND_COUNT,
} SW_ID_DRIVER;

SW_ID_DRIVER getStateNumber(char *stateString);
#endif
