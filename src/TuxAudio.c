/* =============== GPL HEADER =====================
 * TuxAudio.c is part of TuxDroidServer
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

#include <TuxCompat.h>
#include <TuxAudio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <math.h>  /* lrint() */
#include <TuxTime.h>
#include <TuxLogger.h>
#include <TuxStrings.h>
#include <TuxUtils.h>
#include <TuxDownloader.h>
#include <TuxDriver.h>

int TuxAudio_micDeviceId=-1;   /* micro Tux */
int TuxAudio_musicDeviceId=-1; /* Audio Tux */
int TuxAudio_ttsDeviceId=-1;   /* TTS Tux */
int TuxAudio_isInitialized=0;

static char *TuxAudio_SoundCardName;
static char *TuxAudio_MicroName;

int isRecording=0;  /* are we recording right now ? */


/* Get Tux microphone device id */
TuxAudioError TuxAudio_getTuxMicroDeviceId()
{
	TuxLogger_Debug("TuxAudio_getTuxMicroDeviceId()",NULL);

	int id=-1;

	FMOD_SYSTEM *system;
	int numdrivers;

	FMOD_System_Create(&system);
	FMOD_System_Init(system, 1, FMOD_INIT_NORMAL, NULL);
	FMOD_System_GetRecordNumDrivers(system, &numdrivers);

	int i;
	for(i = 0; i < numdrivers; i++)
	{
		char name[256];
		FMOD_System_GetRecordDriverInfo(system,i,name,256,0);

		TuxLogger_Debug("Microphone found: %s (id=%d)",name,i);


		if(strstr(strtolower(name),"tuxdroid") != NULL || strstr(strtolower(name),"tux droid") != NULL)
		{
			TuxAudio_MicroName = duplicate_string(name);

			id=i;
			break;
		}
	}

	if(id >= 0)
	{
		TuxLogger_Debug("Microphone ID=%d",id);
		TuxAudio_micDeviceId=id;
	}
	else
	{
		TuxLogger_Error("Microphone not found :(",NULL);
		return E_TUXAUDIO_MICRO_NOTFOUND;
	}
	FMOD_System_Close(system);
	FMOD_System_Release(system);

	return E_TUXAUDIO_NOERROR;
}

/* Get soundcard id */
TuxAudioError TuxAudio_getTuxAudioDeviceId()
{
	TuxLogger_Debug("TuxAudio_getTuxAudioDeviceId()",NULL);

	int id=-1;
	FMOD_SYSTEM *system;
	int numdrivers;

	FMOD_System_Create(&system);
	FMOD_System_Init(system, 1, FMOD_INIT_NORMAL, NULL);
	FMOD_System_GetNumDrivers(system, &numdrivers);

	int i;
	for(i = 0; i < numdrivers; i++)
	{
		char name[256];
		FMOD_System_GetDriverInfo(system,i,name,256,0);

		TuxLogger_Debug("Soundcard found: %s (ID=%d)",name,i);

		if((strstr(strtolower(name),"tuxdroid") != NULL || strstr(strtolower(name),"tux droid") != NULL)
		&& strstr(strtolower(name),"tts") == NULL)
		{
			TuxAudio_SoundCardName = duplicate_string(name);

			id=i;
			break;
		}
	}

	if(id >= 0)
	{
		TuxLogger_Debug("Soundcard id=%d",id);
		TuxAudio_musicDeviceId=id;
		TuxAudio_ttsDeviceId=id;
	}
	else
	{
		TuxLogger_Error("Soundcard not found :(",NULL);
		return E_TUXAUDIO_SNDCARD_NOTFOUND;
	}
	FMOD_System_Close(system);
	FMOD_System_Release(system);

	return E_TUXAUDIO_NOERROR;
}

/**
 Get microphone name
 @return microphone name or empty string if not found
*/
char *TuxAudio_getMicroName(void)
{
	if(!TuxAudio_isInitialized)
		return "";

	if(TuxAudio_MicroName != NULL)
		return TuxAudio_MicroName;

	return "";
}

/**
 Get soundcard name
 @return soundcard name or empty string if not found
*/
char *TuxAudio_getSoundCardName(void)
{
	if(!TuxAudio_isInitialized)
		return "";

	if(TuxAudio_SoundCardName != NULL)
		return TuxAudio_SoundCardName;

	return "";
}

/* stop recording */
TuxAudioError TuxAudio_StopRecord()
{
	if(!TuxAudio_isInitialized)
		return E_TUXAUDIO_NOTINITIALIZED;

	TuxLogger_Debug("Stopping microphone record",NULL);
	isRecording=0;

	return E_TUXAUDIO_NOERROR;
}

