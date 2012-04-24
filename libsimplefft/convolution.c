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
#include"convolution.h"

CONVOLUTION_CONTEXT* lsfft_init_convolution(CPLX_SAMPLES* kernel){
	if(kernel){
	
		//init fft of type of kernel
		FFT_CONTEXT* context = lsfft_init(kernel->length,kernel->type,FFT_MODE_NORMAL);
		FFT_CONTEXT* icontext = lsfft_init(kernel->length,kernel->type,FFT_MODE_INVERSE);
		
		if(!context || !icontext){
			return NULL;
		}
		
		//calculate fft of kernel in place
		lsfft_perform(context,kernel);
	
		CONVOLUTION_CONTEXT* res = (CONVOLUTION_CONTEXT*) calloc(1,sizeof(CONVOLUTION_CONTEXT));

		res->type          = kernel->type;
		res->fft_context   = context;
		res->ifft_contexxt = icontext;
		res->samples       = kernel->length;
		res->kernel        = kernel;
	
		//return context		
		return res;
	}

	return NULL;
}

void perform_convolution(CONVOLUTION_CONTEXT* context, CPLX_SAMPLES* signal){
	if(context && signal){
		//compute the fft of the signal
		lsfft_perform(context->fft_context,signal);
		
		//integer pointers
		int16_t* kre = (int16_t*) context->kernel->re;
		int16_t* kim = (int16_t*) context->kernel->im;
		int16_t* sre = (int16_t*) signal->re;
		int16_t* sim = (int16_t*) signal->im;
				
		//float pointers
		float* kre = (float*) context->kernel->re;
		float* kim = (float*) context->kernel->im;
		float* sre = (float*) signal->re;
		float* sim = (float*) signal->im;
					
		//double pointers
		double* kim = (double*) context->kernel->im;
		double* kre = (double*) context->kernel->re;
		double* sre = (double*) signal->re;
		double* sim = (double*) signal->im;
		
		//point wise multiplication
		int i = 0;
		switch(context->type){
			case CPLX_TYPE_INT:			
				for(;i<context->samples;i++){
					sre[i] = re_mul_i(sre[i],sim[i],kre[i],kim[i]);
					sim[i] = im_mul_i(sre[i],sim[i],kre[i],kim[i]);
				}
			break;
			
			case CPLX_TYPE_SP:
				for(;i<context->samples;i++){
					sre[i] = re_mul_f(sre[i],sim[i],kre[i],kim[i]);
					sim[i] = im_mul_f(sre[i],sim[i],kre[i],kim[i]);
				}
			break;
			
			case CPLX_TYPE_DP:
				for(;i<context->samples;i++){					
					sre[i] = re_mul_d(sre[i],sim[i],kre[i],kim[i]);
					sim[i] = im_mul_d(sre[i],sim[i],kre[i],kim[i]);
				}
			break;
		}
		//perform the ifft of the convoluted signal		
		lsfft_perform(context->ifft_context,signal);
	}
}

void destroy_covolution_context(CONVOLUTION_CONTEXT* context){
	if(context){
		if(context->fft_context){
			//destroy the fft context
			lsfft_destroy_context(context->fft_context);
		}
		
		if(context->ifft_context){
			//revert kernel but no freeing!
			if(context->kernel){
				lsfft_perform(context->ifft_context,context->kernel);
			}
			//destroy the ifft context
			lsfft_destroy_context(context->fft_context);
		}
		//free structure		
		free(context);
	}	
}
