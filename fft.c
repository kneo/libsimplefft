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
		
		for(stage=0;stage<stages;stage++){ //for all stages
			//printf("stage->%d/%d\n",stage,stages);
			for(dft=0;dft<dfts;dft++){ //in all DFTs
				//printf("\tdft->%d/%d\n",dft,dfts);
				for(butterfly=0;butterfly<butterflies;butterfly++){ //compute n butterflies
					
					//printf("\t\tbutterfly->%d/%d\n",butterfly,butterflies);
					
					pos_odd  = dft*dft_size + butterfly; //update the position of the odd sample
					pos_even = pos_odd + butterflies; //update the position of the even sample
					
					pos_t = butterfly * stride_t % size_tw; //compute the index of the necessary twiddle factor
					
					//printf("\t\ttwiddle factor->%d -> %f + i * %f\n",pos_t,twiddle_re[pos_t],twiddle_im[pos_t]);
					
					tmp_re = re[pos_odd]; //temprorary storing the first sample as mentioned above
					tmp_im = im[pos_odd];
					
					re[pos_odd]  = tmp_re + re[pos_even]; //butterfly wing 1
					im[pos_odd]  = tmp_im + im[pos_even];
					
					diff_re = tmp_re-re[pos_even];
					diff_im = tmp_im-im[pos_even];
					
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


void bit_reverse_float(CPLX_SAMPLES* buffer){
	if(buffer){
		float* re = (float*)buffer->re;
		float* im = (float*)buffer->im;
		
		uint32_t samples = buffer->length;
		
		int i = 0;
		uint32_t index;
		
		float tmp_re,tmp_im;
		
		for(;i < buffer->length/2;i++){
			index = bit_reversal(samples,i);
			if(index>i){ //<- this is a god damn motherfucking trap!
			
				tmp_re = re[i]; //simple exchange ...
				tmp_im = im[i];
			
				re[i] = re[index];
				im[i] = im[index];
				
				re[index] = tmp_re;
				im[index] = tmp_im;
			}
		}
	}
}

void bit_reverse_double(CPLX_SAMPLES* buffer){
	
}

void bit_reverse_int(CPLX_SAMPLES* buffer){
	
}

void lsfft_perform(FFT_CONTEXT* context, CPLX_SAMPLES* buffer){
	if(context && buffer){
		if(context->samples == buffer->length){
			if(context->mode==FFT_MODE_NORMAL){
				switch(context->type){
					case CPLX_TYPE_SP:
						printf("performing fft with %d samples single precision\n",context->samples);
						//printf("clocks %d\n",(int) clock());
						buffer->exec_time = clock();
						fft_float(context,buffer);
						bit_reverse_float(buffer);
						buffer->exec_time = clock() - buffer->exec_time;
					break;
					
					case CPLX_TYPE_DP:
					
					break;
					
					case CPLX_TYPE_INT:
					
					break;
				}
			} else {
				
			
			}
		

			
		} else {
			
		}	
	}
}
