/*
*This is a part of libsimplefft
*
* Copyright (C) 2012  Kevin Krüger (kkevin@gmx.net)
* 
* libsimplefft is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
* 
* libsimplefft is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
* 
* You should have received a copy of the GNU Lesser General Public
* License along with libsimplefft; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "alloc.h"

CPLX_SAMPLES* alloc_samples_struct(){
	CPLX_SAMPLES* res = (CPLX_SAMPLES*) calloc(1,sizeof(struct CPLX_SAMPLES));

	if(res)
		return res;
	else
		return NULL;
}

CPLX_SAMPLES* lsfft_alloc_complex_buffer(uint32_t samples, uint8_t type){
	if(type>-1 && type<3){
		unsigned int samp;
		
		//allocate 2^n samples
		//if 2 is not the nth root of samples
		//generate a buffer with 2^(log_2(samples)+1) samples
		
		if(samples % 2 == 0 && bitcount(samples)==1){
			samp = samples;
		} else {
			samp = exp_2(log_2(samples)+1);
		}
		
		//allocate the basic complex sample buffer structure
		CPLX_SAMPLES* res = alloc_samples_struct();
		res->type   = type;
		res->length = samp;
		
		//distinguish the type of the fft, and allocate memory accordingly to the type of the fft
		
		switch(type){
			case CPLX_TYPE_SP: //single precision FFT
				res->re = (void*) calloc(samp,sizeof(float));
				res->im = (void*) calloc(samp,sizeof(float));
			break;
			
			case CPLX_TYPE_DP://double precision FFT
				res->re = (void*) calloc(samp,sizeof(double));
				res->im = (void*) calloc(samp,sizeof(double));
			break;
			
			case CPLX_TYPE_INT://integer FFT
				res->re = (void*) calloc(samp,sizeof(int16_t));
				res->im = (void*) calloc(samp,sizeof(int16_t));
			break;
		}
		
		return res;
	} return NULL;
}

void lsfft_free_complex_buffer(CPLX_SAMPLES* buffer){
	if(buffer){
		free(buffer->re);
		free(buffer->im);
		free(buffer);
	}
}

CPLX_SAMPLES* lsfft_alloc_complex_buffer_md(uint32_t samples, uint8_t type, uint32_t dimensions) {

	if(type>-1 && type<3){
		unsigned int samp;
		
		//allocate 2^n samples
		//if 2 is not the nth root of samples
		//generate a buffer with 2^(log_2(samples)+1) samples
		
		if(samples % 2 == 0 && bitcount(samples)==1){
			samp = samples;
		} else {
			samp = exp_2(log_2(samples)+1);
		}
		
		//allocate the basic complex sample buffer structure
		CPLX_SAMPLES* res = alloc_samples_struct();



	}
}

