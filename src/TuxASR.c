/* =============== GPL HEADER =====================
 * TuxASR.c is part of TuxDroidServer
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
#include <fcntl.h>
#include <sys/stat.h> 
#include <curl/curl.h>
#include <string.h>

#include <TuxASR.h>
#include <TuxStrings.h>
#include <TuxUtils.h>
#include <TuxLogger.h>
#include <TuxConfig.h>

#define READSIZE 1024

extern tuxconfig config;

char *TuxASR_getTextFromSpeech(char *_file)
{
	char *file = (char *)malloc(sizeof(char)*strlen(_file)+4);
	sprintf(file,"%s.asr",replace(_file,".flac",""));
	char *mystr=NULL;
	if(file_exists(file))
	{
		FILE *fp = fopen(file,"r");
		if(fp)
		{
			char *json=NULL;
			char buffer[4096];
			while(!feof(fp))
			{
				fgets(buffer,4096,fp);
			
				if(strcmp(buffer,""))
				{
					json = (char *)malloc(sizeof(char)*strlen(buffer));
					sprintf(json,"%s",buffer);
				}
			}
			fclose(fp);
			
			//{"status":0,"id":"18d0eea10550b7450516d0c877140b28-1","hypotheses":[{"utterance":"test du micro","confidence":0.5560999}]}
			char **sp = explode(json,'[');
			
			if(sp != NULL)
			{
				if(sp[1] != NULL)
				{
					sp = explode(sp[1],',');
				
					if(sp != NULL)
					{
						sp = explode(sp[0],':');
						
						if(sp != NULL)
							mystr = replace(sp[1],"\"","");
					}
				}
			}
			
			free(sp);
			
			if(file_exists(file))
				remove(file); /* xxx.asr */
			
			if(file_exists(replace(file,".asr",".wav")))
				remove(replace(file,".asr",".wav")); /* wav's deletion */
			
			return mystr;
		}
	}
	
	return "";
}

size_t TuxASR_writeData(void *ptr, size_t size, size_t nmemb, FILE *stream) 
{
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int TuxASR_uploadSpeech(char *file)
{
	if(file_exists(replace(file,".flac",".asr")))
		remove(replace(file,".flac",".asr"));
	
	CURL *curl;
  	CURLcode res;
 
	struct curl_httppost *formpost=NULL;
	struct curl_httppost *lastptr=NULL;
 
  	curl_global_init(CURL_GLOBAL_ALL);
 
  	/* Fill in the file upload field */ 
  	curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "sendfile",
               CURLFORM_FILE, file,
               CURLFORM_END);
 
	/* Fill in the filename field */ 
  	curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "filename",
               CURLFORM_COPYCONTENTS, file,
               CURLFORM_END);
 
 
	/* Fill in the submit field too, even if this is rarely needed */ 
  	curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "submit",
               CURLFORM_COPYCONTENTS, "send",
               CURLFORM_END);
 
	curl = curl_easy_init();

  	if(curl) 
	{
		struct curl_slist *chunk = NULL;
	    chunk = curl_slist_append(chunk, "Content-Type:audio/x-flac; rate=44100");

   		curl_easy_setopt(curl, CURLOPT_POST,1L);
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
	    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
	    
	    char *gUrl = (char *)malloc(sizeof(char)*83+strlen(config->LANG_ASR));
	    sprintf(gUrl,"http://www.google.com/speech-api/v1/recognize?xjerr=1&client=chromium&lang=%s&filter0",config->LANG_ASR);
		curl_easy_setopt(curl, CURLOPT_URL,gUrl);		
				
		char *asrfile = (char *)malloc(sizeof(char)*strlen(file)+4);
		sprintf(asrfile,"%s.asr",replace(file,".flac",""));
		FILE *fp = fopen(asrfile,"wb");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, TuxASR_writeData);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
				
 		res = curl_easy_perform(curl);
 		TuxLogger_Debug("RES => %d",res);
 		
    	curl_easy_cleanup(curl);
 
    	curl_formfree(formpost);
    	curl_slist_free_all (chunk);
    	
    	fclose(fp);
	
		return 1;
	}
	
	return 0;
}

static unsigned total_samples = 0; /* can use a 32-bit number due to WAVE size limitations */
static FLAC__byte buffer[READSIZE/*samples*/ * 2/*bytes_per_sample*/ * 2/*channels*/]; /* we read the WAVE data into here */
static FLAC__int32 pcm[READSIZE/*samples*/ * 2/*channels*/];

