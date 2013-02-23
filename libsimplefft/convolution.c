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
		FFT_CONTEXT* context  = lsfft_init(kernel->length,kernel->type,FFT_MODE_NORMAL);
		FFT_CONTEXT* icontext = lsfft_init(kernel->length,kernel->type,FFT_MODE_INVERSE);
		
		if(!context || !icontext){
			return NULL;
		}
		
		//calculate fft of kernel in place
		lsfft_perform(context,kernel);
	
		CONVOLUTION_CONTEXT* res = (CONVOLUTION_CONTEXT*) calloc(1,sizeof(CONVOLUTION_CONTEXT));

		res->type          = kernel->type;
		res->fft_context   = context;
		res->ifft_context  = icontext;
		res->samples       = kernel->length;
		res->kernel        = kernel;
	
		//return context		
		return res;
	}

	return NULL;
}

//function for reusing fft contexts
CONVOLUTION_CONTEXT* lsfft_init_convolution_using_fft_context(FFT_CONTEXT* fft, FFT_CONTEXT* ifft, CPLX_SAMPLES* kernel){
	if(kernel){
		FFT_CONTEXT* context;
		FFT_CONTEXT* icontext;
		
		if(!fft){
			context  = lsfft_init(kernel->length,kernel->type,FFT_MODE_NORMAL);
		}
		else{
			context  = fft;
		}
		
		if(!ifft){
			icontext = lsfft_init(kernel->length,kernel->type,FFT_MODE_INVERSE);
		}
		else{
			icontext = ifft;
		}
		
		
		if(!context || !icontext){
			return NULL;
		}
		
		//calculate fft of kernel in place
		lsfft_perform(context,kernel);
	
		CONVOLUTION_CONTEXT* res = (CONVOLUTION_CONTEXT*) calloc(1,sizeof(CONVOLUTION_CONTEXT));

		res->type          = kernel->type;
		res->fft_context   = context;
		res->ifft_context  = icontext;
		res->samples       = kernel->length;
		res->kernel        = kernel;
	
		//return context		
		return res;
	}

	return NULL;
}



void lsfft_perform_convolution(CONVOLUTION_CONTEXT* context, CPLX_SAMPLES* signal){
	if(context && signal){
		//compute the fft of the signal
		lsfft_perform(context->fft_context,signal);
		
		//integer pointers
		int16_t* ikre = (int16_t*) context->kernel->re;
		int16_t* ikim = (int16_t*) context->kernel->im;
		int16_t* isre = (int16_t*) signal->re;
		int16_t* isim = (int16_t*) signal->im;
				
		//float pointers
		float* fkre = (float*) context->kernel->re;
		float* fkim = (float*) context->kernel->im;
		float* fsre = (float*) signal->re;
		float* fsim = (float*) signal->im;
					
		//double pointers
		double* dkim = (double*) context->kernel->im;
		double* dkre = (double*) context->kernel->re;
		double* dsre = (double*) signal->re;
		double* dsim = (double*) signal->im;
		
		//point wise multiplication
		int i = 0;
		switch(context->type){
			case CPLX_TYPE_INT:
				for(;i<context->samples;i++){
					int16_t itmp = isre[i];
					isre[i] = re_mul_i(isre[i],isim[i],ikre[i],ikim[i]);
					isim[i] = im_mul_i(itmp,isim[i],ikre[i],ikim[i]);
				}
			break;
			
			case CPLX_TYPE_SP:
				for(;i<context->samples;i++){
					float ftmp = fsre[i];
					fsre[i] = re_mul_f(fsre[i],fsim[i],fkre[i],fkim[i]);
					fsim[i] = im_mul_f(ftmp,fsim[i],fkre[i],fkim[i]);
				}
			break;
			
			case CPLX_TYPE_DP:
				for(;i<context->samples;i++){
					double dtmp = dsre[i];
					dsre[i] = re_mul_d(dsre[i],dsim[i],dkre[i],dkim[i]);
					dsim[i] = im_mul_d(dtmp,dsim[i],dkre[i],dkim[i]);
				}
			break;
		}
		//perform the ifft of the convoluted signal
		lsfft_perform(context->ifft_context,signal);
	}
}

void lsfft_destroy_covolution_context(CONVOLUTION_CONTEXT* context){
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
