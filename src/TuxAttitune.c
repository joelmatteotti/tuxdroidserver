/* =============== GPL HEADER =====================
 * TuxAttitune.c is part of TuxDroidServer
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
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdbool.h>
#include <pthread.h>
#include <libxml/xmlreader.h>

#include <TuxLogger.h>
#include <TuxStrings.h>
#include <TuxAttitune.h>
#include <TuxDownloader.h>
#include <TuxDriver.h>
#include <TuxAudio.h>
#include <TuxTime.h>
#include <TuxUtils.h>
#include <TuxTypes.h>

pthread_mutex_t TTSMutex, WAVMutex;
pthread_t mThread;
pthread_attr_t mThread_attr;

attitune myAttitune = NULL; /* create an attitune */

char current_element[1024];

FILE *fp = NULL;

/* write the command in macro file */
void TuxAttitune_writeMacroCMD(attitune_block block) {
	
	TuxLogger_Debug("Adding a command in macro file",NULL);
	 
	if (block == NULL)
		TuxLogger_Debug("The block is Null",NULL);

	char gCmd[1024]; /* command formatter */

	snprintf(gCmd, 1024, "NULL"); /* no command recognized for the moment */

	if (!strcmp(block->cmd, "sound_play")) {
		/* Sound command*/
		snprintf(gCmd, 1024, "%f:TUX_CMD:SOUND_FLASH:PLAY:%d,100.0\n",
				block->start_time, block->track);
	} else if (!strcmp(block->cmd, "spinl_on")) {
		/* Left rotate */
		snprintf(gCmd, 1024, "%f:TUX_CMD:SPINNING:LEFT_ON:%d\n",
				block->start_time, block->count);
	} else if (!strcmp(block->cmd, "spinr_on")) {
		/* Right rotate */
		snprintf(gCmd, 1024, "%f:TUX_CMD:SPINNING:RIGHT_ON:%d\n",
				block->start_time, block->count);
	} else if (!strcmp(block->cmd, "mouth_close")) {
		/* Mouth closing */
		snprintf(gCmd, 1024, "%f:TUX_CMD:MOUTH:CLOSE\n", block->start_time);
	} else if (!strcmp(block->cmd, "mouth_open")) {
		/* Mouth opening */
		snprintf(gCmd, 1024, "%f:TUX_CMD:MOUTH:OPEN\n", block->start_time);
	} else if (!strcmp(block->cmd, "mouth_on")) {
		/* Mouth on */
		snprintf(gCmd, 1024, "%f:TUX_CMD:MOUTH:ON:%d,NDEF\n", block->start_time,
				block->count);
	} else if (!strcmp(block->cmd, "eyes_open")) {
		/* Eyes opening */
		snprintf(gCmd, 1024, "%f:TUX_CMD:EYES:OPEN\n", block->start_time);
	} else if (!strcmp(block->cmd, "eyes_close")) {
		/* Eyes closing */
		snprintf(gCmd, 1024, "%f:TUX_CMD:EYES:CLOSE\n", block->start_time);
	} else if (!strcmp(block->cmd, "eyes_on")) {
		/* Eyes on */
		snprintf(gCmd, 1024, "%f:TUX_CMD:EYES:ON:%d,NDEF\n", block->start_time,
				block->count);
	} else if (!strcmp(block->cmd, "leds_on")) {
		/* Leds on */
		snprintf(gCmd, 1024, "%f:TUX_CMD:LED:ON:LED_BOTH,1.0\n",
				block->start_time);
	} else if (!strcmp(block->cmd, "ledl_on")) {
		/* Left led on */
		snprintf(gCmd, 1024, "%f:TUX_CMD:LED:ON:LED_LEFT,1.0\n",
				block->start_time);
	} else if (!strcmp(block->cmd, "ledr_on")) {
		/* Right led on */
		snprintf(gCmd, 1024, "%f:TUX_CMD:LED:ON:LED_RIGHT,1.0\n",
				block->start_time);
	} else if (!strcmp(block->cmd, "leds_off")) {
		/* Leds off */
		snprintf(gCmd, 1024, "%f:TUX_CMD:LED:OFF:LED_BOTH\n",
				block->start_time);
	} else if (!strcmp(block->cmd, "ledl_off")) {
		/* Left led off */
		snprintf(gCmd, 1024, "%f:TUX_CMD:LED:OFF:LED_LEFT\n",
				block->start_time);
	} else if (!strcmp(block->cmd, "ledr_off")) {
		/* Right led off */
		snprintf(gCmd, 1024, "%f:TUX_CMD:LED:OFF:LED_RIGHT\n",
				block->start_time);
	} else if (!strcmp(block->cmd, "leds_blink")) {
		/* Leds blinking */
		snprintf(gCmd, 1024, "%f:TUX_CMD:LED:BLINK:LED_BOTH:%d,%d\n",
				block->start_time, block->count, block->speed);
	} else if (!strcmp(block->cmd, "wings_on")) {
		/* Flippers up and down */
		snprintf(gCmd, 1024, "%f:TUX_CMD:FLIPPERS:ON_DURING:%d,NDEF\n",
				block->start_time, block->count);
	} else if (!strcmp(block->cmd, "wings_up")) {
		/* Flippers up */
		snprintf(gCmd, 1024, "%f:TUX_CMD:FLIPPERS:UP\n", block->start_time);
	} else if (!strcmp(block->cmd, "wings_down")) {
		/* Flippers down */
		snprintf(gCmd, 1024, "%f:TUX_CMD:FLIPPERS:DOWN\n", block->start_time);
	}

	if (strcmp(gCmd, "NULL"))
		fputs(gCmd, fp);
}

