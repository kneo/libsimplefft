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


CPLX_SAMPLES* alloc_samples_struct();

CPLX_SAMPLES* alloc_samples_struct(){
	CPLX_SAMPLES* res = (CPLX_SAMPLES*) calloc(1,sizeof(struct CPLX_SAMPLES));

	if(res)
		return res;
	else
		return NULL;
}

CPLX_SAMPLES* lsfft_alloc_complex_buffer(uint32_t samples, uint8_t type){
	if(type<3){
		uint32_t samp;

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

			default: return NULL;
		}

		return res;
	} return NULL;
}

void lsfft_free_complex_buffer(CPLX_SAMPLES* buffer){
	if(buffer){
		free(buffer->re);
		free(buffer->im);

		if(buffer->dimension_strides){
			free(buffer->dimension_strides);
		}

		free(buffer);
	}
}

CPLX_SAMPLES* lsfft_alloc_complex_buffer_md(uint32_t samples, uint8_t type, uint32_t dimensions) {

	if(type<5 && dimensions>0){
		uint32_t samp;
		uint32_t i;
		/*printf("allocating multi dimensional complex buffer\n");*/

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

		res->type = type;

		//assign the base length of the FFT
		res->base_length = samples;

		//assign the dimensions count of the fft ... nonegative Integer!
		res->dimension = dimensions;

		//compute polynomial coefficients of array storage access
		res->dimension_strides = calloc(dimensions,sizeof(uint32_t));

		//compute the total length of the
		res->length = 1;

		for(i=0;i<dimensions;i++){
			/*printf("Dimension %d Stride %d\n",i,res->length);*/
			res->dimension_strides[i] = res->length;
			res->length *= samp;
		}

		/*printf("Total Length %d\n",res->length);*/

		switch(type){
			case CPLX_TYPE_SP: //single precision FFT
				res->re = (void*) calloc(res->length,sizeof(float));
				res->im = (void*) calloc(res->length,sizeof(float));
				res->type_size = sizeof(float);
			break;

			case CPLX_TYPE_DP://double precision FFT
				res->re = (void*) calloc(res->length,sizeof(double));
				res->im = (void*) calloc(res->length,sizeof(double));
				res->type_size = sizeof(double);
			break;

			case CPLX_TYPE_INT://integer FFT
				res->re = (void*) calloc(res->length,sizeof(int16_t));
				res->im = (void*) calloc(res->length,sizeof(int16_t));
				res->type_size = sizeof(int16_t);
			break;

			case CPLX_TYPE_INT32://integer FFT
				res->re = (void*) calloc(res->length,sizeof(int32_t));
				res->im = (void*) calloc(res->length,sizeof(int32_t));
				res->type_size = sizeof(int32_t);
			break;

			case CPLX_TYPE_BYTE://integer FFT
				res->re = (void*) calloc(res->length,sizeof(int8_t));
				res->im = (void*) calloc(res->length,sizeof(int8_t));
				res->type_size = sizeof(int8_t);
			break;

			default: return NULL;

		}

		return res;
	} return NULL;
}

