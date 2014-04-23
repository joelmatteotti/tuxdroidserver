/* =============== GPL HEADER =====================
 * TuxCommands.c is part of TuxDroidServer
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
#include <stdlib.h>
#include <pthread.h>

#include <TuxCompat.h>
#include <TuxCommands.h>
#include <TuxLogger.h>
#include <TuxTime.h>
#include <TuxAttitune.h>
#include <TuxDriver.h>
#include <TuxAudio.h>
#include <TuxServer.h>
#include <TuxStrings.h>
#include <TuxConfig.h>
#include <TuxMessages.h>
#include <TuxTime.h>
#include <TuxTypes.h>
#include <TuxUtils.h>
#include <TuxASR.h>

extern tuxconfig config;

int isLocked=0;  /* Is an app prioritary on the others ? */

char *PriorityID;  /* Application id to avoid locking its commands */

#define READSIZE 1024

void* convertToFlac(void *data)
{
	#ifdef _WIN32
	Sleep(200);
	#else
	TuxSleep(200);
	#endif

	flac_convert convert =(flac_convert)data;

	if(convertWavToFlac(convert->file))
		SendMsgToClient(convert->client,TDSM_FLACCONVERT_FINISHED);
	else
		SendMsgToClient(convert->client,TDSM_FLACCONVERT_FAILED);

	return 0;
}

/*
  DEFINED COMMANDS FOR TUXSERVER:

  Tux_Open()
  Tux_Close()
  Tux_OpenClose()
  Tux_Flippers()
  Tux_Rotate()

  Tux_TTS()
  Tux_Flash()

  Tux_State()

  Tux_Micro()
  Tux_Audio()

  Tux_PlayAtt()

  ....

*/





/* Convert a file to FLAC format */
void Tux_Flac(tux_client client, char *file)
{
	TuxLogger_Debug("Tux_Flac()",NULL);
	
	TuxLogger_Debug("Allocating memory to convert..",NULL);
	flac_convert convert = (flac_convert)malloc(sizeof(flac_convert_t));
	TuxLogger_Debug("Allocating done",NULL);
	
	TuxLogger_Debug("Copying from client to convert->client",NULL);
	convert->client = client;

	TuxLogger_Debug("Copying from file to convert->file",NULL);
	convert->file = file;

	TuxLogger_Debug("Preparing thread..",NULL);
	//thread
	pthread_t FlacConverterThread;
	pthread_attr_t FlacConverterThread_attr;
	pthread_attr_init(&FlacConverterThread_attr);
	pthread_attr_setdetachstate(&FlacConverterThread_attr, PTHREAD_CREATE_DETACHED);
	TuxLogger_Debug("The thread is ready !",NULL);
	TuxLogger_Debug("Starting thread convertToFlac..",NULL);
	pthread_create(&FlacConverterThread,&FlacConverterThread_attr,convertToFlac,convert);
	TuxLogger_Debug("Thread started",NULL);
}

void* ASRProcessing(void *data)
{
	TuxLogger_Debug("ASRProcessing() thread",NULL);
	
	TuxLogger_Debug("Getting asrcontent",NULL);
	flac_convert asrcontent = (flac_convert)data;
	
	TuxLogger_Debug("Sending file to Google",NULL);
	while(TuxASR_uploadSpeech(asrcontent->file) != 1){ } /* Waiting for upload */
	
	TuxLogger_Debug("File sent",NULL);
	
	TuxLogger_Debug("Getting ASR",NULL);
	char *str = TuxASR_getTextFromSpeech(replace(asrcontent->file,".flac",""));
	


	TuxLogger_Debug("ASR => %s",str);
	
	//TDSM_ASR_RESULT
	
	TuxLogger_Debug("Allocating memory for ASR result message..",NULL);
	
	char *msg = NULL;
	
	if(str != NULL)
		msg = (char *)malloc(sizeof(char)*strlen(TDSM_ASRRESULT)+strlen(str));
	else
		msg = (char *)malloc(sizeof(char)*strlen(TDSM_ASRRESULT));
	
	TuxLogger_Debug("Allocating done",NULL);
	
	TuxLogger_Debug("Copying message",NULL);
	
	if(str != NULL)
		sprintf(msg,TDSM_ASRRESULT,str);
	else
		sprintf(msg,TDSM_ASRRESULT,"NULL");
		
	TuxLogger_Debug("Sending message",NULL);
	SendMsgToClient(asrcontent->client,msg);
	
	TuxLogger_Debug("Deleting %s..",asrcontent->file);
	if(file_exists(asrcontent->file))
		remove(asrcontent->file); /* xxxx.flac */
	
	
	TuxLogger_Debug("File deleted",NULL);
	
	return 0;
}

