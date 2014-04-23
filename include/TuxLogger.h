/* =============== GPL HEADER =====================
 * TuxLogger.h is part of TuxDroidServer
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

#ifndef __TUXLOGGER_H__
#define __TUXLOGGER_H__

#include <stdbool.h>

typedef enum
{
	TUX_LOG_INFO=0,
	TUX_LOG_WARN=1,
	TUX_LOG_ERROR=2,
	TUX_LOG_DEBUG=3,
} TUX_LOG_LEVEL;

void TuxLogger_setLevel(TUX_LOG_LEVEL log_level);
void TuxLogger_checkLogDir(void);
void TuxLogger_writeLog(TUX_LOG_LEVEL log_level, const char *fmt, ...);

#define TuxLogger_Info(fmt, ...) TuxLogger_writeLog(TUX_LOG_INFO, (fmt), ## __VA_ARGS__)
#define TuxLogger_Warning(fmt, ...) TuxLogger_writeLog(TUX_LOG_WARN, (fmt), ## __VA_ARGS__)
#define TuxLogger_Error(fmt, ...) TuxLogger_writeLog(TUX_LOG_ERROR, (fmt), ## __VA_ARGS__)
#define TuxLogger_Debug(fmt, ...) TuxLogger_writeLog(TUX_LOG_DEBUG, (fmt), ## __VA_ARGS__)


#endif