/* Writing header in WAV file */
void WriteWavHeader(FILE *fp, FMOD_SOUND *sound, int length)
{
	isRecording=1;

	int channels, bits;
	float rate;

	if (!sound)
	{
		return;
	}

	fseek(fp, 0, SEEK_SET);

	FMOD_Sound_GetFormat  (sound, 0, 0, &channels, &bits);
	FMOD_Sound_GetDefaults(sound, &rate, 0, 0, 0);

	{
		#if defined(WIN32) || defined(__WATCOMC__) || defined(_WIN32) || defined(__WIN32__)
			#pragma pack(1)
			#define __PACKED  /* dummy */
		#else
			#define __PACKED __attribute__((packed))  /* gcc packed */
		#endif


		/*
			WAV Structures
		*/
		typedef struct
		{
			signed char id[4];
			int size;
		} RiffChunk;

		struct
		{
			RiffChunk       chunk           __PACKED;
			unsigned short  wFormatTag      __PACKED;  /* format type */
			unsigned short  nChannels       __PACKED;  /* number of channels (i.e. mono, stereo...)  */
			unsigned int    nSamplesPerSec  __PACKED;  /* sample rate */
			unsigned int    nAvgBytesPerSec __PACKED;  /* for buffer estimation  */
			unsigned short  nBlockAlign     __PACKED;  /* block size of data */
			unsigned short  wBitsPerSample  __PACKED;  /* number of bits per sample of mono data */
	#ifdef _WIN32
		} FmtChunk = { {{'f','m','t',' '}, sizeof(FmtChunk) - sizeof(RiffChunk) }, 1, channels, (int)rate, (int)rate * channels * bits / 8, 1 * channels * bits / 8, bits } __PACKED;
	#else
		} __PACKED FmtChunk = { {{'f','m','t',' '}, sizeof(FmtChunk) - sizeof(RiffChunk) }, 1, channels, (int)rate, (int)rate * channels * bits / 8, 1 * channels * bits / 8, bits };
	#endif


		struct
		{
			RiffChunk chunk;
		} DataChunk = { {{'d','a','t','a'}, length } };

		struct
		{
			RiffChunk chunk;
			signed char rifftype[4];
		} WavHeader = { {{'R','I','F','F'}, sizeof(FmtChunk) + sizeof(RiffChunk) + length }, {'W','A','V','E'} };


		/*
			Write out the WAV header.
		*/
		fwrite(&WavHeader, sizeof(WavHeader), 1, fp);
		fwrite(&FmtChunk, sizeof(FmtChunk), 1, fp);
		fwrite(&DataChunk, sizeof(DataChunk), 1, fp);
	}
}

