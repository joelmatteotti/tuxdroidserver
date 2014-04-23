/* =============== GPL HEADER =====================
 * TuxDownloader.c is part of TuxDroidServer
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
#include <malloc.h>
#include <TuxDownloader.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <string.h>
#include <stdbool.h>
#include <TuxStrings.h>
#include <TuxUtils.h>
#include <TuxLogger.h>
#include <TuxServer.h>
#include <TuxTypes.h>
#include <TuxMessages.h>

size_t TuxDownloader_writeData(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	size_t written;
	written = fwrite(ptr, size, nmemb, stream);
	return written;
}

bool TuxDownloader_DownloadFile(tux_client client, char *url, char *destination)
{
	bool result = false;
	
	CURL *curl;
	FILE *fp;

	char **ex = explode(url,'/');
	int argc = countCharacterOccurency(url,'/');

	char *file = (char *)malloc(sizeof(char)*(strlen(destination)+1+strlen(ex[argc])));
	sprintf(file,"%s%s",destination,ex[argc]);

	curl = curl_easy_init();
	if (curl)
	{
		fp = fopen(file,"wb");
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, TuxDownloader_writeData);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		CURLcode result = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		fclose(fp);

		TuxLogger_Debug("CURL Error Code: %s",curl_easy_strerror(result));

		if(result != CURLE_OK)
		{
			char *msg = (char *)malloc(sizeof(char)*strlen(curl_easy_strerror(result))+strlen(TDSM_DOWNLOADFAILED)-2);
			sprintf(msg,TDSM_DOWNLOADFAILED,curl_easy_strerror(result));
			SendMsgToClient(client,msg);	
			result = false;
		}
	}
	else
		result = false;
	
	free(file);
	file = NULL;
	freeExplode(ex);
	ex = NULL;

	return result;
}

bool TuxDownloader_checkSiteAccess(char *url)
{
	CURL *curl;
	curl = curl_easy_init();
	if(curl) 
	{
		#ifdef _WIN32
		FILE *fp = fopen("c:\\windows\\temp\\null","wb");
		#else
		FILE *fp = fopen("/tmp/null","wb");
		#endif
		
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, TuxDownloader_writeData);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		
		CURLcode result = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		
		if(result == 0)
			return true;
	}

	return false;
}

