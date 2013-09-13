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
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with libsimplefft; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "libsimplefft.h"

FFT_CONTEXT* lsfft_init(uint32_t samples, uint8_t type, uint8_t mode){

	FFT_CONTEXT* res_context = NULL;

	uint32_t samp = 0;

	if(samples>2) { //at least 2 samples required to perform a fft
		if(bitcount(samples)==1) { //samples is power of 2
			samp = samples;

			//printf("samples is a power of 2!\n");
		} else { //samples is not a power of 2 find its next power of to and pad nonexisting samples with 0
			int exp = log_2(samples);
			samp = exp_2(exp+1);
		}
	} else {
		return NULL;
	}

	res_context = (FFT_CONTEXT*) calloc(1,sizeof(FFT_CONTEXT));

	if(!res_context) goto error;

	res_context->samples = samp;

	res_context->stages = log_2(samp);

	res_context->bit_rev_indices = (uint32_t*) calloc(samp,sizeof(uint32_t));

	if(!res_context->bit_rev_indices) goto error;

	uint32_t i = 0;

	for(;i<samp;i++) { //initialize look up table for bit reversed indices
		res_context->bit_rev_indices[i] = bit_reversal(samp,i);
	}

	if(type<5) { //determine the data type of the FFT
		res_context->type = type;
		res_context->twiddle_factors = compute_twiddles(samp,type,mode);

		if(!res_context->twiddle_factors) goto error;

	} else {
		 goto error;
	}
	//minor modification to support multidimensional FFTs to reuse this code
	if((mode&1)>=0 && (mode&1)<2) { //determines the mode of the FFT the normal or its inverse
		res_context->mode = mode;
	} else {
		 goto error;
	}

	return res_context;

	error:
		if(res_context){
			if(res_context->twiddle_factors){
				free(res_context->twiddle_factors);
			}

			if(res_context->bit_rev_indices){
				free(res_context->bit_rev_indices);
			}

			free(res_context);
		}
		return NULL;
}

void lsfft_destroy_context(FFT_CONTEXT* context){
	if(context){
		lsfft_free_complex_buffer(context->twiddle_factors);
		free(context->bit_rev_indices);
		free(context);
	}
}