/* TODO: Errors handling */
void *TuxAudio_StartRecord(void *data)
{
	TuxLogger_Debug("TuxAudio_StartRecord()",NULL);

	/* system => TuxAudio_micsystem */
	/* sound => TuxAudio_micsound */
	/* result => TuxAudio_micresult */

	char *file = (char *)data;

	if(file_exists(file))
		remove(file);

	FILE *fp;
	unsigned int datalength = 0, soundlength;

	int TuxMicroId = TuxAudio_getTuxMicroDeviceId();

	TuxAudio_micresult = FMOD_System_Create(&TuxAudio_micsystem);

	#ifdef _WIN32
	TuxAudio_micresult = FMOD_System_SetOutput(TuxAudio_micsystem, FMOD_OUTPUTTYPE_WINMM);
	#else
	TuxAudio_micresult = FMOD_System_SetOutput(TuxAudio_micsystem, FMOD_OUTPUTTYPE_ALSA);
	#endif

	TuxAudio_micresult = FMOD_System_Init(TuxAudio_micsystem, 32, FMOD_INIT_NORMAL, 0);

	memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));

	exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
	exinfo.numchannels = 2;
	exinfo.format = FMOD_SOUND_FORMAT_PCM16;
	exinfo.defaultfrequency = 44100;
	exinfo.length = exinfo.defaultfrequency * sizeof(short) * exinfo.numchannels * 2;

	TuxAudio_micresult = FMOD_System_CreateSound(TuxAudio_micsystem, 0, FMOD_2D | FMOD_SOFTWARE | FMOD_OPENUSER, &exinfo, &TuxAudio_micsound);
	TuxAudio_micresult = FMOD_System_RecordStart(TuxAudio_micsystem, TuxMicroId, TuxAudio_micsound, 1);

	fp = fopen(file, "wb");

	/* WriteWavHeader(fp, sound, datalength); */

	WriteWavHeader(fp,TuxAudio_micsound,datalength);

	TuxAudio_micresult = FMOD_Sound_GetLength(TuxAudio_micsound, &soundlength, FMOD_TIMEUNIT_PCM);

	do
	{
		static unsigned int lastrecordpos = 0;
		unsigned int recordpos = 0;
		FMOD_System_GetRecordPosition(TuxAudio_micsystem, TuxMicroId, &recordpos);

		if (recordpos != lastrecordpos)
		{
			void *ptr1, *ptr2;
			int blocklength;
			unsigned int len1, len2;

			blocklength = (int)recordpos - (int)lastrecordpos;
			if (blocklength < 0)
			{
				blocklength += soundlength;
			}

			/*
				Lock the sound to get access to the raw data.
			*/
			FMOD_Sound_Lock(TuxAudio_micsound, lastrecordpos * exinfo.numchannels * 2, blocklength * exinfo.numchannels * 2, &ptr1, &ptr2, &len1, &len2);	/* * exinfo.numchannels * 2 = stereo 16bit.	 1 sample = 4 bytes. */

			/*
				Write it to disk.
			*/
			if (ptr1 && len1)
			{
				datalength += fwrite(ptr1, 1, len1, fp);
			}
			if (ptr2 && len2)
			{
				datalength += fwrite(ptr2, 1, len2, fp);
			}

			/*
				Unlock the sound to allow FMOD to use it again.
			*/
			FMOD_Sound_Unlock(TuxAudio_micsound, ptr1, ptr2, len1, len2);

			#ifdef _WIN32
			sleep(10);  /* 10 ms */
			#else
			TuxSleep(10);  /* 10 ms */
			#endif

		}

		lastrecordpos = recordpos;

		FMOD_System_Update(TuxAudio_micsystem);
	}
	while(isRecording==1);

	WriteWavHeader(fp,TuxAudio_micsound,datalength);

	fclose(fp);

	TuxAudio_micresult = FMOD_Sound_Release(TuxAudio_micsound);
	TuxAudio_micresult = FMOD_System_Release(TuxAudio_micsystem);

	if(file != NULL)
	{
		/*free(file);
		file=NULL;*/
	}

	if(fp != NULL)
	{
		/*free(fp);
		fp=NULL;*/
	}

	return 0;
}

/* Stop wav playing */
TuxAudioError TuxAudio_StopMusic()
{
	TuxLogger_Debug("TuxAudio_StopMusic()",NULL);

	if(!TuxAudio_isInitialized)
		return E_TUXAUDIO_NOTINITIALIZED;

	TuxAudio_musicresult = FMOD_Sound_Release(TuxAudio_musicsound);
	if(TuxAudio_musicresult != FMOD_OK)
			return E_TUXAUDIO_FMOD_SNDRELEASE;

	TuxAudio_musicresult = FMOD_System_Close(TuxAudio_musicsystem);
	if(TuxAudio_musicresult != FMOD_OK)
		return E_TUXAUDIO_FMOD_SNDCLOSE;

	TuxAudio_musicresult = FMOD_System_Release(TuxAudio_musicsystem);
	if(TuxAudio_musicresult != FMOD_OK)
			return E_TUXAUDIO_FMOD_SYSRELEASE;

	return E_TUXAUDIO_NOERROR;
}

/* Stop TTS playing */
TuxAudioError TuxAudio_StopTTS()
{
	TuxLogger_Debug("TuxAudio_StopTTS",NULL);

	if(!TuxAudio_isInitialized)
		return E_TUXAUDIO_NOTINITIALIZED;

	TuxAudio_ttsresult = FMOD_Sound_Release(TuxAudio_ttssound);
	if(TuxAudio_ttsresult != FMOD_OK)
			return E_TUXAUDIO_FMOD_SNDRELEASE;

	TuxAudio_ttsresult = FMOD_System_Close(TuxAudio_ttssystem);
	if(TuxAudio_ttsresult != FMOD_OK)
		return E_TUXAUDIO_FMOD_SNDCLOSE;

	TuxAudio_ttsresult = FMOD_System_Release(TuxAudio_ttssystem);
	if(TuxAudio_ttsresult != FMOD_OK)
				return E_TUXAUDIO_FMOD_SYSRELEASE;

	Tux_Close_Mouth();

	if(file_exists("tts.wav"))
		remove("tts.wav");

	return E_TUXAUDIO_NOERROR;
}