void Tux_ASR(tux_client client, char *file)
{
	TuxLogger_Debug("Tux_ASR()",NULL);

	if(!config->ASR_ENABLED)
	{
		TuxLogger_Debug("ASR inaccessible donc désactiver !",NULL);
		return;
	}

	TuxLogger_Debug("Allocation memory for asrcontent..",NULL);
	flac_convert asrcontent = (flac_convert)malloc(sizeof(flac_convert_t));
	TuxLogger_Debug("Allocating done",NULL);
	TuxLogger_Debug("Copying from client to asrcontent->client",NULL);
	asrcontent->client = client;
	TuxLogger_Debug("Copying from file to asrcontent->file",NULL);
	asrcontent->file = file;
	

	TuxLogger_Debug("Preparing thread..",NULL);
	//thread
	pthread_t ASRProcessingThread;
	pthread_attr_t ASRProcessingThread_attr;
	pthread_attr_init(&ASRProcessingThread_attr);
	pthread_attr_setdetachstate(&ASRProcessingThread_attr, PTHREAD_CREATE_DETACHED);
	TuxLogger_Debug("The thread is ready",NULL);
	TuxLogger_Debug("Starting thread..",NULL);
	pthread_create(&ASRProcessingThread,&ASRProcessingThread_attr,ASRProcessing,asrcontent);
	TuxLogger_Debug("Thread started",NULL);
	
}

void Tux_Open(char *motor)
{
	if(!strcmp(strtolower(motor),"eyes"))
		Tux_Open_Eyes();
	else if(!strcmp(strtolower(motor),"mouth"))
		Tux_Open_Mouth();
	else
		TuxLogger_Warning("Incorrect parameter (%s) for Tux_Open() !",motor);
}

void Tux_Close(char *motor)
{
	if(!strcmp(strtolower(motor),"eyes"))
		Tux_Close_Eyes();
	else if(!strcmp(strtolower(motor),"mouth"))
		Tux_Close_Mouth();
	else
		TuxLogger_Warning("Incorrect parameter (%s) for Tux_Close() !",motor);
}

void Tux_OpenClose(char *motor, int count)
{
	if(!strcmp(strtolower(motor),"eyes"))
		Tux_OpenClose_Eyes(count);
	else if(!strcmp(strtolower(motor),"mouth"))
		Tux_OpenClose_Mouth(count);
	else
		TuxLogger_Warning("Incorrect parameter (%s) for Tux_OpenClose() !",motor);
}

void Tux_Leds_OnOff(char *led, char *action)
{
		leds_t LED = getLedsFromString(led);

		if(LED == 0)
			return;

		if(!strcmp(strtolower(action),"on"))
			Tux_Led_On(LED);
		if(!strcmp(strtolower(action),"off"))
			Tux_Led_Off(LED);
}

void Tux_Leds_Blink(char *led, char *count, char *delay)
{
	if(!atoi(count) || !atof(delay))
		return;

	leds_t LED = getLedsFromString(led);

	Tux_BlinkLeds(LED, atoi(count), atof(delay));
}

void Tux_Leds_Pulse(char *led, char *min_intensity, char *max_intensity, char *count, char *period, char *effect, char *speed, char *step)
{
	leds_t LED = getLedsFromString(led);
	effect_type_t EFFECT = getEffectFromString(effect);

	if(LED == LED_NONE)
		return;

	if(!atof(min_intensity) || !atof(max_intensity) || !atoi(count) || !atof(period)
		|| !atof(speed) || !atoi(step))
				return;

	Tux_PulseLeds(LED,atof(min_intensity), atof(max_intensity), (byte)atoi(count),atof(period),EFFECT, atof(speed), (byte)atoi(step));
}

void Tux_Micro(char *action, char *file)
{
	if(!strcmp(strtolower(action),"record"))
	{
		pthread_t recordThread;
		pthread_attr_t recordThread_attr;
		pthread_attr_init (&recordThread_attr);
		pthread_attr_setdetachstate (&recordThread_attr, PTHREAD_CREATE_DETACHED);
		pthread_create(&recordThread, &recordThread_attr, TuxAudio_StartRecord, file);
		pthread_join(recordThread, NULL);
	}
	if(!strcmp(strtolower(action),"stop"))
		TuxAudio_StopRecord();
}