bool convertWavToFlac(char *input)
{
	if(file_exists(replace(input,".wav",".flac")))
		remove(replace(input,".wav",".flac"));
	
	char *output = replace(input,".wav",".flac");
	
	FLAC__bool ok = true;
	FLAC__StreamEncoder *encoder = 0;
	FLAC__StreamEncoderInitStatus init_status;
	FLAC__StreamMetadata *metadata[2];
	FLAC__StreamMetadata_VorbisComment_Entry entry;
	FILE *fin;
	unsigned sample_rate = 0;
	unsigned channels = 0;
	unsigned bps = 0;
	
	if((fin = fopen(input, "rb")) == NULL) 
	{
		TuxLogger_Error("Opening %s for output\n", input);
		return false;
	}
	
	/* read wav header and validate it */
	if(
		fread(buffer, 1, 44, fin) != 44 ||
		memcmp(buffer, "RIFF", 4) ||
		memcmp(buffer+8, "WAVEfmt \020\000\000\000\001\000\002\000", 16) ||
		memcmp(buffer+32, "\004\000\020\000data", 8)
	) 
	{
		TuxLogger_Error("Invalid/unsupported WAVE file, only 16bps stereo WAVE in canonical form allowed",NULL);
		fclose(fin);
		return false;
	}
	
	sample_rate = ((((((unsigned)buffer[27] << 8) | buffer[26]) << 8) | buffer[25]) << 8) | buffer[24];
	channels = 2;
	bps = 16;
	total_samples = (((((((unsigned)buffer[43] << 8) | buffer[42]) << 8) | buffer[41]) << 8) | buffer[40]) / 4;
   
	/* allocate the encoder */
	if((encoder = FLAC__stream_encoder_new()) == NULL) 
	{
		TuxLogger_Error("Allocating encoder",NULL);
		fclose(fin);
		return false;
	}
	
	ok &= FLAC__stream_encoder_set_verify(encoder, true);
	ok &= FLAC__stream_encoder_set_compression_level(encoder, 5);
	ok &= FLAC__stream_encoder_set_channels(encoder, channels);
	ok &= FLAC__stream_encoder_set_bits_per_sample(encoder, bps);
	ok &= FLAC__stream_encoder_set_sample_rate(encoder, sample_rate);
	ok &= FLAC__stream_encoder_set_total_samples_estimate(encoder, total_samples);
	
	/* now add some metadata; we'll add some tags and a padding block */
	if(ok) 
	{
		if(
			(metadata[0] = FLAC__metadata_object_new(FLAC__METADATA_TYPE_VORBIS_COMMENT)) == NULL ||
			(metadata[1] = FLAC__metadata_object_new(FLAC__METADATA_TYPE_PADDING)) == NULL ||
			/* there are many tag (vorbiscomment) functions but these are convenient for this particular use: */
			!FLAC__metadata_object_vorbiscomment_entry_from_name_value_pair(&entry, "ARTIST", "Some Artist") ||
			!FLAC__metadata_object_vorbiscomment_append_comment(metadata[0], entry, /*copy=*/false) || /* copy=false: let metadata object take control of entry's allocated string */
			!FLAC__metadata_object_vorbiscomment_entry_from_name_value_pair(&entry, "YEAR", "1984") ||
			!FLAC__metadata_object_vorbiscomment_append_comment(metadata[0], entry, /*copy=*/false)
		) 
		{
			/* fprintf(stderr, "ERROR: out of memory or tag error\n"); */
			TuxLogger_Error("Out of memory or tag error",NULL);
			ok = false;
		}

		metadata[1]->length = 1234; /* set the padding length */

		ok = FLAC__stream_encoder_set_metadata(encoder, metadata, 2);
	}

	/* initialize encoder */
	if(ok) 
	{
		init_status = FLAC__stream_encoder_init_file(encoder, output, NULL /* callback */, /*client_data=*/NULL);
		if(init_status != FLAC__STREAM_ENCODER_INIT_STATUS_OK) 
		{
			TuxLogger_Error("Initializing encoder: %s", FLAC__StreamEncoderInitStatusString[init_status]);
			ok = false;
		}
	}

	/* read blocks of samples from WAVE file and feed to encoder */
	if(ok) 
	{
		size_t left = (size_t)total_samples;
		while(ok && left) 
		{
			size_t need = (left>READSIZE? (size_t)READSIZE : (size_t)left);
			if(fread(buffer, channels*(bps/8), need, fin) != need) 
			{
				TuxLogger_Error("Reading from WAVE file",NULL);
				ok = false;
			}
			else 
			{
				/* convert the packed little-endian 16-bit PCM samples from WAVE into an interleaved FLAC__int32 buffer for libFLAC */
				size_t i;
				for(i = 0; i < need*channels; i++) 
				{
					/* inefficient but simple and works on big- or little-endian machines */
					pcm[i] = (FLAC__int32)(((FLAC__int16)(FLAC__int8)buffer[2*i+1] << 8) | (FLAC__int16)buffer[2*i]);
				}
				/* feed samples to encoder */
				ok = FLAC__stream_encoder_process_interleaved(encoder, pcm, need);
			}
			left -= need;
		}
	}

	ok &= FLAC__stream_encoder_finish(encoder);

	TuxLogger_Error("Encoding: %s", ok? "succeeded" : "FAILED");
	TuxLogger_Error("   state: %s\n", FLAC__StreamEncoderStateString[FLAC__stream_encoder_get_state(encoder)]);

	/* now that encoding is finished, the metadata can be freed */
	FLAC__metadata_object_delete(metadata[0]);
	FLAC__metadata_object_delete(metadata[1]);

	FLAC__stream_encoder_delete(encoder);
	fclose(fin);
	
	if(ok)
		return true;

	return false;
}
