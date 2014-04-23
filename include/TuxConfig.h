/* =============== GPL HEADER =====================
 * TuxConfig.h is part of TuxDroidServer
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

#ifndef __TUXCONFIG_H__
#define __TUXCONFIG_H__

#include <TuxLogger.h>
#include <stdbool.h>



/* Config's structure for TuxDroidServer*/
typedef struct
{
	int SERVER_PORT;			/* Listening port */
	char *USER_KEY;				/* user key */
	TUX_LOG_LEVEL LOG_LEVEL;	/* Log level */
	char *LANG_FILE;			/* Translation's file */
	char *LANG_ASR;				/* ASR's language */
	int AUTO_START;				/* Autostart */
	bool ASR_ENABLED;
} tuxconfig_t;
typedef tuxconfig_t *tuxconfig;

void TuxConfig_checkConfigDir(void);
void TuxConfig_writeConfig(void);
void TuxConfig_loadConfig(void);


#endif