void Tux_Flippers(char *action, char *count)
{
	if(!strcmp(strtolower(action),"up"))
		Tux_Flippers_Up();
	if(!strcmp(strtolower(action),"down"))
		Tux_Flippers_Down();
	if(!strcmp(strtolower(action),"updown"))
	{
		if(!atoi(count))
			return;

		Tux_FlippersUpDown(atoi(count));
	}

	if(!strcmp(strtolower(action),"speed"))
	{
		if(!atoi(count))
			return;

		Tux_SetFlippersSpeed((byte)atoi(count));
	}
}

void Tux_Rotate(char *action, char *count)
{
	if(!atoi(count))
		return;

	if(!strcmp(strtolower(action),"left"))
		Tux_RotateLeft(atoi(count));
	if(!strcmp(strtolower(action),"right"))
		Tux_RotateRight(atoi(count));
	if(!strcmp(strtolower(action),"speed"))
		Tux_SetRotationSpeed((byte)atoi(count));
}

void Tux_Flash(char *sound, char *amplitude)
{
	if(!atoi(sound) || !atoi(amplitude))
		return;

	int snd = atoi(sound);
	int ampli = atoi(amplitude);

	if(snd < 1)
		snd = 1;
	if(snd > 17)
		snd = 17;

	if(ampli < 0)
		ampli=0;
	if(ampli > 100)
		ampli=100;

	Tux_PlayFlashSound(snd,ampli);
}

void Tux_Audio(char *action, char *rawcmd)
{
	if(!strcmp(action,"stopmusic"))
		TuxAudio_StopMusic();

	if(!strcmp(action,"playmusic"))
	{
		char *mystring = rawcmd;
		mystring = replace(mystring,"tux_audio(playmusic,","");
		char **sp = explode(mystring,'.');
		mystring = replace(mystring,sp[1],replace(sp[1],")",""));

		if(file_exists(mystring))
			TuxAudio_PlayMusic(mystring);
	}
}

void Tux_State(tux_client client, char *sw_id)
{
	char valeur[256];

	int sn = getStateNumber(sw_id);

	if(sn > -1)
		TuxDrv_GetStatusValue(sn,valeur);
	else
		TuxDrv_GetStatusValue(atoi(sw_id),valeur);

	SendMsgToClient(client,valeur);
}

void Tux_Sleep(void)
{
	char cmd[32];
	snprintf(cmd,32,"RAW_CMD:0x00:0xB7:0x01:0x00:0x00");
	TuxDrv_PerformCommand(0.0,cmd);
}

void Tux_Wakeup(void)
{
	char cmd[32];
	snprintf(cmd,32,"RAW_CMD:0x00:0xB6:0xFF:0x01:0x00");
	TuxDrv_PerformCommand(0.0,cmd);
}

void Tux_Priority(tux_client client, char *action)
{
	/* asking for priority */
	if(!strcmp(strtolower(action),"lock"))
	{
		/* if the priority has not already been given */
		if(isLocked == 0)
		{
			/* check if the app has an id */
			if(client->pID == NULL)
			{
				TuxLogger_Error("Application begging for priority has not identifier !",NULL);
				SendMsgToClient(client,TDSM_PRIORITY_NOIDSET);
				return;
			}
			else
			{
				isLocked=1;
				PriorityID = duplicate_string(client->pID);
				SendMsgToAll(TDSM_PRIORITY_LOCKED,client);
			}
		}
		else
		{
			TuxLogger_Warning("Priority has already been given !",NULL);
			return;
		}
	}
	else if(!strcmp(strtolower(action),"unlock")) /* asking for removing priority */
	{
		/* check priority has already been given */
		if(isLocked == 1)
		{
			if(client->pID != NULL)
			{
				if(!strcmp(client->pID,PriorityID))
				{
					isLocked=0;
					PriorityID = duplicate_string("");
					SendMsgToAll(TDSM_PRIORITY_UNLOCKED,client);
				}
				else
				{
					TuxLogger_Warning("Application with different identifier tried to remove priority !",NULL);
					return;
				}
			}
			else
			{
				TuxLogger_Warning("Application without identifier tried to remove priority !",NULL);
				return;
			}
		}
		else
		{
			TuxLogger_Warning("No priority set for the moment",NULL);
			return;
		}
	}
	else
	{
		TuxLogger_Warning("Incorrect parameter (%s) for Tux_Priority() !",NULL);
		return;
	}
}

