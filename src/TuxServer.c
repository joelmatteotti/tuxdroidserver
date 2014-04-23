/* =============== GPL HEADER =====================
 * TuxServer.c is part of TuxDroidServer
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
#include <strings.h>
#include <malloc.h>
#include <pthread.h>
#include <stdint.h>
#include <math.h> /* lrint() */
#include <sys/stat.h>

#include <TuxCompat.h>
#include <TuxMessages.h>
#include <TuxTime.h>
#include <TuxLogger.h>
#include <TuxDriver.h>
#include <TuxAudio.h>
#include <TuxServer.h>
#include <TuxStrings.h>
#include <TuxAttitune.h>
#include <TuxDownloader.h>
#include <TuxState.h>
#include <TuxUtils.h>
#include <TuxLang.h>
#include <TuxConfig.h>
#include <TuxCommands.h>
#include <TuxDroid.h>
#include <TuxASR.h>

extern tuxconfig config;

char **UniqueIDs;
int nUniqueIDs = 0;


/* Pointer on Tux clients */
tux_client* clients = NULL;

/* Current number of clients */
int nClients = 0;
/* Is the server running or not */
int isRunning = 0;

static bool server_started = false;

bool isRealyUniqueID(char *ID)
{
	TuxLogger_Debug("isRealyUniqueID => %s",ID);
	
	bool isUnique = true;
	
	TuxLogger_Debug("before loop on %d ID",nUniqueIDs);
	int i;
	for(i = 0; i < nUniqueIDs; i++)
	{
		TuxLogger_Debug("In the loop => i = %d",i);
		if(UniqueIDs[i] != NULL)
		{
			TuxLogger_Debug("ID[%d] not null !",i);
			if(!strcmp(UniqueIDs[i],ID))
			{	TuxLogger_Debug("ID found so is not unique !!");
				isUnique=false;
				break;
			}
		}
	}
	
	if(isUnique)
		TuxLogger_Debug("ID is unique :)");
	
	return isUnique;
}

void removeUniqueID(char *ID)
{
	TuxLogger_Debug("removeUniqueID => %s",ID);
	
	if(ID == NULL)
		return;
	
	TuxLogger_Debug("Before the loop on %d ID",nUniqueIDs);
	int i;
	for(i = 0; i < nUniqueIDs; i++)
	{
		TuxLogger_Debug("In the loop => i = %d",i);
		if(UniqueIDs[i] != NULL)
		{
			TuxLogger_Debug("ID[%d] not null",i);
			if(!strcmp(UniqueIDs[i],ID))
			{
				free(UniqueIDs[i]);
				UniqueIDs[i] == NULL;
				
				if(nUniqueIDs >= 2)
				{
					UniqueIDs[i] = (char *)malloc(sizeof(char)*strlen(UniqueIDs[nUniqueIDs-1]));
					sprintf(UniqueIDs[i],"%s",UniqueIDs[nUniqueIDs-1]);
					free(UniqueIDs[nUniqueIDs-1]);
					UniqueIDs[nUniqueIDs-1];
					nUniqueIDs--;
					UniqueIDs = (char **)realloc(UniqueIDs,nUniqueIDs);
				}
				else
				{
					free(UniqueIDs);
					UniqueIDs = NULL;
					nUniqueIDs = 0;
				}
			}
		}
	}
}

void addUniqueID(char *ID)
{
	TuxLogger_Debug("addUniqueID => %s",ID);

	if(ID == NULL)
		return;
	
	if(nUniqueIDs == 0)
	{
		TuxLogger_Debug("Not yet ID definined, do first allocation");
		UniqueIDs = (char **)malloc(sizeof(char *));
	}
	else
	{
		TuxLogger_Debug("Already %d ID defined, reallocate 1 place in the memory");
		UniqueIDs = (char **)realloc(UniqueIDs,nUniqueIDs+1);
	}

	TuxLogger_Debug("Before the allocation for the ID");
	UniqueIDs[nUniqueIDs] = (char *)malloc(sizeof(char)*strlen(ID)+1);
	TuxLogger_Debug("After allocation for the ID");
	
	TuxLogger_Debug("Before copy the ID");
	sprintf(UniqueIDs[nUniqueIDs],"%s",ID);
	TuxLogger_Debug("After copy of the id, all fine :)");
	
	
	nUniqueIDs++;
}

/**
 Read data from clients
 @param data data to be read
 */
