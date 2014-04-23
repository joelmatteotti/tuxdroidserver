/* =============== GPL HEADER =====================
 * TuxLang.c is part of TuxDroidServer
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

#include <TuxLang.h>
#include <string.h>
#include <malloc.h>
#include <stdbool.h>


tux_translation *translations; 	/* Variables containing string translations */
int tr_count; 					/* Translations counter */


char *TuxLang_getTranslation(char *_key_)
{
	int i;
	char *str=NULL;
	for(i = 0; i < tr_count; i++)
	{
		if(!strcmp(translations[i]->key,_key_))
		{
			str = (char *)malloc(sizeof(char)*strlen(translations[i]->str));
			sprintf(str,translations[i]->str);
			break;
		}
	}

	if(str != NULL)
		return str;

	return "";
}


/* langfile => FR.txt */
bool TuxLang_loadTranslation(char *_langfile_)
{
	if(_langfile_ == NULL)
		return false;

	tr_count = 0;

	translations = malloc(sizeof(tux_translation)*TRANSLATIONS_NUMBER);

	char *langfile = (char *)malloc(sizeof(char)*strlen(_langfile_)+5);
	sprintf(langfile,"lang/%s",_langfile_);

	FILE *fp = fopen(langfile,"r");
	if(fp)
	{
		char buff[2048];
		while(!feof(fp))
		{
			fgets(buff,2048,fp);

			if(strcmp(buff,""))
			{
				if(strstr(buff,"="))
				{
					char **param = explode(buff,'=');

					translations[tr_count] = malloc(sizeof(tux_translation));
					translations[tr_count]->key = (char *)malloc(sizeof(char)*strlen(param[1]));
					sprintf(translations[tr_count]->key,"%s",param[0]);
					translations[tr_count]->str = (char *)malloc(sizeof(char)*strlen(param[1]));
					sprintf(translations[tr_count]->str,"%s",trim(param[1]));
					tr_count++;

					freeExplode(param);
					param = NULL;
				}
			}
		}

		fclose(fp);
		return true;
	}

	return false;
}
