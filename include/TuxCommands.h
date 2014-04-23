/* =============== GPL HEADER =====================
 * TuxCommands.h is part of TuxDroidServer
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

#ifndef __TUXCOMMANDS_H__
#define __TUXCOMMANDS_H__

#include <stdio.h>
#include <TuxTypes.h>


typedef struct
{
	tux_client client;
	char *file;
} flac_convert_t;
typedef flac_convert_t *flac_convert;

void* convertToFlac(void *data);
void Tux_ASR(tux_client client, char *file);
void Tux_Flac(tux_client client, char *file);

void Tux_Open(char *motor);
void Tux_Close(char *motor);
void Tux_OpenClose(char *motor, int count);
void Tux_Leds_OnOff(char *led, char *action);
void Tux_Leds_Blink(char *led, char *count, char *delay);
void Tux_Leds_Pulse(char *led, char *min_intensity, char *max_intensity, char *count, char *period, char *effect, char *speed, char *step);
void Tux_Micro(char *action, char *file);
void Tux_Flippers(char *action, char *count);
void Tux_Rotate(char *action, char *count);
void Tux_Flash(char *sound, char *amplitude);
void Tux_Audio(char *action, char *rawcmd);
void Tux_State(tux_client client, char *sw_id);
void Tux_Sleep(void);
void Tux_Wakeup(void);
void Tux_Priority(tux_client client, char *action);
void Tux_Off(char *motor);
void Tux_Reset(void);
void Tux_Timestamp(tux_client  client);

void Tux_TTS(char **argv, int argc);

void ParseCommand(tux_client client, char *rawcmd);
void *syncMouthMovements(void *data);

#endif