const xmlChar *name, *value;

/* Parsing events inXML file */
void TuxAttitune_processNode(xmlTextReaderPtr reader) {
	name = xmlTextReaderConstName(reader);
	value = xmlTextReaderConstValue(reader);

	if (current_element != NULL && value != NULL) {
		
		if (!strcmp(current_element, "start_time")) {
			if (myAttitune->total_blocks >= 0)
				TuxAttitune_writeMacroCMD(
						myAttitune->attitune_block[myAttitune->total_blocks]);

			myAttitune->total_blocks++;

			myAttitune->attitune_block[myAttitune->total_blocks] =
					(attitune_block) malloc(sizeof(attitune_block_t));
			myAttitune->attitune_block[myAttitune->total_blocks]->start_time =
					atof((char *) value);

			/* Variables init*/
			myAttitune->attitune_block[myAttitune->total_blocks]->wav = NULL;
			myAttitune->attitune_block[myAttitune->total_blocks]->text = NULL;
			myAttitune->attitune_block[myAttitune->total_blocks]->cmd = NULL;
			myAttitune->attitune_block[myAttitune->total_blocks]->pitch = 50;
			myAttitune->attitune_block[myAttitune->total_blocks]->speed = 115;
		} else {
			if (!strcmp(current_element, "cmd")) {
				myAttitune->attitune_block[myAttitune->total_blocks]->cmd =
						duplicate_string((char *) value);
			}
			if (!strcmp(current_element, "duration"))
				myAttitune->attitune_block[myAttitune->total_blocks]->duration =
						atof((char *) value);
			if (!strcmp(current_element, "count"))
				myAttitune->attitune_block[myAttitune->total_blocks]->count =
						atoi((char *) value);
			if (!strcmp(current_element, "speed"))
				myAttitune->attitune_block[myAttitune->total_blocks]->speed =
						atoi((char *) value);
			if (!strcmp(current_element, "text")) {
				if (strlen((char *) value) > 1) {
					myAttitune->attitune_block[myAttitune->total_blocks]->text =
							duplicate_string((char *) value);
				}
			}
			if (!strcmp(current_element, "wav_name")) {
				myAttitune->attitune_block[myAttitune->total_blocks]->wav =
						(char *) malloc(
								sizeof(char)
										* (strlen((char *) value)
												+ strlen(myAttitune->directory)
												+ 5));
				strcpy(
						myAttitune->attitune_block[myAttitune->total_blocks]->wav,
						myAttitune->directory);
#ifdef _WIN32
				strcat(myAttitune->attitune_block[myAttitune->total_blocks]->wav,"wavs\\");
#else
				strcat(
						myAttitune->attitune_block[myAttitune->total_blocks]->wav,
						"wavs/");
#endif
				strcat(
						myAttitune->attitune_block[myAttitune->total_blocks]->wav,
						(char *) value);
			}
			if (!strcmp(current_element, "pitch"))
				myAttitune->attitune_block[myAttitune->total_blocks]->pitch =
						atoi((char *) value);
			if (!strcmp(current_element, "track"))
				myAttitune->attitune_block[myAttitune->total_blocks]->track =
						atoi((char *) value);

			/* header (informations on attitune) */
			if (!strcmp(current_element, "name")) {
				myAttitune->name = duplicate_string((char *) value);
			}
			if (!strcmp(current_element, "category")) {
				myAttitune->category = duplicate_string((char *) value);
			}
			if (!strcmp(current_element, "author")) {
				myAttitune->author = duplicate_string((char *) value);
			}
			if (!strcmp(current_element, "language")) {
				myAttitune->language = duplicate_string((char *) value);
			}
			if (!strcmp(current_element, "keywords")) {
				myAttitune->keywords = duplicate_string((char *) value);
			}
			if (!strcmp(current_element, "version")) {
				myAttitune->version = duplicate_string((char *) value);
			}
			if (!strcmp(current_element, "sub_category")) {
				myAttitune->sub_cat = duplicate_string((char *) value);
			}
			if (!strcmp(current_element, "description")) {
				myAttitune->desc = duplicate_string((char *) value);
			}
			if (!strcmp(current_element, "length")) {
				myAttitune->length = atof((char *) value);
			}
		}

		value = NULL;
	}

	if (name != NULL) {
		if (!strcmp((char *) name, "cmd") || !strcmp((char *) name, "duration")
				|| !strcmp((char *) name, "start_time")
				|| !strcmp((char *) name, "count")
				|| !strcmp((char *) name, "speed")
				|| !strcmp((char *) name, "text")
				|| !strcmp((char *) name, "wav_name")
				|| !strcmp((char *) name, "pitch")
				|| !strcmp((char *) name, "name")
				|| !strcmp((char *) name, "category")
				|| !strcmp((char *) name, "author")
				|| !strcmp((char *) name, "language") /* header */
				|| !strcmp((char *) name, "keywords")
				|| !strcmp((char *) name, "version")
				|| !strcmp((char *) name, "sub_category")
				|| !strcmp((char *) name, "description")) /* header */
				{
			strcpy(current_element, (char *) name);
		}

		name = NULL;
	}
}

