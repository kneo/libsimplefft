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

#include "fft.h"

void fft_float(FFT_CONTEXT* context, CPLX_SAMPLES* buffer){
	if(context && buffer){
		uint32_t stages      = context->stages; //stages of the fft
		uint32_t dfts        = 1;				//each stage consists of several sub dtfs this is its initial value
		uint32_t butterflies = context->samples >> 1; //a butterfly is the elemental computation of a fft. each
													  //sub dft consists of several butterfly calculations

		uint32_t stage; //loop variables
		uint32_t dft;
		uint32_t butterfly;
		
		float* twiddle_re = (float*)context->twiddle_factors->re; //get the reference of the twiddle factors or unity roots
		float* twiddle_im = (float*)context->twiddle_factors->im; //
		
		float* re = (float*)buffer->re; //get a reference of the imaginary and real parts of the input samples
		float* im = (float*)buffer->im;
		
		//RADIX 2 DIF FFT
		
		float tmp_re; //since the first samples is required to be used in two calculations within each butterfly, its
		float tmp_im; //necessary to temporary store its original Value within these variables
		
		float diff_re; //the even dft requires a complex multiplication 
		float diff_im; //to avoid redundant computations its been stored in extra variables
		
		uint32_t pos_odd; //current position of the odd sample of the butterfly computation
		uint32_t pos_even;//current position of the even sample of the butterfly computation
		
		uint32_t stride_t = 1; //twiddle factor stride, hard to explain in short, best way is to use a sheet of paper to understand this
		
		uint32_t size_tw = context->twiddle_factors->length; //length of the twiddle factor buffer
		
		uint32_t pos_t; //position of the twiddle factor
		
		uint32_t dft_size = context->samples; //size of a sub dft
		uint32_t dft_stride;
		
		for(stage=0;stage<stages;stage++){ //for all stages
			//printf("stage->%d/%d\n",stage,stages);
			for(dft=0;dft<dfts;dft++){ //in all DFTs
				//printf("\tdft->%d/%d\n",dft,dfts);
				
				dft_stride = dft * dft_size;
				
				for(butterfly=0;butterfly<butterflies;butterfly++){ //compute n butterflies
					
					//printf("\t\tbutterfly->%d/%d\n",butterfly,butterflies);
					
					pos_odd  = dft_stride + butterfly; //update the position of the odd sample
					pos_even = pos_odd + butterflies; //update the position of the even sample
					
					pos_t = (butterfly * stride_t) % size_tw; //compute the index of the necessary twiddle factor
					
					//printf("\t\ttwiddle factor->%d -> %f + i * %f\n",pos_t,twiddle_re[pos_t],twiddle_im[pos_t]);
					
					tmp_re = re[pos_odd]; //temprorary storing the first sample as mentioned above
					tmp_im = im[pos_odd];
					
					re[pos_odd]  = tmp_re + re[pos_even]; //butterfly wing 1
					im[pos_odd]  = tmp_im + im[pos_even];
					
					diff_re = tmp_re - re[pos_even];
					diff_im = tmp_im - im[pos_even];
					
					re[pos_even] = re_mul_f(diff_re, diff_im, twiddle_re[pos_t], twiddle_im[pos_t]); //butterfly wing 2
					im[pos_even] = im_mul_f(diff_re, diff_im, twiddle_re[pos_t], twiddle_im[pos_t]);
					
					//printf("\t\tresult odd  -> %f + i * %f\n",re[pos_odd],im[pos_odd]);
					//printf("\t\tresult even -> %f + i * %f\n",re[pos_even],im[pos_even]);
					
				}
			}
			
			dfts<<=1; //update the dfts,butterflys and several further values
			butterflies>>=1;
			dft_size>>=1;
			stride_t<<=1;
		}
	}
}

