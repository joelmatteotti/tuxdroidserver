/* =============== GPL HEADER =====================
 * TuxLogger.c is part of TuxDroidServer
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
#include <stdarg.h>
#include <time.h>
#include <malloc.h>
#include <TuxLogger.h>
#include <TuxVersion.h>
#include <TuxUtils.h>
#include <TuxStrings.h>
#ifndef _WIN32
	#include <sys/stat.h>
#endif


TUX_LOG_LEVEL LOG_LEVEL = TUX_LOG_INFO;

void TuxLogger_checkLogDir()
{
	if(!directory_exists("./logs"))
	{
		#ifdef _WIN32
		mkdir("logs");
		#else
		mkdir("logs",0755);
		#endif
	}
}

void TuxLogger_setLevel(TUX_LOG_LEVEL log_level)
{
     LOG_LEVEL = log_level;
}

void TuxLogger_writeLog(TUX_LOG_LEVEL log_level, const char *fmt, ...)
{
	if(!(log_level <= LOG_LEVEL))
		return;

	TuxLogger_checkLogDir();

	va_list al;
	char log[2048];
	va_start(al, fmt);
	vsnprintf(log,2048,fmt,al);
	va_end(al);

	time_t rawtime;
	struct tm * timeinfo;
	char buffer [10];

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	strftime (buffer,10,"%H:%M:%S",timeinfo);

	char str_level[16];
	switch(log_level)
	{
		case TUX_LOG_INFO: sprintf(str_level,"INFO");
		break;
		case TUX_LOG_WARN: sprintf(str_level,"WARNING");
		break;
		case TUX_LOG_ERROR: sprintf(str_level,"ERROR");
		break;
		case TUX_LOG_DEBUG: sprintf(str_level,"DEBUG");
	}

	char log_text[sizeof(log)+sizeof(buffer)+8];
	sprintf(log_text,"[%s] - [%s] %s\n",buffer,str_level,trim(log));

	strftime (buffer,10,"%d_%m_%y",timeinfo);
	char logfile[25];
	sprintf(logfile,"logs/%s.log",buffer);

	FILE *fp = fopen(logfile,"a+");
	if(fp)
	{
		fputs(log_text,fp);
		fclose(fp);
	}
}
