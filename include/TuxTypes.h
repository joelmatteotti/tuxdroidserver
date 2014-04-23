/* =============== GPL HEADER =====================
 * TuxTypes.h is part of TuxDroidServer
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

#ifndef __TUXTYPES_H__
#define __TUXTYPES_H__

#include <stdio.h>

#ifndef NULL
	#define NULL 0x00
#endif

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
	#ifndef _WS2TCPIP_H
	typedef size_t socklen_t;
	#endif
#else
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
typedef unsigned short u_short;
#endif

/*=====================================================*/


/*
-----------------------------
 FROM tux_leds.h
-----------------------------
*/

/**
 * Type indicating which led should be affected by the command.
 * The left LED is affected to bit0 and the right LED is at bit1. This
 * simplifies comparisons. Assigning an hex value helps keep in mind the bit
 * relation.
 */
typedef enum
{
	LED_NONE = 0,
	LED_LEFT = 0x01,
	LED_RIGHT = 0x02,
	LED_BOTH = 0x03,
} leds_t;

/** Types of effects applied when changing the intensity of the LEDs. */
typedef enum
{
	UNAFFECTED,     /**< Don't update the effect parameters. This can either be
                      the last effect set by software, or by firmware in the
                      autonomous mode. This is probably not what you want. */
	LAST,           /**< Last effect requested by software. */
	NONE,           /**< Don't use effects, equivalent to on/off mode. */
	DEFAULT,        /**< Default effect which is a short fading effect. */
	FADE_DURATION,  /**< Fading effect, 'effect.speed' sets the duration (in
                      seconds) the effect will last. */
	FADE_RATE,      /**< Fading effect, 'effect.speed' sets the rate of the
                      effect. Its value represents the number of seconds it
                      takes to apply the effect from off to on. So the actual
                      effect duration will take less time than specified if the
                      intensity starts or ends at intermediate values.
                      Therefore this parameter guarantees a constant rate of
                      the effect, not the duration.
                      */
	GRADIENT_NBR,   /**< Gradient effect, the intensity changes gradually by a
                      number of steps given by 'effect.step'. 'effect.speed'
                      represents the number of seconds it should take to apply
                      the effect. */
	GRADIENT_DELTA, /**< Gradient effect, the intensity changes by a delta
                      value of 'effect.step'. 'effect.speed' represents the
                      number of seconds it should take to apply the effect. */
} effect_type_t;

/*============================================================*/


/* Client structure */
typedef struct
{
	int id;
	char *username;  /* user name */
	char *uKey;  /* user key */
	char *pID;  /* client application's ID */

	#ifdef _WIN32
	SOCKET sock;
	#else
	int sock;
	#endif
} tux_client_t;
typedef tux_client_t *tux_client;

/* Structure to synchronize mouth movements with TTS playing */
typedef struct
{
	char *phr;
	int count;
} syncMouthMov_t;
typedef syncMouthMov_t *syncMouthMov;


#endif
