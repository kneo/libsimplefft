#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<libsimplefft/libsimplefft.h>

//compile with libsimplefft installed: gcc -o main main.c -lsimplefft

int main(void){
	//CPLX_SAMPLES* samples = lsfft_alloc_complex_buffer(16,CPLX_TYPE_INT); //get the sample buffer structure
	
	//get the sample buffer structure for multi dimension processing
	//it allocates a 8 bin fft with 2 dimensions
	CPLX_SAMPLES* samples_md = lsfft_alloc_complex_buffer_md(8,CPLX_TYPE_INT,2);


	FFT_CONTEXT* fft_context = lsfft_init(8,CPLX_TYPE_INT,FFT_MODE_NORMAL|FFT_MODE_MD); //get the FFT context
	//FFT_CONTEXT* ifft_context = lsfft_init(16,CPLX_TYPE_INT,FFT_MODE_INVERSE); //get the FFT context

	uint32_t i;
	for(i=0;i<samples_md->length;i++){ //fill the buffer note: it takes 4GiB of memory to fill this buffer...
		((uint16_t*)samples_md->re)[i] = i;
		((uint16_t*)samples_md->im)[i] = i;
	}
	
	CPLX_SAMPLES* buffer = samples_md;

	uint32_t samples = buffer->length;
	
	uint32_t index;
	
	int16_t tmp_re,tmp_im;

	uint32_t count_vector[buffer->dimension];


	//initialize vector
	memset(count_vector,0,sizeof(count_vector));
	uint32_t counter = 0;
	uint32_t rev_index;
	uint32_t stride;
	uint32_t j;
	printf("i->vector->lind = value\n");
	for(i=0;i<buffer->length;i++){
		printf("%d:->",i);
		get_memory_vector(count_vector,buffer->dimension_strides,i,buffer->dimension);

		//printf("[%d]:%d -> %d\n",i,index,rev_index);
		printf("[ ");
		for(j=0;j<buffer->dimension;j++){
			printf("%d ",count_vector[j]);
		}
		j=get_memory_index(count_vector, buffer->dimension_strides, buffer->dimension);
		printf("]-> %d = %d + i*%d\n",j,((uint16_t*)samples_md->re)[j],((uint16_t*)samples_md->im)[j]);	
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