void Tux_Off(char *motor)
{
	if(!strcmp(strtolower(motor),"mouth"))
		TuxDrv_Mouth_Off();
	else if(!strcmp(strtolower(motor),"spin"))
		TuxDrv_Spinning_Off();
	else if(!strcmp(strtolower(motor),"flippers"))
		TuxDrv_Flippers_Off();
	else if(!strcmp(strtolower(motor),"eyes"))
		TuxDrv_Eyes_Off();
	else
		TuxLogger_Warning("Incorrect parameter (%s) for Tux_Off() !",motor);
}

void Tux_Reset(void)
{
	TuxDrv_Mouth_Off();
	TuxDrv_Spinning_Off();
	TuxDrv_Flippers_Off();
	TuxDrv_Eyes_Off();

	#ifdef _WIN32
	Sleep(2000);
	#else
	TuxSleep(2000);
	#endif

	TuxDrv_ResetPositions();
}

void Tux_Timestamp(tux_client client)
{
	char *msg = (char *)malloc(sizeof(char)*15);
	sprintf(msg,"%lu",Tux_TimeStamp());

	SendMsgToClient(client,msg);

	if(msg != NULL)
	{
		/*free(msg);
		msg=NULL;*/
	}
}

void Tux_TTS(char **argv, int argc)
{
	TuxLogger_Debug("Entering function Tux_TTS..",NULL);

	char args[4096] = "";
	int i;

	TuxLogger_Debug("Before for loop",NULL);

	for(i = 0; i < argc; i++)
	{
		strcat(args,argv[i]);
		strcat(args,",");
	}

	TuxLogger_Debug("After for loop",NULL);

	char **sp = explode(args,'"');

	if(sp == NULL)
		return;

	if(sp[0] == NULL)
		return;

	char *str = duplicate_string(sp[1]);

	//sp[2] => params

	char *options = duplicate_string(sp[2]);

	freeExplode(sp);

	sp = explode(options,',');

	free(options);
	options = NULL;

	if(sp == NULL)
		return;

	//sp[1] => voice
	//sp[2] => pitch
	//sp[3] => speed
	//sp[4] => auto stop (yes/no)
	//sp[5] => mouth sync (yes/no)

	if(sp[1] == NULL || sp[2] == NULL || sp[3] == NULL || sp[4] == NULL || sp[5] == NULL)
		return;

	bool syncMouth=true;
	bool autoStop=true;

	int pitch = atoi(sp[2]);
	int speed = atoi(sp[3]);

	if(!atoi(sp[4]))
		autoStop=false;

	if(!atoi(sp[5]))
		syncMouth=false;

	if(pitch < 0)
		pitch = 0;
	if(pitch > 99)
		pitch = 99;

	if(speed < 80)
		speed=80;
	if(speed > 450)
		speed=450;

	int word = countCharacterOccurency(trim(str),' ') + 1;

	/* mouth synchronized */
	if(syncMouth)
	{
		/* 4 characters = 1 movement (for normal speed) */

		syncMouthMov mov;
		mov = (syncMouthMov)malloc(sizeof(syncMouthMov_t));

		mov->phr = duplicate_string(str);

		mov->count = word;

		pthread_t syncMouthThread;
		pthread_attr_t syncMouthThread_attr;
		pthread_attr_init (&syncMouthThread_attr);
		pthread_attr_setdetachstate (&syncMouthThread_attr, PTHREAD_CREATE_DETACHED);
		pthread_create(&syncMouthThread, &syncMouthThread_attr, syncMouthMovements, mov);

		if(mov != NULL)
			free(mov->phr);
		free(mov);
		mov = NULL;
	}

	TuxAudio_PlayTTS(sp[1], str, pitch, speed, autoStop);

	freeExplode(sp);
	free(str);
}