void fft_double(FFT_CONTEXT* context, CPLX_SAMPLES* buffer){
	if(context && buffer){
		uint32_t stages      = context->stages; //stages of the fft
		uint32_t dfts        = 1;				//each stage consists of several sub dtfs this is its initial value
		uint32_t butterflies = context->samples >> 1; //a butterfly is the elemental computation of a fft. each
													  //sub dft consists of several butterfly calculations

		uint32_t stage; //loop variables
		uint32_t dft;
		uint32_t butterfly;
		
		double* twiddle_re = (double*)context->twiddle_factors->re; //get the reference of the twiddle factors or unity roots
		double* twiddle_im = (double*)context->twiddle_factors->im; //
		
		double* re = (double*)buffer->re; //get a reference of the imaginary and real parts of the input samples
		double* im = (double*)buffer->im;
		
		//RADIX 2 DIF FFT
		
		double tmp_re; //since the first samples is required to be used in two calculations within each butterfly, its
		double tmp_im; //necessary to temporary store its original Value within these variables
		
		double diff_re; //the even dft requires a complex multiplication 
		double diff_im; //to avoid redundant computations its been stored in extra variables
		
		uint32_t pos_odd; //current position of the odd sample of the butterfly computation
		uint32_t pos_even;//current position of the even sample of the butterfly computation
		
		uint32_t stride_t = 1; //twiddle factor stride, hard to explain in short, best way is to use a sheet of paper to understand this
		
		uint32_t size_tw = context->twiddle_factors->length; //length of the twiddle factor buffer
		
		uint32_t pos_t; //position of the twiddle factor
		
		uint32_t dft_size = context->samples; //size of a sub dft
		uint32_t dft_stride;
		
		for(stage=0;stage<stages;stage++){ //for all stages
			//printf("stage->%d/%d\n",stage,stages);
			for(dft=0;dft<dfts;dft++){ //in all DFTs
				//printf("\tdft->%d/%d\n",dft,dfts);
				
				dft_stride = dft * dft_size;
				
				for(butterfly=0;butterfly<butterflies;butterfly++){ //compute n butterflies
					
					//printf("\t\tbutterfly->%d/%d\n",butterfly,butterflies);
					
					pos_odd  = dft_stride + butterfly; //update the position of the odd sample
					pos_even = pos_odd + butterflies; //update the position of the even sample
					
					pos_t = (butterfly * stride_t) % size_tw; //compute the index of the necessary twiddle factor
					
					//printf("\t\ttwiddle factor->%d -> %f + i * %f\n",pos_t,twiddle_re[pos_t],twiddle_im[pos_t]);
					
					tmp_re = re[pos_odd]; //temprorary storing the first sample as mentioned above
					tmp_im = im[pos_odd];
					
					re[pos_odd]  = tmp_re + re[pos_even]; //butterfly wing 1
					im[pos_odd]  = tmp_im + im[pos_even];
					
					diff_re = tmp_re - re[pos_even];
					diff_im = tmp_im - im[pos_even];
					
					re[pos_even] = re_mul_d(diff_re, diff_im, twiddle_re[pos_t], twiddle_im[pos_t]); //butterfly wing 2
					im[pos_even] = im_mul_d(diff_re, diff_im, twiddle_re[pos_t], twiddle_im[pos_t]);
					
					//printf("\t\tresult odd  -> %f + i * %f\n",re[pos_odd],im[pos_odd]);
					//printf("\t\tresult even -> %f + i * %f\n",re[pos_even],im[pos_even]);
					
				}
			}
			
			dfts<<=1; //update the dfts,butterflys and several further values
			butterflies>>=1;
			dft_size>>=1;
			stride_t<<=1;
		}
	}
}