/* XML file parsing */
bool TuxAttitune_ParseXMLFile(const char *xmlfile) {
	bool result = true;

	xmlTextReaderPtr reader;
	int ret;

	reader = xmlReaderForFile(xmlfile, NULL, 0);
	if (reader != NULL) {
		ret = xmlTextReaderRead(reader);
		while (ret == 1) {
			TuxAttitune_processNode(reader);
			ret = xmlTextReaderRead(reader);
		}
		xmlFreeTextReader(reader);

		if (ret != 0) {
			result = false;
			TuxLogger_Error( "Can't open xml file %s",
					xmlfile);
		}
	} else {
		result = false;
		TuxLogger_Error( "Can't open xml file %s", xmlfile);
	}

	return result;
}

/* Unzip file .att in a temporary directory */
int TuxAttitune_unzipAttitune(const char *attfile) {
	TuxLogger_Debug( "Unzipping Attitune -----> %s", attfile);

	char *cmd;
#ifdef _WIN32
	cmd = (char *)malloc(sizeof(char)*strlen(attfile)+34);
	strcpy(cmd,"unzip -q -o \"");
	strcat(cmd,attfile);
	strcat(cmd,"\" -d c:\\windows\\temp\\");
#else
	cmd = (char *) malloc(sizeof(char) * strlen(attfile) + 21);
	strcpy(cmd, "unzip -q -o ");
	strcat(cmd, attfile);
	strcat(cmd, " -d /tmp/");
#endif

	TuxLogger_Debug( "Unzipped ------> %s", cmd);

	return system(cmd);
}

