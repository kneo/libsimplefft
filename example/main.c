#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<libsimplefft/libsimplefft.h>

//compile with libsimplefft installed: gcc -o main main.c -lsimplefft

int main(void){
	//CPLX_SAMPLES* samples = lsfft_alloc_complex_buffer(16,CPLX_TYPE_INT); //get the sample buffer structure
	
	//get the sample buffer structure for multi dimension processing
	//it allocates a 8 bin fft with 2 dimensions

	CPLX_SAMPLES* samples_md = lsfft_alloc_complex_buffer_md(16,CPLX_TYPE_INT,3);


	FFT_CONTEXT* fft_context = lsfft_init(samples_md->base_length,CPLX_TYPE_INT,FFT_MODE_NORMAL|FFT_MODE_MD); //get the FFT context
	//FFT_CONTEXT* ifft_context = lsfft_init(16,CPLX_TYPE_INT,FFT_MODE_INVERSE); //get the FFT context

	uint32_t i;
	for(i=0;i<samples_md->length;i++){ //fill the buffer note: it takes 4GiB of memory to fill this buffer...
		((uint16_t*)samples_md->re)[i] = i;
		((uint16_t*)samples_md->im)[i] = i;
	}
	
	CPLX_SAMPLES* buffer = samples_md;

	uint32_t samples = buffer->length;
	
	uint32_t index;
	
	int16_t tmp_re,tmp_im,norm=1;

	uint32_t memory_from[buffer->dimension];
	uint32_t memory_to[buffer->dimension];
	//initialize vectors
	memset(memory_from,0,sizeof(memory_from));
	memset(memory_to,0,sizeof(memory_to));

	uint32_t j;

	uint16_t* re = (uint16_t*)buffer->re;
	uint16_t* im = (uint16_t*)buffer->im;

	uint32_t axis = 2;

	uint32_t from_index;
	uint32_t to_index;
	
	for(i=0;i<buffer->base_length;i++){
		printf("%d ",i);
	}
	printf("\n");

	for(i=0;i<buffer->base_length / 2;i++){
		memory_from[axis] = i;
		memory_to[axis] = fft_context->bit_rev_indices[i];

		from_index = get_memory_index(memory_from,buffer->dimension_strides,buffer->dimension);	//compute the strided indices	
		to_index   = get_memory_index(memory_to,buffer->dimension_strides,buffer->dimension); //compute the stridet indices of the target

		if(to_index >= from_index){
			tmp_re = re[from_index]; //simple exchange ...
			tmp_im = im[from_index];
			
			re[from_index] = re[to_index]*norm;
			im[from_index] = im[to_index]*norm;
				
			re[to_index] = tmp_re*norm;
			im[to_index] = tmp_im*norm;
		}
	}

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
