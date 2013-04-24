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
			if(index>=i){ 
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
			if(index>=i){ 
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
			if(index>=i){
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



//------TODO: 1. bit reversing using array strides!------
//TODO: 2. fft using array strides!
//TODO: 3. Parallelizing multi dimensional FFTs

void bit_reverse_md(FFT_CONTEXT* context, CPLX_SAMPLES* buffer, uint32_t* memory_vector, uint32_t axis){
	uint32_t samples = buffer->length;
	uint32_t index;

	int16_t tmp_re,tmp_im,norm=1;

	uint32_t memory_from[buffer->dimension];
	uint32_t memory_to[buffer->dimension];
	//initialize vectors

	memcpy(memory_from, memory_vector,sizeof(memory_from));
	memcpy(memory_to, memory_vector,sizeof(memory_to));

	uint8_t* re_8t = (uint8_t*)buffer->re;
	uint8_t* im_8t = (uint8_t*)buffer->im;

	uint16_t* re_16t = (uint16_t*)buffer->re;
	uint16_t* im_16t = (uint16_t*)buffer->im;

	float* re_ft = (float*)buffer->re;
	float* im_ft = (float*)buffer->im;

	double* re_dt = (double*)buffer->re;
	double* im_dt = (double*)buffer->im;

	int8_t tmp_re_8t,tmp_im_8t;
	int16_t tmp_re_16t,tmp_im_16t;

	float tmp_re_ft,tmp_im_ft;
	double tmp_re_dt,tmp_im_dt;

	uint32_t from_index;
	uint32_t to_index;
	uint32_t i;
	
	for(i=0;i<buffer->base_length / 2;i++){
		//printf("i:%d\n",i);
		memory_from[axis] = i;
		memory_to[axis]   = context->bit_rev_indices[i];

		//printf("getting linear indices ...");
		from_index = get_memory_index(memory_from, buffer->dimension_strides, buffer->dimension);	//compute the strided indices	
		to_index   = get_memory_index(memory_to,   buffer->dimension_strides, buffer->dimension);   //compute the stridet indices of the target
		//printf("done!\n");

		//printf("from %d -> %d to %d -> %d\n",memory_from[axis],from_index,memory_to[axis],to_index);
		if(memory_to[axis] >= memory_from[axis]){
			switch(context->type){
				case CPLX_TYPE_BYTE:
					tmp_re_8t = re_8t[from_index]; //simple exchange ...
					tmp_im_8t = im_8t[from_index];
			
					re_8t[from_index] = re_8t[to_index]*norm;
					im_8t[from_index] = im_8t[to_index]*norm;
				
					re_8t[to_index] = tmp_re_8t*norm;
					im_8t[to_index] = tmp_im_8t*norm;
				break;

				case CPLX_TYPE_INT:
					tmp_re_16t = re_16t[from_index]; //simple exchange ...
					tmp_im_16t = im_16t[from_index];
			
					re_16t[from_index] = re_16t[to_index]*norm;
					im_16t[from_index] = im_16t[to_index]*norm;
				
					re_16t[to_index] = tmp_re_16t*norm;
					im_16t[to_index] = tmp_im_16t*norm;
				break;

				case CPLX_TYPE_SP:
					tmp_re_ft = re_ft[from_index]; //simple exchange ...
					tmp_im_ft = im_ft[from_index];
			
					re_ft[from_index] = re_ft[to_index]*norm;
					im_ft[from_index] = im_ft[to_index]*norm;
				
					re_ft[to_index] = tmp_re_ft*norm;
					im_ft[to_index] = tmp_im_ft*norm;
				break;

				case CPLX_TYPE_DP:
					tmp_re_dt = re_dt[from_index]; //simple exchange ...
					tmp_im_dt = im_dt[from_index];
			
					re_dt[from_index] = re_dt[to_index]*norm;
					im_dt[from_index] = im_dt[to_index]*norm;
				
					re_dt[to_index] = tmp_re_dt*norm;
					im_dt[to_index] = tmp_im_dt*norm;
				break;
			}

		}
	}
}

void fft_md(FFT_CONTEXT* context, CPLX_SAMPLES* buffer, uint32_t axis,uint32_t* memory_vector){
	if(context && buffer && (context->mode & FFT_MODE_MD)){
		uint32_t stages      = context->stages; //stages of the fft
		uint32_t dfts        = 1;				//each stage consists of several sub dtfs this is its initial value
		uint32_t butterflies = context->samples >> 1; //a butterfly is the elemental computation of a fft. each
													  //sub dft consists of several butterfly calculations
		uint32_t stage; //loop variables
		uint32_t dft;
		uint32_t butterfly;

		uint32_t odd_vector[buffer->dimension];
		uint32_t even_vector[buffer->dimension];

		int8_t* re_8t;
		int8_t* im_8t;

		int16_t* re_16t;
		int16_t* im_16t;

		float *re_ft, *twiddle_re_ft;
		float *im_ft, *twiddle_im_ft;

		double *re_dt,*twiddle_re_dt;
		double *im_dt,*twiddle_im_dt;

		int8_t tmp_re_8t,tmp_im_8t,diff_re_8t,diff_im_8t;
		int16_t tmp_re_16t,tmp_im_16t,diff_re_16t,diff_im_16t;

		float tmp_re_ft,tmp_im_ft,diff_re_ft,diff_im_ft;
		double tmp_re_dt,tmp_im_dt,diff_re_dt,diff_im_dt;

		int16_t* re = (int16_t*)buffer->re; //get a reference of the imaginary and real parts of the input samples
		int16_t* im = (int16_t*)buffer->im;
		
		//RADIX 2 DIF FFT
		
		int16_t tmp_re; //since the first sample is required to be used in two calculations within each butterfly, its
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

		memcpy(odd_vector,  memory_vector, sizeof(odd_vector));
		memcpy(even_vector, memory_vector, sizeof(even_vector));

		/*
		int32_t i;
		printf("odd [");
		for(i=0;i<buffer->dimension;i++){
			printf("%d ",odd_vector[i]);
		}
		printf("]\n");

		printf("even [");
		for(i=0;i<buffer->dimension;i++){
			printf("%d ",even_vector[i]);
		}
		printf("]\n");
		*/	

		re_8t = (int8_t*)buffer->re;
		im_8t = (int8_t*)buffer->im;

		re_16t = (int16_t*)buffer->re;
		im_16t = (int16_t*)buffer->im;

		re_ft = (float*)buffer->re;
		im_ft = (float*)buffer->im;

		re_dt = (double*)buffer->re;
		im_dt = (double*)buffer->im;

		twiddle_re_dt = (double*) (context->twiddle_factors->re);
		twiddle_im_dt = (double*) (context->twiddle_factors->im);

		twiddle_re_ft = (float*) (context->twiddle_factors->re);
		twiddle_im_ft = (float*) (context->twiddle_factors->im);

		for(stage=0;stage<stages;stage++){ //for all stages
			//printf("stage->%d/%d\n",stage,stages);
			for(dft=0;dft<dfts;dft++){ //in all DFTs
				//printf("\tdft->%d/%d\n",dft,dfts);
				
				dft_stride = dft * dft_size;
				
				for(butterfly=0;butterfly<butterflies;butterfly++){ //compute n butterflies
					
					//printf("\t\tbutterfly->%d/%d\n",butterfly,butterflies);
					//load effective address of stride and multi dimensional strides

					odd_vector[axis]  = dft_stride + butterfly; //update the position of the odd sample
					even_vector[axis] = odd_vector[axis] + butterflies; //update the position of the even sample

					//printf("loading effective address odd %d \n",odd_vector[axis]);
					//printf("loading effective address even %d \n",even_vector[axis]);

					pos_odd  = get_memory_index(odd_vector,  buffer->dimension_strides, buffer->dimension);
					pos_even = get_memory_index(even_vector, buffer->dimension_strides, buffer->dimension);
					
					//printf("effective address odd %d \n",pos_odd);
					//printf("effective address even %d \n",pos_even);

					pos_t = (butterfly * stride_t) % size_tw; //compute the index of the necessary twiddle factor
					
					//printf("\t\ttwiddle factor->%d -> %f + i * %f\n",pos_t,twiddle_re[pos_t],twiddle_im[pos_t]);
					
					switch(context->type){ // compute the type independent fft
						case CPLX_TYPE_BYTE:


							tmp_re_8t = re_8t[pos_odd];
							tmp_im_8t = im_8t[pos_odd];

							re_8t[pos_odd]  = tmp_re_8t + re_8t[pos_even]; //butterfly wing 1
							im_8t[pos_odd]  = tmp_im_8t + im_8t[pos_even];

							diff_re_8t = tmp_re_8t - re_8t[pos_even];
							diff_im_8t = tmp_im_8t - im_8t[pos_even];

							re_8t[pos_even] = re_mul_b(diff_re_8t, diff_im_8t, twiddle_re_ft[pos_t], twiddle_im_ft[pos_t]); //butterfly wing 2
							im_8t[pos_even] = im_mul_b(diff_re_8t, diff_im_8t, twiddle_re_ft[pos_t], twiddle_im_ft[pos_t]);
						break;

						case CPLX_TYPE_INT:
							tmp_re_16t = re_16t[pos_odd];
							tmp_im_16t = im_16t[pos_odd];

							re_16t[pos_odd]  = tmp_re_16t + re_16t[pos_even]; //butterfly wing 1
							im_16t[pos_odd]  = tmp_im_16t + im_16t[pos_even];

							diff_re_16t = tmp_re_16t - re_16t[pos_even];
							diff_im_16t = tmp_im_16t - im_16t[pos_even];

							re_16t[pos_even] = re_mul_i(diff_re_16t, diff_im_16t, twiddle_re_ft[pos_t], twiddle_im_ft[pos_t]); //butterfly wing 2
							im_16t[pos_even] = im_mul_i(diff_re_16t, diff_im_16t, twiddle_re_ft[pos_t], twiddle_im_ft[pos_t]);
						break;

						case CPLX_TYPE_SP:
							tmp_re_ft = re_ft[pos_odd];
							tmp_im_ft = im_ft[pos_odd];

							re_ft[pos_odd]  = tmp_re_ft + re_ft[pos_even]; //butterfly wing 1
							im_ft[pos_odd]  = tmp_im_ft + im_ft[pos_even];

							diff_re_ft = tmp_re_ft - re_ft[pos_even];
							diff_im_ft = tmp_im_ft - im_ft[pos_even];

							re_ft[pos_even] = re_mul_i(diff_re_ft, diff_im_ft, twiddle_re_ft[pos_t], twiddle_im_ft[pos_t]); //butterfly wing 2
							im_ft[pos_even] = im_mul_i(diff_re_ft, diff_im_ft, twiddle_re_ft[pos_t], twiddle_im_ft[pos_t]);
						break;

						case CPLX_TYPE_DP:


							tmp_re_dt = re_dt[pos_odd];
							tmp_im_dt = im_dt[pos_odd];

							re_dt[pos_odd]  = tmp_re_dt + re_dt[pos_even]; //butterfly wing 1
							im_dt[pos_odd]  = tmp_im_dt + im_dt[pos_even];

							diff_re_dt = tmp_re_dt - re_dt[pos_even];
							diff_im_dt = tmp_im_dt - im_dt[pos_even];

							re_dt[pos_even] = re_mul_i(diff_re_dt, diff_im_dt, twiddle_re_dt[pos_t], twiddle_im_dt[pos_t]); //butterfly wing 2
							im_dt[pos_even] = im_mul_i(diff_re_dt, diff_im_dt, twiddle_re_dt[pos_t], twiddle_im_dt[pos_t]);
						break;
					}
				}
			}
			
			dfts        <<= 1; //update the dfts,butterflys and several further values
			butterflies >>= 1;
			dft_size    >>= 1;
			stride_t    <<= 1;
		}
	}
}

void lsfft_perform(FFT_CONTEXT* context, CPLX_SAMPLES* buffer){
	if(context && buffer){
		//printf("context type:%d size:%d mode:%d \n", context->type,context->samples,context->mode);
		if(context->mode&2 != 0){
			//Multidimensional entrypoint.
		} else {
			if(context->samples == buffer->length){
				switch(context->type){
					case CPLX_TYPE_SP:
						//printf("performing fft with %d samples single precision\n",context->samples);
						//printf("clocks %d\n",(int) clock());
						//buffer->exec_time = clock();
						fft_float(context,buffer);
						bit_reverse_float(context,buffer);
						//buffer->exec_time = clock() - buffer->exec_time;
					break;
				
					case CPLX_TYPE_DP:
						//printf("performing fft with %d samples double precision\n",context->samples);
						//printf("clocks %d\n",(int) clock());
						//buffer->exec_time = clock();
						fft_double(context,buffer);
						bit_reverse_double(context,buffer);
						//buffer->exec_time = clock() - buffer->exec_time;
					break;
				
					case CPLX_TYPE_INT:
						//printf("performing fft with %d samples double precision\n",context->samples);
						//printf("clocks %d\n",(int) clock());
						//buffer->exec_time = clock();
						fft_int(context,buffer);
						bit_reverse_int(context,buffer);
						//buffer->exec_time = clock() - buffer->exec_time;
					break;

					case CPLX_TYPE_BYTE:
						
					break;
				}
			}		
		}
	}
}