/* parsing commands sent by the client */
void ParseCommand(tux_client client, char *rawcmd)
{
	TuxLogger_Debug("New command received from client %d",client->id);
	TuxLogger_Debug("Raw command ---> %s",rawcmd);

	if(!strchr(rawcmd,'(') && !strchr(rawcmd,')')) /* wrong formatted command (must be at least command()) */
		return;

	TuxLogger_Debug("Exploding params..",NULL);

	char *cmd;
	char **command_parts = explode(rawcmd,'(');

	TuxLogger_Debug("Params exploded",NULL);

	if(command_parts[0] != NULL)
	{
		cmd = duplicate_string(command_parts[0]);
	}
	else
	{
		freeExplode(command_parts);
		command_parts = NULL;
		return;
	}

	int argc=0;
	char **argv = NULL;

	if(!strcmp(command_parts[1],")"))
	{
		/* no params */
		TuxLogger_Debug("Command has no parameters !",NULL);
	}
	else
	{
		TuxLogger_Debug("Command has parameters",NULL);

		char *pch = strtok(command_parts[1],")");

		if(pch != NULL)
		{
			argv = explode(pch,',');
			argc = countCharacterOccurency(pch,',') + 1;
			if(argc > 1)
			{
				TuxLogger_Debug("Command has more than one parameters",NULL);
				int i;
				for(i = 0; i < argc; i++)
					TuxLogger_Debug("argv[%d] -----> %s",i,argv[i]);
			}
			else
			{
				TuxLogger_Debug("Command has only one parameter",NULL);
				TuxLogger_Debug("-----> %s",pch);
			}
		}
	}

	freeExplode(command_parts);
	command_parts = NULL;

	/* The command Tux_Key() must be the first check to avoid all errors due to priority demand */
	if(!strcmp(strtolower(cmd),"tux_key"))
	{
		if(argv == NULL)
		{
			TuxLogger_Warning("No parameters have been given for Tux_Key() command but one parameter was expected !",NULL);
			return;
		}

		TuxLogger_Debug("Defining key -> %s for the client %d",argv[0], client->id);

		client->uKey = (char *)realloc(client->uKey,strlen(argv[0])*sizeof(char));
		strcpy(client->uKey,argv[0]);
	}

	TuxLogger_Debug("Checking key is valid..",NULL);

	if(strcmp(client->uKey,config->USER_KEY))
	{
		TuxLogger_Debug("Invalid key ! Disconnecting client %d !",client->id);
		
		removeUniqueID(client->pID);
		
		if(client->sock)
		{
			//send messaage
			SendMsgToClient(client,TDSM_INVALIDKEY); //inform the client
			#ifdef _WIN32
			Sleep(200);
			#else
			TuxSleep(200);
			#endif
			close(client->sock);
		}

		return;
	}

	//key is after tux_key(xxxx)
	if(!strcmp(strtolower(cmd),"tux_key"))
	{
		SendMsgToClient(client,TDSM_VALIDEDKEY);
	}

	TuxLogger_Debug("Valid key given",NULL);


	/* Allow an application to identify itself, needed for priority system */
	if(!strcmp(strtolower(cmd),"tux_setid"))
	{
		if(argv == NULL)
		{
			TuxLogger_Warning("No parameters have been given for Tux_SetId() command but one parameter was expected !",NULL);
			return;
		}

		if(isRealyUniqueID(argv[0]))
		{
			addUniqueID(argv[0]);
			client->pID = duplicate_string(argv[0]);
		}
		else
		{
			SendMsgToClient(client,TDSM_PRIORITY_IDNOTUNIQUE); //ID Not Unique so close connection
			#ifdef _WIN32
			Sleep(200);
			#else
			TuxSleep(200);
			#endif
			close(client->sock);
			return;
		}
	}
	
	if(client->uKey != NULL && strcmp(strtolower(cmd),"tux_key"))
	{
		TuxLogger_Debug("Checking if app have an ID");

		if(strcmp(client->uKey,"0") && client->pID == NULL)
		{
			TuxLogger_Debug("Application don't have pID !");
			
			if(client->sock)
			{
				removeUniqueID(client->pID);
				
				//send message
				SendMsgToClient(client,TDSM_PRIORITY_NOIDSET);
				#ifdef _WIN32
				Sleep(200);
				#else
				TuxSleep(200);
				#endif
				close(client->sock);
			}
			
			return;
		}
	}
	

	if(!strcmp(strtolower(cmd),"tux_priority"))
	{
		if(argv == NULL)
		{
			TuxLogger_Warning("No parameters have been given for Tux_Priority() command but one parameter was expected !",NULL);
			return;
		}

		Tux_Priority(client, argv[1]);
	}

	/* Checking priority */
	/* Checking is done after commands Tux_Priority() and Tux_SetId() to avoid problems */
	/* If the connection fails, the application MUST be capable to retake priority */
	if(isLocked == 1)
	{
		if(client->pID == NULL)
		{
			TuxLogger_Warning("An application without identifier tried to send commands when priority is locked !",NULL);
			return;
		}

		if(strcmp(client->pID,PriorityID))
		{
			TuxLogger_Warning("An application without identifier tried to send commands communicate when priority is locked !",NULL);
			return;
		}
	}
	/* End of priority checking
	If we fall here, that's because the application sending commands has priority or no application asked for it */

	if(!strcmp(strtolower(cmd),"tux_getmicro"))
	{
		SendMsgToClient(client,TuxAudio_getMicroName());
	}

	if(!strcmp(strtolower(cmd),"tux_getsoundcard"))
	{
		SendMsgToClient(client, TuxAudio_getSoundCardName());
	}

	/* Stop all motors' movement (independently of each other)*/
	if(!strcmp(strtolower(cmd),"tux_off"))
	{
		if(argv == NULL)
		{
			TuxLogger_Warning("No parameters have been given for Tux_Off() command but one parameter was expected !",NULL);
			return;
		}

		Tux_Off(argv[0]);
	}

	if(!strcmp(strtolower(cmd),"tux_reset"))
		Tux_Reset();

	if(!strcmp(strtolower(cmd),"tux_timestamp"))
		Tux_Timestamp(client);

	if(!strcmp(strtolower(cmd),"tux_user"))
	{
		if(argv == NULL)
		{
			TuxLogger_Warning("No parameters have been given for Tux_User() command but one parameter was expected !",NULL);
			return;
		}

		TuxLogger_Debug("(Re)Defining username -> %s for client %d",argv[0],client->id);

		client->username = (char *)realloc(client->username,strlen(argv[0])*sizeof(char));
		strcpy(client->username,argv[0]);
	}


	/* Simulate remote's buttons pressed */
	if(!strcmp(strtolower(cmd),"tux_remote"))
	{
		if(argv != NULL)
			onRemoteButtonPressed(client, argv[0]); /* simulate button pressing */
		else
		{
			TuxLogger_Warning("No parameters have been given for Tux_Remote() command but one parameter was expected !",NULL);
		}
	}

	if(!strcmp(strtolower(cmd),"tux_playatt"))
	{
		if(argv == NULL)
		{
			TuxLogger_Warning("No parameters have been given for Tux_PlayAtt() command but one parameter was expected !",NULL);
			return;
		}
		TuxAttitune_loadAttitune(client, argv[0]);
	}

	if(!strcmp(strtolower(cmd),"tux_close"))
	{
		if(argv == NULL)
		{
			TuxLogger_Warning("No parameters have been given for Tux_Close() command but one parameter was expected !",NULL);
			return;
		}

		Tux_Close(argv[0]);
	}

	if(!strcmp(strtolower(cmd),"tux_open"))
	{
		if(argv == NULL)
		{
			TuxLogger_Warning("No parameters have been given for Tux_Open() command but one parameter was expected !",NULL);
			return;
		}

		Tux_Open(argv[0]);
	}

	if(!strcmp(strtolower(cmd),"tux_openclose"))
	{
		if(argv == NULL)
		{
			TuxLogger_Warning("No parameters have been given for Tux_OpenClose() command but one parameter was expected !",NULL);
			return;
		}

		if(argv[1] == NULL)
		{
			TuxLogger_Warning("Only one parameter given to Tux_OpenClose() but two parameters expected !",NULL);
			return;
		}

		int count;
		if(!(count = atoi(argv[1])))
		{
			TuxLogger_Warning("Second parameter in Tux_OpenClose() must be a number !",NULL);
			return;
		}

		Tux_OpenClose(argv[0],count);
	}

	if(!strcmp(strtolower(cmd),"tux_leds"))
	{
		if(argv == NULL)
			return;

		if(argv[0] == NULL)
			return;

		if(!strcmp(strtolower(argv[1]),"on") || !strcmp(strtolower(argv[1]),"off"))
			Tux_Leds_OnOff(argv[0],argv[1]);

		if(!strcmp(strtolower(argv[1]),"blink"))
			Tux_Leds_Blink(argv[0],argv[2],argv[3]);

		if(!strcmp(strtolower(argv[1]),"pulse"))
		{
			if(!strcmp(argv[6],""))
				return;

			Tux_Leds_Pulse(argv[0],argv[2],argv[3],argv[4],argv[5],argv[6],argv[7],argv[8]);
		}
	}

	if(!strcmp(strtolower(cmd),"tux_micro"))
	{
		if(argv == NULL)
			return;

		if(argv[1] == NULL)
			Tux_Micro(argv[0],NULL);
		else
			Tux_Micro(argv[0],duplicate_string(argv[1]));
	}

	if(!strcmp(strtolower(cmd),"tux_flippers"))
	{
		if(argv == NULL)
			return;

		if(argv[1] != NULL)
			Tux_Flippers(argv[0],argv[1]);
		else
			Tux_Flippers(argv[0],NULL);
	}

	if(!strcmp(strtolower(cmd),"tux_rotate"))
	{
		if(argv == NULL)
			return;

		if(argv[1] == NULL)
			return;

		Tux_Rotate(argv[0],argv[1]);
	}

	if(!strcmp(strtolower(cmd),"tux_flash"))
	{
		if(argv == NULL)
			return;

		if(argv[1] == NULL)
			return;

		Tux_Flash(argv[0],argv[1]);
	}

	/* Tux_TTS("blablabla",<voice>,<pitch>,<speed>,<stop>,<sync>) */
	if(!strcmp(strtolower(cmd),"tux_tts"))
	{
		TuxLogger_Debug("Tux_TTs() command",NULL);

		if(argv[0] == NULL)
			return;

		TuxLogger_Debug("There are at least one parameter",NULL);

		if(!strcmp(argv[0],"stoptts"))
			TuxAudio_StopTTS();
		else
			Tux_TTS(argv,argc);
	}

	/* Tux_Audio(PlayMusic,"/home/joe/Music_(premier titre).mp3") */
	if(!strcmp(strtolower(cmd),"tux_audio"))
	{
		if(argv == NULL)
			return;

		if(argv[1] == NULL)
			Tux_Audio(argv[0],NULL);
		else
			Tux_Audio(argv[0],rawcmd);
	}

	if(!strcmp(strtolower(cmd),"tux_state"))
	{
		if(argv == NULL)
			return;

		Tux_State(client, argv[0]);
	}
	
	//--------- ASR
	if(!strcmp(strtolower(cmd),"tux_flac"))
	{
		if(argv == NULL)
			return;
		
		Tux_Flac(client,argv[0]);
	}
	
	if(!strcmp(strtolower(cmd),"tux_asr"))
	{
		if(argv == NULL)
			return;
			
		Tux_ASR(client, argv[0]);
	}
	
	//--------- //ASR

	if(!strcmp(strtolower(cmd),"tux_sleep"))
		Tux_Sleep();

	if(!strcmp(strtolower(cmd),"tux_wakeup"))
		Tux_Wakeup();

	if(!strcmp(strtolower(cmd),"tux_resetpos"))
		TuxDrv_ResetPositions();

	freeExplode(argv);
	argv = NULL;

	free(cmd);
	cmd = NULL;
}

