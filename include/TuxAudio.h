/* =============== GPL HEADER =====================
 * TuxAudio.h is part of TuxDroidServer
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


#ifndef __TUXAUDIO_H__
#define __TUXAUDIO_H__

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <TuxVersion.h>

#ifdef _WIN32
	#include <windows.h> /* _stdcall() (necessary for FMOD) */
#endif

#include <fmod/fmod.h>
#include <fmod/fmod_errors.h>

#define TUXAUDIO_AUTHOR "Joel Matteotti" /* Initial developer (can't change !) */

#define TUXAUDIO_MAINTENER "Joel Matteotti" /* Current developer (if I let this happen :P) */


typedef int TuxAudioError;
typedef enum
{
	E_TUXAUDIO_NOERROR=0, /* OK */
	E_TUXAUDIO_NOTINITIALIZED,
	E_TUXAUDIO_ALREADYINITIALIZED,
	E_TUXAUDIO_SNDCARD_NOTFOUND,
	E_TUXAUDIO_MICRO_NOTFOUND,
	E_TUXAUDIO_FILE_NOTFOUND,
	E_TUXAUDIO_FMOD_CREATESYS,
	E_TUXAUDIO_FMOD_SETDRIVER,
	E_TUXAUDIO_FMOD_INIT,
	E_TUXAUDIO_FMOD_CREATECHAN,
	E_TUXAUDIO_FMOD_SETVOLUMEERROR,
	E_TUXAUDIO_FMOD_CREATESOUND,
	E_TUXAUDIO_FMOD_PLAYERROR,
	E_TUXAUDIO_FMOD_SNDRELEASE,
	E_TUXAUDIO_FMOD_SNDCLOSE,
	E_TUXAUDIO_FMOD_SYSRELEASE,
	E_TUXAUDIO_FMOD_RECORDERROR,
} tux_audio_error_t;

FMOD_SYSTEM *TuxAudio_musicsystem;
FMOD_SYSTEM *TuxAudio_ttssystem;
FMOD_SYSTEM *TuxAudio_micsystem;

FMOD_SOUND *TuxAudio_musicsound;
FMOD_SOUND *TuxAudio_ttssound;
FMOD_SOUND *TuxAudio_micsound;

FMOD_RESULT TuxAudio_musicresult;
FMOD_RESULT TuxAudio_ttsresult;
FMOD_RESULT TuxAudio_micresult;

FMOD_CREATESOUNDEXINFO exinfo; /* for microphone */

/* prototypes */
TuxAudioError TuxAudio_getTuxMicroDeviceId(void);
TuxAudioError TuxAudio_getTuxAudioDeviceId(void);
void WriteWavHeader(FILE *fp, FMOD_SOUND *sound, int length);
TuxAudioError TuxAudio_StopRecord(void);
void *TuxAudio_StartRecord(void *data);
TuxAudioError TuxAudio_StopMusic(void);
TuxAudioError TuxAudio_StopTTS(void);
void *StopTTSThread(void *data);
void TuxAudio_AutoStopTTS(long time);
long TuxAudio_ComputeTTSTime(const char *str, int pitch, int speed);
TuxAudioError TuxAudio_PlayTTS(const char *voice, const char *text, int pitch, int speed, bool autoStop);
TuxAudioError TuxAudio_PlayMusic(const char *url);
void *StopMusicThread(void *data);
TuxAudioError TuxAudio_Initialize(void);

char *TuxAudio_getSoundCardName(void);
char *TuxAudio_getMicroName(void);

#endif