void* ReadClient(void *data) {
	TuxLogger_Debug("Start ReadClient()",NULL);
	
	if (data == NULL)
		return NULL;

	tux_client client = (tux_client) data;

	if (client == NULL) {
		return NULL;
	}

	char *buff = (char *) malloc(sizeof(char) * 2048);

	int iResult;
	int isAlive = 0;

	TuxLogger_Debug("ReadClient() varible are initialized",NULL);

	TuxLogger_Debug("Start reading data from the client %d",
		client->id);

	do {
		TuxLogger_Debug("Reading looping for client %d",
			client->id);
		
		iResult = recv(client->sock, buff, 2048, 0);

		if (iResult > 0) {
			TuxLogger_Debug("Receiving data !",NULL);

			isAlive = 1;

			TuxLogger_Debug(
					"Allocating memory for the command",NULL);
			char *cmd = (char *) malloc(sizeof(char) * (iResult + 1));
			TuxLogger_Debug( "Allocation OK",NULL);
			TuxLogger_Debug( "Copy data in cmd",NULL);
			snprintf(cmd, iResult + 1, "%s", buff);

			TuxLogger_Debug( "Raw data received: %s", cmd);

			if (buff != NULL) {
				/*free(buff);
				 buff=NULL;*/
			}

			ParseCommand(client, trim(cmd));

			if (cmd != NULL) {
				/*free(cmd);
				 cmd=NULL;*/
			}

		} else
			isAlive = 0;

		break;
	} while (iResult > 0);

	if (isAlive == 1) {
		TuxLogger_Debug(
				"Client is still alive. Keep watching data coming from him",NULL);
				
		ReadClient(data);
	} else {
		TuxLogger_Debug(
				"Connection with the client %d has been closed by him. Closing the socket..",
				client->id);
		
		removeUniqueID(client->pID);
		
		close(client->sock);
		TuxLogger_Debug( "Socket closed", client->id);
	}

	if (client->uKey != NULL) {
		/*free(client->uKey);
		 client->uKey = NULL;*/
	}

	if (client->username != NULL) {
		/*free(client->username);
		 client->username=NULL;*/
	}

	if (client != NULL) {
		/*free(client);
		 client=NULL;*/
	}

	if (data != NULL) {
		/*free(data);
		 data=NULL;*/
	}

	return 0;
}

void SendMsgToAll(char *msg, tux_client except_client)
{
	if(!server_started)
		return;
	
	TuxLogger_Debug("Server sending message to all clients (message: %s)",msg);
	
	
	int dSize = strlen(msg) + 2;
	char *data = (char *)malloc(sizeof(char)*dSize);
	sprintf(data,"%s\r",msg);
	
	int check_except=0;
	TuxLogger_Debug("Check if a client d'ont must receive the message !",NULL);
	
	if(except_client != NULL)
	{
		TuxLogger_Debug("The client %d must be ignored",except_client->id);
		check_except=1;
	}
	
	int ignore = 0;
	int i;
	
	for(i = 0; i < nClients; i++)
	{
		TuxLogger_Debug("Check if the connection with the client %d is always alive...",clients[i]->id);
		if(check_except == 1 && except_client != NULL)
		{
			TuxLogger_Debug("Ignore asked !",NULL);
			if(except_client->id == clients[i]->id)
			ignore = 1;
		}
		if(ignore == 0)
		{
			if(clients[i]->sock)
			{
				TuxLogger_Debug("The connection is alive !",NULL);
				if(!strcmp(clients[i]->uKey,config->USER_KEY))
				{
					TuxLogger_Debug("The client %d has the correct key (%s) then we send the message :)",i,clients[i]->uKey);
					send(clients[i]->sock,data,dSize,0);
				}
				else
				{
					TuxLogger_Debug("The client %d has an invalid key (%s)then disconnect him",clients[i]->id,clients[i]->uKey);
					if(clients[i]->sock)
					{
						removeUniqueID(clients[i]->pID);
					
						close(clients[i]->sock);
						if(clients[i]->uKey != NULL)
						{
							/*free(clients[i]->uKey);
							clients[i]->uKey =NULL;*/
						}
				
						if(clients[i]->username != NULL)
						{
							/*free(clients[i]->username);
							clients[i]->username=NULL;*/
						}
				
						if(clients[i] != NULL)
						{
							/*free(clients[i]);
							clients[i] = NULL;*/
						}
					}
				}
			}
			else
			{
				TuxLogger_Debug("The connection is no longer active we close the socket",NULL);
				
				removeUniqueID(clients[i]->pID);
				
				close(clients[i]->sock);
				if(clients[i]->uKey != NULL)
				{
					/*free(clients[i]->uKey);
					clients[i]->uKey=NULL;*/
				}
				
				if(clients[i]->username != NULL)
				{
					/*free(clients[i]->username);
					clients[i]->username=NULL;*/
				}
				
				if(clients[i] != NULL)
				{
					/*free(clients[i]);
					clients[i] = NULL;*/
				}
			}
		}
		else
		{
			TuxLogger_Debug("The client %d was ignored as requested !",except_client->id);
		}
	}
	
	if(data != NULL)
	{
		free(data);
		data=NULL;
	}
} 

