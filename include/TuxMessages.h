/* =============== GPL HEADER =====================
 * TuxMessages.h is part of TuxDroidServer
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

/* This file contains various messages sent to the client by the server */

#ifndef __TUXMESSAGES_H__
#define __TUXMESSAGES_H__

#define TDSM_DONGLE_CONNECTED 			"#TUXDONGLE:CONNECTED"
#define TDSM_DONGLE_DISCONNECTED		"#TUXDONGLE:DISCONNECTED"

#define TDSM_CHARGER_PLUGGED			"#TUXCHARGER:PLUGGED"
#define TDSM_CHARGER_UNPLUGGED			"#TUXCHARGER:UNPLUGGED"

#define TDSM_LEFTBUTTON_CLICKED			"#TUXBUTTON:LEFT"
#define TDSM_LEFTBUTTON_RELEASED		"#TUXBUTTON:LEFT_RELEASED"
#define TDSM_RIGHTBUTTON_CLICKED		"#TUXBUTTON:RIGHT"
#define TDSM_RIGHTBUTTON_RELEASED		"#TUXBUTTON:RIGHT_RELEASED"
#define TDSM_HEADBUTTON_CLICKED			"#TUXBUTTON:HEAD"
#define TDSM_HEADBUTTON_RELEASED		"#TUXBUTTON:HEAD_RELEASED"

#define TDSM_REMOTEBUTTON_CLICKED		"#TUXREMOTE:%s"			/* Only contains button's name */
#define TDSM_REMOTEBUTTONMULTI_CLICKED	"#TUXREMOTE:%s,%s" 		/* Contains button's name + username */
#define TDSM_REMOTEBUTTON_RELEASE		"#TUXREMOTE:RELEASE"

#define TDSM_PRIORITY_LOCKED			"#TUXPRIORITY:LOCKED"
#define TDSM_PRIORITY_UNLOCKED			"#TUXPRIORITY:UNLOCKED"
#define TDSM_PRIORITY_NOIDSET			"#TUXPRIORITY:NOIDSET"
#define TDSM_PRIORITY_IDNOTUNIQUE		"#TUXPRIORITY:IDNOTUNIQUE"

#define TDSM_FLACCONVERT_FINISHED		"#TUXDROIDSERVER:FLACFINISH"
#define TDSM_FLACCONVERT_FAILED			"#TUXDROIDSERVER:FLACFAIL"

#define TDSM_ASRRESULT					"#TUXASRRESULT:%s"

#define TDSM_DOWNLOADFAILED				"#TUXDOWNLOADER:%s"		/* error code for downloaded (or not) files */

#define TDSM_INVALIDKEY					"#TUXDROIDSERVER:INVALIDKEY"
#define TDSM_VALIDEDKEY					"#TUXDROIDSERVER:VALIDEDKEY"

#endif