void fft_int(FFT_CONTEXT* context, CPLX_SAMPLES* buffer){
	if(context && buffer){
		uint32_t stages      = context->stages; //stages of the fft
		uint32_t dfts        = 1;				//each stage consists of several sub dtfs this is its initial value
		uint32_t butterflies = context->samples >> 1; //a butterfly is the elemental computation of a fft. each
													  //sub dft consists of several butterfly calculations

		uint32_t stage; //loop variables
		uint32_t dft;
		uint32_t butterfly;
		
		float* twiddle_re = (float*)context->twiddle_factors->re; //get the reference of the twiddle factors or unity roots
		float* twiddle_im = (float*)context->twiddle_factors->im; //
		
		int16_t* re = (int16_t*)buffer->re; //get a reference of the imaginary and real parts of the input samples
		int16_t* im = (int16_t*)buffer->im;
		
		//RADIX 2 DIF FFT
		
		int16_t tmp_re; //since the first samples is required to be used in two calculations within each butterfly, its
		int16_t tmp_im; //necessary to temporary store its original Value within these variables
		
		int16_t diff_re; //the even dft requires a complex multiplication 
		int16_t diff_im; //to avoid redundant computations its been stored in extra variables
		
		uint32_t pos_odd; //current position of the odd sample of the butterfly computation
		uint32_t pos_even;//current position of the even sample of the butterfly computation
		
		uint32_t stride_t = 1; //twiddle factor stride, hard to explain in short, best way is to use a sheet of paper to understand this
		
		uint32_t size_tw = context->twiddle_factors->length; //length of the twiddle factor buffer
		
		uint32_t pos_t; //position of the twiddle factor
		
		uint32_t dft_size = context->samples; //size of a sub dft
		uint32_t dft_stride;
		
		for(stage=0;stage<stages;stage++){ //for all stages
			//printf("stage->%d/%d\n",stage,stages);
			for(dft=0;dft<dfts;dft++){ //in all DFTs
				//printf("\tdft->%d/%d\n",dft,dfts);
				
				dft_stride = dft * dft_size;
				
				for(butterfly=0;butterfly<butterflies;butterfly++){ //compute n butterflies
					
					//printf("\t\tbutterfly->%d/%d\n",butterfly,butterflies);
					
					pos_odd  = dft_stride + butterfly; //update the position of the odd sample
					pos_even = pos_odd + butterflies; //update the position of the even sample
					
					pos_t = (butterfly * stride_t) % size_tw; //compute the index of the necessary twiddle factor
					
					//printf("\t\ttwiddle factor->%d -> %f + i * %f\n",pos_t,twiddle_re[pos_t],twiddle_im[pos_t]);
					
					tmp_re = re[pos_odd]; //temprorary storing the first sample as mentioned above
					tmp_im = im[pos_odd];
					
					re[pos_odd]  = tmp_re + re[pos_even]; //butterfly wing 1
					im[pos_odd]  = tmp_im + im[pos_even];
					
					diff_re = tmp_re - re[pos_even];
					diff_im = tmp_im - im[pos_even];
					
					re[pos_even] = re_mul_i(diff_re, diff_im, twiddle_re[pos_t], twiddle_im[pos_t]); //butterfly wing 2
					im[pos_even] = im_mul_i(diff_re, diff_im, twiddle_re[pos_t], twiddle_im[pos_t]);
					
					//printf("\t\tresult odd  -> %f + i * %f\n",re[pos_odd],im[pos_odd]);
					//printf("\t\tresult even -> %f + i * %f\n",re[pos_even],im[pos_even]);
					
				}
			}
			
			dfts<<=1; //update the dfts,butterflys and several further values
			butterflies>>=1;
			dft_size>>=1;
			stride_t<<=1;
		}
	}
}

void bit_reverse_float(FFT_CONTEXT* context, CPLX_SAMPLES* buffer){
	if(buffer){
		float* re = (float*)buffer->re;
		float* im = (float*)buffer->im;
		
		uint32_t samples = buffer->length;
		
		int i = 0;
		uint32_t index;
		
		float tmp_re,tmp_im;
		float norm = (context->mode == FFT_MODE_INVERSE) ? (1.0/ (float) samples) : 1;
		
		for(;i < buffer->length;i++){
			index = context->bit_rev_indices[i];//bit_reversal(samples,i);//
			if(index>=i){ //<- this is a god damn motherfucking trap!
				tmp_re = re[i]; //simple exchange ...
				tmp_im = im[i];
			
				re[i] = re[index] * norm;
				im[i] = im[index] * norm;
				
				re[index] = tmp_re * norm;
				im[index] = tmp_im * norm;
			}
		}
	}
}

