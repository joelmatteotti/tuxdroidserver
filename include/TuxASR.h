/* =============== GPL HEADER =====================
 * TuxASR.h is part of TuxDroidServer
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

#ifndef __TUXASR_H__
#define __TUXASR_H__

#include <stdio.h>
#include <stdbool.h>

#include <FLAC/metadata.h>
#include <FLAC/stream_encoder.h>

char *TuxASR_getTextFromSpeech(char *_file);
size_t TuxASR_writeData(void *ptr, size_t size, size_t nmemb, FILE *stream);
int TuxASR_uploadSpeech(char *file);
bool convertWavToFlac(char *input);

#endif