void *syncMouthMovements(void *data)
{
	syncMouthMov mov = (syncMouthMov)data;

	char **WORDS = explode(mov->phr,' ');
	int i;

	for(i = 0; i < mov->count; i++)
	{
		TuxLogger_Debug("WORDS[%d] -> %s",i,WORDS[i]);

		if(strlen(WORDS[i]) < 3)
		{
			TuxLogger_Debug("Less than 3 characters. Waiting 0.7 second",NULL);
			sleep(700);
		}
		else
		{
			TuxLogger_Debug("More than 3 characters. Counting characters' number to know how many movements are needed",NULL);

			int len = strlen(WORDS[i]);
			TuxLogger_Debug("Characters' number -> %d",len);

			if((len % 2))
			{
				TuxLogger_Debug("Odd number, add 1",NULL);
				len++;
			}

			int x = len / 4; /* 4 characters = 1 movement (@ normal speed) */
			if(x < 0)
				x=0;

			if(x == 0)
			{
				TuxLogger_Debug("Canceling movement, not enough characters !",NULL);
			}
			else
			{
				if(x % 2)
					x++;

				TuxLogger_Debug("Movements to do -> %d",x);
				Tux_OpenClose_Mouth(x);
			}
		}
	}

	if(mov->phr != NULL)
	{
		/*free(mov->phr);
		mov->phr=NULL;*/
	}

	if(mov != NULL)
	{
		/*free(mov);
		mov=NULL;*/
	}

	freeExplode(WORDS);
	WORDS = NULL;
	return 0;
}