void bit_reverse_double(FFT_CONTEXT* context, CPLX_SAMPLES* buffer){
	if(buffer){
		double* re = (double*)buffer->re;
		double* im = (double*)buffer->im;
		
		uint32_t samples = buffer->length;
		
		int i = 0;
		uint32_t index;
		
		double tmp_re,tmp_im;
		
		double norm = (context->mode == FFT_MODE_INVERSE) ? (1.0/ (float) samples) : 1;
		
		for(;i < buffer->length;i++){
			index = context->bit_rev_indices[i];//bit_reversal(samples,i);//
			if(index>=i){ //<- this is a god damn motherfucking trap!
				tmp_re = re[i]; //simple exchange ...
				tmp_im = im[i];
			
				re[i] = re[index]*norm;
				im[i] = im[index]*norm;
				
				re[index] = tmp_re*norm;
				im[index] = tmp_im*norm;
			}
		}
	}
}

void bit_reverse_int(FFT_CONTEXT* context, CPLX_SAMPLES* buffer){
	if(buffer){
		int16_t* re = (int16_t*)buffer->re;
		int16_t* im = (int16_t*)buffer->im;
		
		uint32_t samples = buffer->length;
		
		int i = 0;
		uint32_t index;
		
		int16_t tmp_re,tmp_im;
		
		
		//since we iterating throught this array anyways, we can normalise it in case
		//of the iFFT as well. Saves some redundant loop calls though
		float norm = (context->mode == FFT_MODE_INVERSE) ? (1.0/ (float) samples) : 1;
		
		for(;i < buffer->length;i++){
			index = context->bit_rev_indices[i];//bit_reversal(samples,i);//
			if(index>=i){ //<- this is a god damn motherfucking trap!			
				tmp_re = re[i]; //simple exchange ...
				tmp_im = im[i];
			
				re[i] = re[index]*norm;
				im[i] = im[index]*norm;
				
				re[index] = tmp_re*norm;
				im[index] = tmp_im*norm;
			}
		}
	}
}

void lsfft_perform(FFT_CONTEXT* context, CPLX_SAMPLES* buffer){
	if(context && buffer){
		printf("context type:%d size:%d mode:%d \n", context->type,context->samples,context->mode);
	
		if(context->samples == buffer->length){
			if(context->mode==FFT_MODE_NORMAL){
				switch(context->type){
					case CPLX_TYPE_SP:
						printf("performing fft with %d samples single precision\n",context->samples);
						//printf("clocks %d\n",(int) clock());
						buffer->exec_time = clock();
						fft_float(context,buffer);
						bit_reverse_float(context,buffer);
						buffer->exec_time = clock() - buffer->exec_time;
					break;
					
					case CPLX_TYPE_DP:
						//printf("performing fft with %d samples double precision\n",context->samples);
						//printf("clocks %d\n",(int) clock());
						buffer->exec_time = clock();
						fft_double(context,buffer);
						bit_reverse_double(context,buffer);
						buffer->exec_time = clock() - buffer->exec_time;
					break;
					
					case CPLX_TYPE_INT:
						//printf("performing fft with %d samples double precision\n",context->samples);
						//printf("clocks %d\n",(int) clock());
						buffer->exec_time = clock();
						fft_int(context,buffer);
						bit_reverse_int(context,buffer);
						buffer->exec_time = clock() - buffer->exec_time;
					break;
				}
			} else {
				switch(context->type){
					case CPLX_TYPE_SP:
						printf("performing ifft with %d samples single precision\n",context->samples);
						//printf("clocks %d\n",(int) clock());
						buffer->exec_time = clock();
						fft_float(context,buffer);
						bit_reverse_float(context,buffer);
						buffer->exec_time = clock() - buffer->exec_time;
					break;
					
					case CPLX_TYPE_DP:
						//printf("performing fft with %d samples double precision\n",context->samples);
						//printf("clocks %d\n",(int) clock());
						buffer->exec_time = clock();
						fft_double(context,buffer);
						bit_reverse_double(context,buffer);
						buffer->exec_time = clock() - buffer->exec_time;
					break;
					
					case CPLX_TYPE_INT:
						//printf("performing fft with %d samples double precision\n",context->samples);
						//printf("clocks %d\n",(int) clock());
						buffer->exec_time = clock();
						fft_int(context,buffer);
						bit_reverse_int(context,buffer);
						buffer->exec_time = clock() - buffer->exec_time;
					break;
				}
			}			
		} else {
			
		}	
	}
}