void *TuxAttitune_startAttituneWav(void *data) {
	pthread_mutex_lock(&WAVMutex);
	attitune_block block = (attitune_block) data;

	if (block == NULL)
		return NULL; /* error */

	long time = (long) (block->start_time * 100);

	if (time > 0) {
#ifdef _WIN32
		Sleep(time);
#else
		TuxSleep(time);
#endif
	}

	TuxAudio_PlayMusic(block->wav);

	pthread_mutex_unlock(&WAVMutex);
	pthread_exit(0);
}

void *TuxAttitune_startAttituneTTS(void *data) {
	pthread_mutex_lock(&TTSMutex);
	attitune_block block = (attitune_block) data;

	if (block == NULL)
		return NULL; /* error */

	long time = (long) (block->start_time * 1000);

	if (time > 0) {
#ifdef _WIN32
		Sleep(time);
#else
		TuxSleep(time);
#endif
	}

	TuxAudio_PlayTTS("mb-fr1", block->text, block->pitch, block->speed, false);

	pthread_mutex_unlock(&TTSMutex);
	pthread_exit(0);
}

int compare(const void * a, const void * b) {
	return (*(int*) a - *(int*) b);
}

/* Start the attintune's loading */
void TuxAttitune_loadAttitune(tux_client client, char *attfile) {
	if (strstr(attfile, "http://")) {
		TuxLogger_Debug( "URL found. Download is needed !",NULL);

		char **ex = explode(attfile, '/');
		int argc = countCharacterOccurency(attfile, '/');

#ifdef _WIN32
		if(!TuxDownloader_DownloadFile(client, attfile,"c:\\windows\\temp\\"))
		{
			TuxLogger_Error("The download of the file %s have failed",attfile);
			return;	
		}	
			
		attfile = (char *)realloc(attfile,sizeof(char)*(16+strlen(ex[argc])));
		sprintf(attfile,"c:\\windows\\temp\\%s",ex[argc]);
#else
		if(!TuxDownloader_DownloadFile(client, attfile, "/tmp/"))
		{
			TuxLogger_Error("The download of the file %s have failed",attfile);
			return;
		}
		
		attfile = (char *) realloc(attfile,
				sizeof(char) * (6 + strlen(ex[argc])));
		snprintf(attfile, 25, "/tmp/%s", ex[argc]);
#endif

		freeExplode(ex);

	} else if (!file_exists(attfile)) {
		TuxLogger_Error( "File does not exist !", attfile);
		return;
	}

	if (TuxAttitune_unzipAttitune(attfile) != 1) {
		TuxLogger_Error( "Error during unzipping attitune !",NULL);
		return; /*TODO: Errors handling */
	}

	TuxLogger_Debug( "Create a new attitune",NULL);
	if (myAttitune != NULL) {
		if (myAttitune != NULL) {
			/*free(myAttitune);*//* freeing memory */
			/*myAttitune=NULL;*/
		}
		myAttitune = NULL; /* reinit to NULL */
	}

	TuxLogger_Debug( "Allocating memory for myAttitune",NULL);
	myAttitune = (attitune) malloc(sizeof(attitune_t));
	myAttitune->total_blocks = -1;

	char **sp;
	char **sp2;
	int occ;
	char *xmlpath;
	char *macropath;
#ifdef _WIN32
	sp = explode(attfile,'\\');
	occ = countCharacterOccurency(attfile,'\\');
	xmlpath = (char *)malloc(sizeof(char)*(strlen(attfile)+22));
	strcpy(xmlpath,"c:\\windows\\temp\\");
	sp2 = explode(sp[occ],'.');
	strcat(xmlpath,sp2[0]);
	strcat(xmlpath,"\\");
#else
	sp = explode(attfile, '/');
	occ = countCharacterOccurency(attfile, '\\');
	xmlpath = (char *) malloc(sizeof(char) * (strlen(attfile) + 11));
	strcpy(xmlpath, "/tmp/");
	sp2 = explode(sp[occ], '.');
	strcat(xmlpath, sp2[0]);
	strcat(xmlpath, "/");
#endif
	macropath = (char *) malloc(sizeof(char) * strlen(xmlpath));
	myAttitune->directory = (char *) malloc(sizeof(char) * strlen(xmlpath));
	strcpy(myAttitune->directory, xmlpath);
	strcpy(macropath, xmlpath);
	strcat(macropath, "macro.txt");
	strcat(xmlpath, "scene.xml");

	freeExplode(sp);
	freeExplode(sp2);

	TuxLogger_Debug( "XML PATH ------------------> %s",
			xmlpath);

	if (!file_exists(xmlpath)) {
		TuxLogger_Error(
				"Something wrong happened during unzipping attitune !!",NULL);

		if (xmlpath != NULL) {
			free(xmlpath);
			xmlpath = NULL;
		}

		if (macropath != NULL) {
			free(macropath);
			macropath = NULL;
		}

		if (myAttitune != NULL) {
			free(myAttitune);
			myAttitune = NULL;
		}
		return;
	}

	TuxLogger_Debug( "XML parsing started",NULL);

	fp = fopen(macropath, "a+");
	bool result = TuxAttitune_ParseXMLFile(xmlpath);

	if (xmlpath != NULL) {
		free(xmlpath);
		xmlpath = NULL;
	}

	if (fp != NULL) {
		fclose(fp);
		fp = NULL;
	}

	if (result) {
		TuxLogger_Debug( "XML parsing ended",NULL);
		TuxLogger_Debug( "Number of blocks: %d",
				myAttitune->total_blocks);

		printf("Attitune %s (version %s) made by %s is now loaded !",
				myAttitune->name, myAttitune->version, myAttitune->author);

		TuxLogger_Info(
				"Attitune %s (version %s) made by %s is now loaded !",
				myAttitune->name, myAttitune->version, myAttitune->author);

		int i;
		for (i = 0; i < myAttitune->total_blocks; i++) {
			if (myAttitune->attitune_block[i] != NULL) {

				if (myAttitune->attitune_block[i]->wav != NULL) {
					pthread_mutex_init(&WAVMutex, NULL);
					pthread_attr_setdetachstate(&mThread_attr,
							PTHREAD_CREATE_DETACHED);
					pthread_create(&mThread, &mThread_attr,
							TuxAttitune_startAttituneWav,
							myAttitune->attitune_block[i]);
				}

				if (myAttitune->attitune_block[i]->text != NULL) {
					pthread_mutex_init(&TTSMutex, NULL);
					pthread_attr_setdetachstate(&mThread_attr,
							PTHREAD_CREATE_DETACHED);
					pthread_create(&mThread, &mThread_attr,
							TuxAttitune_startAttituneTTS,
							myAttitune->attitune_block[i]);
				}
			}
		}

		TuxDrv_PerformMacroFile(macropath);
	}
	if (macropath != NULL) {
		free(macropath);
		macropath = NULL;
	}
}