/* Send a message to specific client */
void SendMsgToClient(tux_client client, char *message) {
	if (!server_started)
		return;

	TuxLogger_Debug(
			"(X)Sending this message to client %d: %s", client->id, message);

	int dSize = sizeof(message) + 2;

	char *data = (char *) malloc(sizeof(char) * dSize);

	sprintf(data, "%s\r", message);

	TuxLogger_Debug(
			"(X)Checking if connection with client %d is still alive..",
			client->id);

	if (client->sock) {
		TuxLogger_Debug( "(X)Connection is still alive !",NULL);

		if (!strcmp(client->uKey, config->USER_KEY)) {
			TuxLogger_Debug(
					"(X)Client %d has a valid key (%s), send him the message :)",
					client->id, client->uKey);
			send(client->sock, data, strlen(data), 0);
			TuxLogger_Debug( "(X)Message sent :)",NULL);
		} else {
			TuxLogger_Debug(
					"(X)The client %d has an invalid key (%s), disconnecting him..",
					client->id, client->uKey);

			if (client->sock)
			{
				removeUniqueID(client->pID);
				close(client->sock);
			}
		}
	} else {
		TuxLogger_Debug(
				"(X)The connection with client %d wasn't active anymore, shut it down",
				client->id);
		close(client->sock);
	}

	if (data != NULL) {
		/*free(data);
		 data=NULL;*/
	}
}

/**
 Start server
 */
void *startServer(void *data) {
	TuxLogger_Debug(
			"Initializing TuxDroidInterface..",NULL);

	if (!InitializeTuxDroid()) /* Initializing TuxDroidInterface */
	{
		TuxLogger_Info(
				"Can't initialize TuxDroid :(",NULL);
		tprintf("\n\n%s\n", TuxLang_getTranslation("START_INIT_ERROR_L1"));
		tprintf("%s\n", TuxLang_getTranslation("START_INIT_ERROR_L2"));
		tprintf("%s\n\n", TuxLang_getTranslation("START_INIT_ERROR_L3"));
		tprintf("%s", TuxLang_getTranslation("START_INIT_ERROR_L4"));

		while (1) {
		}
	}

#ifdef _WIN32
	Sleep(2000);
#else
	TuxSleep(2000);
#endif

	TuxLogger_Debug( "Starting server...",NULL);

	TuxLogger_Debug( "Initializing sockets..",NULL);

	/* Initializing TCP server */
#ifdef _WIN32
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2,0), &WSAData);
#endif

	int sock;
	int csock=-1;
	struct sockaddr_in sin;
	struct sockaddr_in csin;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_family = AF_INET;
	sin.sin_port = htons((u_short) config->SERVER_PORT);

	bind(sock, (struct sockaddr *) &sin, sizeof(sin));
	listen(sock, 0);

	socklen_t sinsize = sizeof(csin);

	TuxLogger_Debug( "Socket initialization done",NULL);

	isRunning = 1;

	server_started = true;

	pthread_t SocketThreads;
	pthread_attr_t SocketThreads_attr;

	tprintf("\n%s\n", TuxLang_getTranslation("SERVER_STARTED"));
	
	if(!config->ASR_ENABLED)
	{
		tprintf("\n%s\n",TuxLang_getTranslation("ASR_DISABLED"));
	}

	printf("\nTuxDroidServer> ");

	/* Waiting for connections */
	while (isRunning) {
		if ((csock = accept(sock, (struct sockaddr *) &csin, &sinsize))) {
			clients = (tux_client*) realloc(clients,
					sizeof(tux_client) * (nClients + 1));

			TuxLogger_Debug(
					"New incoming connection accepted",NULL);
			TuxLogger_Debug( "IP Client: %s",
					inet_ntoa(csin.sin_addr));

			TuxLogger_Debug(
					"Creating new client -> %d..", nClients);
			
			clients[nClients] = (tux_client) malloc(sizeof(tux_client_t)); /* allocating memory */

			TuxLogger_Debug("New client created",NULL);

			clients[nClients]->id = nClients;
			clients[nClients]->sock = csock;
			clients[nClients]->uKey = duplicate_string("0");
			clients[nClients]->pID = NULL;

			clients[nClients]->username = duplicate_string("0");

			TuxLogger_Debug("Starting new thread..",NULL);

			pthread_attr_init(&SocketThreads_attr);
			pthread_attr_setdetachstate(&SocketThreads_attr,
					PTHREAD_CREATE_DETACHED);
			pthread_create(&SocketThreads, &SocketThreads_attr, ReadClient,
					clients[nClients]);

			TuxLogger_Debug("New thread started for client %d", nClients);

			nClients++;

		}
	}

	/* If we fall here, that's because server has been shutdown, close all connections */
	if (csock) {
		TuxLogger_Debug(
				"Closing temporary socket",NULL);
		close(csock);
	}

	if (sock) {
		TuxLogger_Debug( "Closing server socket",NULL);
		close(sock);
	}