/* Calculate TTS time needed */
long TuxAudio_ComputeTTSTime(const char *str, int pitch, int speed)
{
	return lrint(((float)(countCharacterOccurency(trim((char *)str),' ') + 1) / (float)speed) * 60 * 1000);
}

void TuxAudio_AutoStopTTS(long time)
{
	pthread_t Thread;
	pthread_attr_t Thread_attr;
	pthread_attr_init (&Thread_attr);
	pthread_attr_setdetachstate (&Thread_attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&Thread, &Thread_attr, StopTTSThread, (void *)time);
}

void *StopTTSThread(void *data)
{
	#ifdef _WIN32
	Sleep((long)data);
	#else
	TuxSleep((long)data);
	#endif

	TuxAudio_StopTTS();
	return 0;
}

/* Start playing TTS */
/* TODO: Voice choise */
TuxAudioError TuxAudio_PlayTTS(const char *voice, const char *text, int pitch, int speed, bool autoStop)
{
	TuxLogger_Debug("TuxAudio_PlayTTS(), text=%s / pitch=%d / speed=%d",text,pitch,speed);


	/* speed: (80 to 450) (default is 175) */
	/* volume: (0 to 200) (default is 100) */

	if(!TuxAudio_isInitialized)
		return E_TUXAUDIO_NOTINITIALIZED;

	char *cmd = (char *)malloc(sizeof(char)*strlen(text)+100);
	#ifdef _WIN32
	sprintf(cmd,"espeak.exe --path=./ -v%s -s %d -p %d -a 100 -w tts.wav \"%s\"",voice,speed,pitch,text);
	#else
	sprintf(cmd,"./espeak --path=./ -v%s -s %d -p %d -a 100 -w tts.wav \"%s\"",voice,speed,pitch,text);
	#endif

	TuxLogger_Debug("TTS CMD ----> %s",cmd);

	system(cmd);

	if(cmd != NULL)
	{
		/*free(cmd);
		cmd=NULL;*/
	}

	FMOD_CHANNELGROUP* channelgroup = NULL;

	TuxAudio_ttsresult = FMOD_System_Create(&TuxAudio_ttssystem);
	if(TuxAudio_ttsresult != FMOD_OK)
		return E_TUXAUDIO_FMOD_CREATESYS;

	TuxAudio_ttsresult = FMOD_System_SetDriver(TuxAudio_ttssystem,TuxAudio_ttsDeviceId);
	if(TuxAudio_ttsresult != FMOD_OK)
		return E_TUXAUDIO_FMOD_SETDRIVER;

	TuxAudio_ttsresult = FMOD_System_Init(TuxAudio_ttssystem, 1, FMOD_INIT_NORMAL, NULL);
	if(TuxAudio_ttsresult != FMOD_OK)
		return E_TUXAUDIO_FMOD_INIT;

	TuxAudio_musicresult = FMOD_System_CreateSound(TuxAudio_ttssystem, "tts.wav", FMOD_CREATESAMPLE, 0, &TuxAudio_ttssound);
	if(TuxAudio_ttsresult != FMOD_OK)
		return E_TUXAUDIO_FMOD_CREATESOUND;

		TuxAudio_musicresult = FMOD_System_CreateChannelGroup(TuxAudio_ttssystem, "TTSChannel", &channelgroup);
	if(TuxAudio_ttsresult != FMOD_OK)
		return E_TUXAUDIO_FMOD_CREATECHAN;

		TuxAudio_musicresult = FMOD_ChannelGroup_SetVolume(channelgroup, 1.0); /* 100% */
	if(TuxAudio_ttsresult != FMOD_OK)
		return E_TUXAUDIO_FMOD_SETVOLUMEERROR;

	TuxAudio_musicresult = FMOD_System_PlaySound(TuxAudio_ttssystem, FMOD_CHANNEL_FREE, TuxAudio_ttssound, 0, NULL);
	if(TuxAudio_ttsresult != FMOD_OK)
		return E_TUXAUDIO_FMOD_PLAYERROR;


	if(autoStop)
		TuxAudio_AutoStopTTS(TuxAudio_ComputeTTSTime(text,pitch,speed));

	return E_TUXAUDIO_NOERROR;
}

