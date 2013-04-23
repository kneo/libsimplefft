#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<libsimplefft/libsimplefft.h>

//compile with libsimplefft installed: gcc -o main main.c -lsimplefft


uint32_t get_memory_index(uint32_t* memory_vector, uint32_t* stride_array, uint32_t dimension){
	uint32_t i = 0;
	uint32_t res = 0;

	for(;i<dimension;i++){
		//printf("stride %d\n",stride_array[i]);
		res = res + memory_vector[i] * stride_array[i];
	}

	return res;
}


void bit_reverse_int_md(FFT_CONTEXT* fft_context, CPLX_SAMPLES* buffer, uint32_t* memory_vector, uint32_t axis){
	uint32_t samples = buffer->length;
	uint32_t index;

	int16_t tmp_re,tmp_im,norm=1;

	uint32_t memory_from[buffer->dimension];
	uint32_t memory_to[buffer->dimension];
	//initialize vectors

	memcpy(memory_from, memory_vector,sizeof(memory_from));
	memcpy(memory_to, memory_vector,sizeof(memory_to));

	uint16_t* re = (uint16_t*)buffer->re;
	uint16_t* im = (uint16_t*)buffer->im;

	uint32_t from_index;
	uint32_t to_index;
	uint32_t i;
	
	for(i=0;i<buffer->base_length / 2;i++){
		//printf("i:%d\n",i);
		memory_from[axis] = i;
		memory_to[axis] = fft_context->bit_rev_indices[i];

		//printf("getting linear indices ...");
		from_index = get_memory_index(memory_from, buffer->dimension_strides, buffer->dimension);	//compute the strided indices	
		to_index   = get_memory_index(memory_to,   buffer->dimension_strides, buffer->dimension);   //compute the stridet indices of the target
		//printf("done!\n");

		//printf("from %d -> %d to %d -> %d\n",memory_from[axis],from_index,memory_to[axis],to_index);
		if(memory_to[axis] >= memory_from[axis]){
			tmp_re = re[from_index]; //simple exchange ...
			tmp_im = im[from_index];
			
			re[from_index] = re[to_index]*norm;
			im[from_index] = im[to_index]*norm;
				
			re[to_index] = tmp_re*norm;
			im[to_index] = tmp_im*norm;
		}
	}
}




void fft_int_md(FFT_CONTEXT* context, CPLX_SAMPLES* buffer, uint32_t axis,uint32_t* memory_vector){
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
		
		float* twiddle_re = (float*)context->twiddle_factors->re; //get the reference of the twiddle factors or unity roots
		float* twiddle_im = (float*)context->twiddle_factors->im; //
		
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
					
					tmp_re = re[pos_odd]; //temprorary storing the first sample as mentioned above
					tmp_im = im[pos_odd];
					
					re[pos_odd]  = tmp_re + re[pos_even]; //butterfly wing 1
					im[pos_odd]  = tmp_im + im[pos_even];
					
					diff_re = tmp_re - re[pos_even];
					diff_im = tmp_im - im[pos_even];
					
					re[pos_even] = re_mul_i(diff_re, diff_im, twiddle_re[pos_t], twiddle_im[pos_t]); //butterfly wing 2
					im[pos_even] = im_mul_i(diff_re, diff_im, twiddle_re[pos_t], twiddle_im[pos_t]);
					
					printf("\t\tresult odd  -> %d + i * %d\n",re[pos_odd],im[pos_odd]);
					printf("\t\tresult even -> %d + i * %d\n",re[pos_even],im[pos_even]);
					
				}
			}
			
			dfts        <<= 1; //update the dfts,butterflys and several further values
			butterflies >>= 1;
			dft_size    >>= 1;
			stride_t    <<= 1;
		}
	}
}

void perform_fft_md(FFT_CONTEXT* context, CPLX_SAMPLES* samples){
	if(context && samples && (context->mode & FFT_MODE_MD)){
		uint32_t memory_vector[samples->dimension];
		uint32_t mask_vector[samples->dimension];
		uint32_t axis=0;
		uint32_t i;

		memset(memory_vector,0,sizeof(memory_vector));
		memset(mask_vector,0,sizeof(mask_vector));
		mask_vector[0] = 1;

		int16_t* re =(uint16_t*)samples->re;
		int16_t* im =(uint16_t*)samples->im;
		
		do{//iterate over all axis
			do{//for each axis iterate over all none variable axis
				memory_vector[axis]=0;

				printf("performing FFT of ");

				printf("Address [");
				for(i=0;i<samples->dimension;i++){
					printf("%d ",memory_vector[i]);
				}
				printf("]\n");

				fft_int_md(context,samples,axis,memory_vector);
				printf("reversing indices of axis %d\n",axis);
				bit_reverse_int_md(context, samples, memory_vector, axis);

			}while(!inc_vector(memory_vector,mask_vector,samples->base_length,samples->dimension));
			axis++;

			for(i=0;i<samples->length;i++){
				if(i>0 && (i % samples->base_length == 0)){
					printf("\n");
				}
				printf("%d+i*%d ",re[i],im[i]);
			}
			printf("\n");

		}while(!vector_lsh(mask_vector,samples->dimension));	
	}
}

