/* =============== GPL HEADER =====================
 * TuxConfig.c is part of TuxDroidServer
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
#include <TuxConfig.h>
#include <TuxLogger.h>
#include <TuxStrings.h>
#include <TuxUtils.h>
#include <TuxTypes.h>
#include <TuxDownloader.h>

#ifndef _WIN32
	#include <sys/stat.h>
#endif

tuxconfig config = NULL;



void TuxConfig_checkConfigDir()
{
	if(!directory_exists("./config"))
	{
		#ifdef _WIN32
		mkdir("config");
		#else
		mkdir("config",0755);
		#endif
	}
}

void TuxConfig_writeConfig()
{
	TuxLogger_Error("Writing default config file ..",NULL);

	FILE *fp = fopen("config/tuxdroidserver.txt","w");
	if(fp)
	{
		fputs("SERVER_PORT=9595\n\0",fp);
		fputs("USER_KEY=test\n\0",fp);
		fputs("LOG_LEVEL=0\n\0",fp);
		fputs("LANG_FILE=FR.txt\n\0",fp);
		fputs("LANG_ASR=fr-FR\n\0",fp);
		fputs("AUTO_START=0\n\0",fp);
		fclose(fp);
	}
	else
		TuxLogger_Error("Can't create default config file tuxdroidserver.txt !!",NULL);

}

/* Loading config */
void TuxConfig_loadConfig()
{
	TuxConfig_checkConfigDir();

	TuxLogger_Debug("Function loadConfig()",NULL);
	TuxLogger_Debug("Opening config file tuxdroidserver.txt",NULL);

	config = (tuxconfig)malloc(sizeof(tuxconfig_t));

	char buff[1024];
	FILE *fp = fopen("config/tuxdroidserver.txt","r");

	config->SERVER_PORT = -1;
	config->USER_KEY="";
	config->LOG_LEVEL=-1;
	config->LANG_FILE="";
	config->LANG_ASR="";
	config->AUTO_START= -1;
	config->ASR_ENABLED=false;
	
	if(TuxDownloader_checkSiteAccess("http://www.google.com/speech-api/v1/recognize"))
		config->ASR_ENABLED=true;

	if(!fp)
	{
		TuxLogger_Error("Config file tuxdroidserver.txt not found !!",NULL);
		TuxConfig_writeConfig();

		TuxLogger_Debug("Setting default config",NULL);

		config->SERVER_PORT = 9595;
		config->USER_KEY = (char *)malloc(sizeof(char)*4);
		sprintf(config->USER_KEY,"test");
		config->LOG_LEVEL = TUX_LOG_INFO;
		config->LANG_FILE = (char *)malloc(sizeof(char)*6);
		sprintf(config->LANG_FILE,"FR.txt");
		config->LANG_ASR = (char *)malloc(sizeof(char)*5);
		sprintf(config->LANG_ASR,"fr-FR");
		config->AUTO_START = 0;

	}
	else
	{
		while(!feof(fp))
		{
			fgets(buff,1024,fp);
			
			if(strcmp(buff,""))
			{
				if(strstr(buff,"="))
				{
					char **param = explode(buff,'=');

					if(!strcmp(param[0],"SERVER_PORT"))
					{
						TuxLogger_Debug("SERVER PORT LISTENING TO %s",param[1]);
						config->SERVER_PORT = atoi(trim(param[1]));
					}

					if(!strcmp(param[0],"USER_KEY"))
					{
						TuxLogger_Debug("USER_KEY SET TO %s",param[1]);
						config->USER_KEY = (char *)malloc(sizeof(char)*4);
						sprintf(config->USER_KEY,"test");
					}

					if(!strcmp(param[0],"LOG_LEVEL"))
					{
						TuxLogger_Debug("LOG LEVEL SET TO %s",param[1]);
						config->LOG_LEVEL=atoi(trim(param[1]));
						TuxLogger_setLevel(config->LOG_LEVEL);
					}

					if(!strcmp(param[0],"LANG_FILE"))
					{
						TuxLogger_Debug("LANG FILE => %s",param[1]);
						config->LANG_FILE = (char *)malloc(sizeof(char)*6);
						sprintf(config->LANG_FILE,trim(param[1]));
					}
					
					if(!strcmp(param[0],"LANG_ASR"))
					{
						TuxLogger_Debug("LANG ASR SET TO %s",param[1]);
						config->LANG_ASR = (char *)malloc(sizeof(char)*6);
						sprintf(config->LANG_ASR,trim(param[1]));
					}

					if(!strcmp(param[0],"AUTO_START"))
					{
						TuxLogger_Debug("AUTO START SET TO %s",param[1]);
						config->AUTO_START = atoi(trim(param[1]));
					}

					freeExplode(param);
					param = NULL;
				}
			}
		}

		TuxLogger_Debug("End of reading config file",NULL);
		fclose(fp);
		
		if(config->SERVER_PORT < 0 ||config->LOG_LEVEL < 0
			|| !strcmp(config->USER_KEY,"") || !strcmp(config->LANG_FILE,"")
			|| !strcmp(config->LANG_ASR,""))
		{
			fp = fopen("config/tuxdroidserver.txt","a+");
		}
		
		if(config->SERVER_PORT < 0)
			fputs("SERVER_PORT=9595\n\0",fp);
		if(config->LOG_LEVEL < 0)
			fputs("LOG_LEVEL=0\n\0",fp);
		if(!strcmp(config->USER_KEY,""))
			fputs("USER_KEY=test\n\0",fp);
		if(!strcmp(config->LANG_FILE,""))
			fputs("LANG_FILE=FR.txt\n\0",fp);
		if(!strcmp(config->LANG_ASR,""))
			fputs("LANG_ASR=fr-FR\n\0",fp);
		if(config->AUTO_START < 0)
			fputs("AUTO_START=0\n\0",fp);
			
		if(config->SERVER_PORT < 0 ||config->LOG_LEVEL < 0
			|| !strcmp(config->USER_KEY,"") || !strcmp(config->LANG_FILE,"")
			|| !strcmp(config->LANG_ASR,""))
		{
			fclose(fp);
			TuxConfig_loadConfig(); /* reload config */
		}
	}

	if(buff != NULL)
	{
		/*free(buff);
		buff=NULL;*/
	}
}