#ifdef _WIN32
	WSACleanup();
#endif

	return 0;
}

void basicStart() {
	if (!server_started) {
		TuxLogger_Debug(
				"Creating main thread..",NULL);
		pthread_attr_t serverThread_attr;
		pthread_t serverThread;
		pthread_attr_init(&serverThread_attr);
		pthread_attr_setdetachstate(&serverThread_attr,
				PTHREAD_CREATE_DETACHED);
		TuxLogger_Debug(
				"Main thread created",NULL);
		pthread_create(&serverThread, &serverThread_attr, startServer, NULL);
		TuxLogger_Debug( "Main thread started",NULL);
	} else {
		TuxLogger_Debug( "Server is already started !",NULL);
		tprintf("%s\n", TuxLang_getTranslation("SERVER_ALREADY_STARTED"));
	}
}

int main(void) {
	TuxConfig_loadConfig();
	
	#ifndef _WIN32
	system("clear");
	#endif

	if (!TuxLang_loadTranslation(config->LANG_FILE)) {
		printf("Unable to load the file lang/%s\n", config->LANG_FILE);
		printf("Please verify your config/tuxdroidserver.txt\n\n");
		printf(
				"For more informations see link bellow:\nhttp://sourceforge.net/p/tuxdroidserver/english/ConfigFile_(en)/\n\n");
		printf("Type Ctrl+C to close this application.\n\n");
		getchar();
		exit(0);
	}

	char ch = '\0';
	char buffer[5];
	int char_count;

	tprintf("*********************************************************\n");
	tprintf("* TuxDroidServer (Version %d.%d.%d) revision %d\t\t*\n",
			TUXSERVER_MAJOR, TUXSERVER_MINOR, TUXSERVER_REVIS, TDS_REVISION);
	tprintf("*\t\t\t\t\t\t\t*\n");
	tprintf("* %s %s\t\t\t*\n", TuxLang_getTranslation("HEADER_L1"),
			TUXSERVER_AUTHOR);
#ifdef TUXSERVER_MAINTENER
	tprintf("*\t\t\t\t\t\t\t*\n");
	tprintf("* %s %s\t\t*\n", TuxLang_getTranslation("HEADER_L2"),
			TUXSERVER_MAINTENER);
#else
	tprintf("*\t\t\t\t\t\t\t*\n");
#endif
#ifdef TUXDRIVER_MAINTENER
	tprintf("* %s %s\t\t*\n", TuxLang_getTranslation("HEADER_L3"),
			TUXDRIVER_MAINTENER);
#endif
#ifdef TUXAUDIO_MAINTENER
	tprintf("* %s %s\t\t*\n", TuxLang_getTranslation("HEADER_L4"),
			TUXAUDIO_MAINTENER);
#endif
	tprintf("*\t\t\t\t\t\t\t*\n");
	tprintf("* https://sourceforge.net/projects/tuxdroidserver/\t*\n");
	tprintf("*********************************************************\n\n");
	
	TuxLogger_Info("TuxDroidServer v%d.%d.%d-rev%d started.",
		TUXSERVER_MAJOR, TUXSERVER_MINOR, TUXSERVER_REVIS, TDS_REVISION);

	tprintf("%s %d\n\n", TuxLang_getTranslation("SERVER_LISTENING_PORT"),
			config->SERVER_PORT);

	tprintf("start - %s\n", TuxLang_getTranslation("START_CMD"));
	tprintf("stop  - %s\n\n", TuxLang_getTranslation("STOP_CMD"));

	if (config->AUTO_START)
		basicStart();
	else
		TuxLogger_Info( "Server waiting for orders..",NULL);

	do {
		tprintf("TuxDroidServer> ");
		ch = getchar();
		char_count = 0;
		while ((ch != '\n') && (char_count < 5)) {
			buffer[char_count++] = ch;
			ch = getchar();
		}
		buffer[char_count] = 0x00; /*NULL*/

		TuxLogger_Debug( "Received command ------> %s", buffer);

		if (!strcmp(buffer, "start")) {

			TuxLogger_Debug(
					"Asking for server start ! buffer -----> %s",
					buffer);

			basicStart();

		}

		if (!strcmp(buffer, "stop")) {
			TuxLogger_Info( "Asking for server stop",NULL);

			tprintf("\n%s", TuxLang_getTranslation("SERVER_STOPPING"));

			/* Waiting before stopping the server for server_started variable to be updated */
#ifdef _WIN32
			Sleep(2000); /* 2 seconds */
#else
			TuxSleep(2000); /* 2 seconds */
#endif
			break;
		}
	} while (1);

	if (server_started) {
		server_started = false;

		TuxLogger_Info( "Stopping server..",NULL);

		/* Stop server */
		/* Close clients' sockets */
		/* Close server's socket */
		/* Close application */

		isRunning = 0; /* we are stopping the server*/

		/* Waiting few seconds to be sure that the first sockets will be closed */
#ifdef _WIN32
		Sleep(2000); /* 2 seconds */
#else
		TuxSleep(2000); /* 2 seconds */
#endif

		tprintf("\n%s", TuxLang_getTranslation("CONNECTIONS_CLOSING"));

		TuxLogger_Info( "Closing connections..",NULL);

		int i;
		for (i = 0; i < nClients; i++) {
			TuxLogger_Debug(
					"Checking if connection's client %d is still alive",
					clients[i]->id);
			if (clients[i]->sock) {
				TuxLogger_Debug(
						"Closing connection for client %d", clients[i]->id);

				removeUniqueID(clients[i]->pID);
				
				close(clients[i]->sock);


				if (clients[i]->uKey != NULL) {
					/*free(clients[i]->uKey);
					 clients[i]->uKey=NULL;*/
				}

				if (clients[i]->username != NULL) {
					/*free(clients[i]->username);
					 clients[i]->username=NULL;*/
				}

				if (clients[i] != NULL) {
					/*free(clients[i]);
					 clients[i] = NULL;*/
				}
			}
		}

		/*
		 free(SocketThreads);
		 */
		/* Waiting 2 more seconds to be sure all connections are closed before stopping driver */
#ifdef _WIN32
		Sleep(2000); /* 2 seconds */
#else
		TuxSleep(2000); /* 2 seconds */
#endif

		tprintf("\n%s", TuxLang_getTranslation("MOTORS_STOPPING"));

		TuxLogger_Info(
				"Stopping all Tuxdroid's movements..",NULL);

		TuxDrv_Mouth_Off();
		TuxDrv_Spinning_Off();
		TuxDrv_Flippers_Off();

#ifdef _WIN32
		TuxDrv_Eyes_Off(); /* disabled to avoid crash on Linux due to au driver's segfault */
#endif

#ifdef _WIN32
		Sleep(2000); /* 2 seconds */
#else
		TuxSleep(2000); /* 2 seconds */
#endif

		tprintf("\n%s", TuxLang_getTranslation("MOTORS_RESETING"));

		TuxLogger_Info(
				"Reinitializing motors' positions",NULL);

		TuxDrv_ResetPositions();

#ifdef _WIN32
		Sleep(5000); /* 5 seconds */
#else
		TuxSleep(5000); /* 5 seconds */
#endif

		tprintf("\n%s\n\n", TuxLang_getTranslation("AUDIO_STOPPING"));
		TuxAudio_StopMusic();
		TuxAudio_StopTTS();

#ifdef _WIN32
		Sleep(2000); /* 2 seconds */
#else
		TuxSleep(2000); /* 2 seconds */
#endif

		tprintf("\n%s\n\n", TuxLang_getTranslation("DRIVER_STOPPING"));

		TuxLogger_Info( "Stopping TuxDroid driver",NULL);

		/* Stopping driver */
		TuxDrv_Stop();

		/* Wait to output driver's messages before closing :) */
#ifdef _WIN32
		Sleep(2000); /* 2 seconds */
#else
		TuxSleep(2000); /* 2 seconds */
#endif
	}
	/* PROGRAM'S END */

	return 0;
}