/*
 Start playing sound file
*/
TuxAudioError TuxAudio_PlayMusic(const char *url)
{
	TuxLogger_Debug("TuxAudio_PlayMusic",NULL);

	#ifndef _WIN32
	if(url[0] == '/')
	{
		/* Linux path */
		if(!file_exists(url))
		{
			TuxLogger_Error("The sound file does not exist !",NULL);

			return E_TUXAUDIO_FILE_NOTFOUND;
		}
	}
	#else
	if((url[0] == 'c' || url[0] == 'C'
		|| url[0] == 'd' || url[0] == 'D'
		|| url[0] == 'e' || url[0] == 'E'
		|| url[0] == 'f' || url[0] == 'F'
		|| url[0] == 'g' || url[0] == 'G')
		&& url[1] == '\\')
	{
			/* Windows path */

			if(!file_exists(url))
			{
				TuxLogger_Error("The sound file does not exist !",NULL);

				return E_TUXAUDIO_NOTINITIALIZED;
			}
	}
	#endif


	if(!TuxAudio_isInitialized)
		return E_TUXAUDIO_NOTINITIALIZED;

	FMOD_CHANNELGROUP* channelgroup = NULL;

	TuxAudio_musicresult = FMOD_System_Create(&TuxAudio_musicsystem);
	if(TuxAudio_musicresult != FMOD_OK)
		return E_TUXAUDIO_FMOD_CREATESYS;

	TuxAudio_musicresult = FMOD_System_SetDriver(TuxAudio_musicsystem,TuxAudio_musicDeviceId);
	if(TuxAudio_musicresult != FMOD_OK)
		return E_TUXAUDIO_FMOD_SETDRIVER;

	TuxAudio_musicresult = FMOD_System_Init(TuxAudio_musicsystem, 1, FMOD_INIT_NORMAL, NULL);
	if(TuxAudio_musicresult != FMOD_OK)
		return E_TUXAUDIO_FMOD_INIT;

	TuxAudio_musicresult = FMOD_System_CreateStream(TuxAudio_musicsystem, url, FMOD_CREATESTREAM, 0, &TuxAudio_musicsound);
	if(TuxAudio_musicresult != FMOD_OK)
		return E_TUXAUDIO_FMOD_CREATESOUND;

		TuxAudio_musicresult = FMOD_System_CreateChannelGroup(TuxAudio_musicsystem, "MUSICChannel", &channelgroup);
	if(TuxAudio_musicresult != FMOD_OK)
		return E_TUXAUDIO_FMOD_CREATECHAN;

		TuxAudio_musicresult = FMOD_ChannelGroup_SetVolume(channelgroup, 0.5);  /* 50% */
	if(TuxAudio_musicresult != FMOD_OK)
		return E_TUXAUDIO_FMOD_SETVOLUMEERROR;


	unsigned int time;
	FMOD_Sound_GetLength(TuxAudio_musicsound, &time, FMOD_TIMEUNIT_MS);  /* Time in milliseconds */

	if(time > 0)  /* -1 = streaming */
	{
		/* TODO: Auto stop */
		pthread_t Thread;
		pthread_attr_t Thread_attr;
		pthread_attr_init (&Thread_attr);
		pthread_attr_setdetachstate (&Thread_attr, PTHREAD_CREATE_DETACHED);
		pthread_create(&Thread, &Thread_attr, StopMusicThread, (void *)&time);
	}

	TuxAudio_musicresult = FMOD_System_PlaySound(TuxAudio_musicsystem, FMOD_CHANNEL_FREE, TuxAudio_musicsound, 0, NULL);
	if(TuxAudio_musicresult != FMOD_OK)
		return E_TUXAUDIO_FMOD_PLAYERROR;

	return E_TUXAUDIO_NOERROR;
}

void *StopMusicThread(void *data)
{
	unsigned int milliseconds = *((unsigned int *)data);
	#ifdef _WIN32
	Sleep(milliseconds);
	#else
	TuxSleep(milliseconds);
	#endif

	TuxAudio_StopMusic();
	return 0;
}


/* Initialize devices */
TuxAudioError TuxAudio_Initialize()
{
	if(TuxAudio_isInitialized)
		return E_TUXAUDIO_ALREADYINITIALIZED;

	TuxLogger_Debug("Initializing TuxAudio..",NULL);

	if(TuxAudio_getTuxAudioDeviceId() != E_TUXAUDIO_NOERROR)
		return E_TUXAUDIO_SNDCARD_NOTFOUND;

	if(TuxAudio_getTuxMicroDeviceId() != E_TUXAUDIO_NOERROR)
		return E_TUXAUDIO_MICRO_NOTFOUND;

	TuxAudio_isInitialized=true;

	TuxLogger_Info("TuxAudio v%d.%d.%d initialized !",TUXAUDIO_MAJOR,TUXAUDIO_MINOR,TUXAUDIO_REVIS);

	return E_TUXAUDIO_NOERROR;
}