int main(void){
	//CPLX_SAMPLES* samples = lsfft_alloc_complex_buffer(16,CPLX_TYPE_INT); //get the sample buffer structure
	
	//get the sample buffer structure for multi dimension processing
	//it allocates a 8 bin fft with 2 dimensions

	CPLX_SAMPLES* samples_md = lsfft_alloc_complex_buffer_md(8,CPLX_TYPE_INT,2);

	FFT_CONTEXT* fft_context = lsfft_init(samples_md->base_length,CPLX_TYPE_INT,FFT_MODE_NORMAL|FFT_MODE_MD); //get the FFT context
	//FFT_CONTEXT* ifft_context = lsfft_init(16,CPLX_TYPE_INT,FFT_MODE_INVERSE); //get the FFT context

	uint32_t i,j;
	printf("loading samples\n");
	int16_t* re =(int16_t*)samples_md->re;
	int16_t* im =(int16_t*)samples_md->im;

	for(i=0;i<samples_md->length;i++){
		re[i] = 0;
		im[i] = 0;
	}

	uint32_t vector[2] = {4,4};

	uint32_t pos = get_memory_index(vector,samples_md->dimension_strides,samples_md->dimension); 

	re[pos] = 10;

	for(i=0;i<samples_md->length;i++){
		if(i>0 && (i % samples_md->base_length == 0)){
			printf("\n");
		}
		printf("%d+i*%d ",re[i],im[i]);
	}
	printf("\n");

	printf("performing MD FFT\n");
	perform_fft_md(fft_context,samples_md);
	
 	printf("Outputting Result\n");

	for(i=0;i<samples_md->length;i++){
		if(i>0 && (i % samples_md->base_length == 0)){
			printf("\n");
		}
		printf("%d+i*%d ",re[i],im[i]);
	}
	printf("\n");

	//printf("axis %d\n",axis);
	/*
	bit_reverse_int_md(fft_context,samples_md,memory_vector,axis);
	
	for(i=0;i<samples_md->base_length;i++){ //fill the buffer note: it takes 4GiB of memory to fill this buffer...
		memory_vector[axis] = i;
		uint32_t index = get_memory_index(memory_vector,samples_md->dimension_strides,samples_md->dimension);
		printf("%d ",((uint16_t*)samples_md->re)[index]);
	}
	printf("\n");*/

	/*
	//--FFT AND iFFT
	int i=0;
	for(;i<16;i++){ //store a square wave pulse
		((int16_t*)samples->re)[i] = i;
	}
	
	lsfft_perform(fft_context,samples); // perform FFT

	printf("FFT result:\n"); //show the result of the FFT
	
	for(i=0;i<16;i++){
		printf("%d + i * %d\n",((int16_t*)samples->re)[i],((int16_t*)samples->im)[i]);
	}
	
	lsfft_perform(ifft_context,samples); // perform FFT
	
	printf("\niFFT result:\n"); //show the result of the inverse DFT
	
	for(i=0;i<16;i++){
		printf("%d + i * %d\n",((int16_t*)samples->re)[i],((int16_t*)samples->im)[i]);
	}
	
	//--filtering using fast convolution
	CPLX_SAMPLES* kernel = lsfft_alloc_complex_buffer(16,CPLX_TYPE_INT); //get the buffer for the convolution kernel

	((int16_t*)kernel->re)[0] = -1; // perform an "edge" detection using 1D Sobel-Filter
	((int16_t*)kernel->re)[1] =  0;
	((int16_t*)kernel->re)[2] =  1;
	
	CONVOLUTION_CONTEXT* conv_context = lsfft_init_convolution(kernel);
	
	lsfft_perform_convolution(conv_context,samples); // compute the convolution
	
	printf("\nConvolution result:\n"); //show the result of the Convolution
	
	for(i=0;i<16;i++){
		printf("%d + i * %d\n",((int16_t*)samples->re)[i],((int16_t*)samples->im)[i]);
	}*/


	lsfft_free_complex_buffer(samples_md);

	printf("press any key to quit!\n");
	getchar();
}
