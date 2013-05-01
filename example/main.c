#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<libsimplefft/libsimplefft.h>

//compile with libsimplefft installed: gcc -o main main.c -lsimplefft

int main(void){
	//CPLX_SAMPLES* samples = lsfft_alloc_complex_buffer(16,CPLX_TYPE_INT); //get the sample buffer structure
	
	//get the sample buffer structure for multi dimension processing
	//it allocates a 8 bin fft with 2 dimensions

	CPLX_SAMPLES* samples_md = lsfft_alloc_complex_buffer_md(8,CPLX_TYPE_BYTE,2);
	printf("getting context ...\n");
	FFT_CONTEXT* fft_context  = lsfft_init(samples_md->base_length,samples_md->type,FFT_MODE_NORMAL); //get the FFT context
	FFT_CONTEXT* ifft_context = lsfft_init(samples_md->base_length,samples_md->type,FFT_MODE_INVERSE);

	uint32_t i,j;
	printf("loading samples\n");
	int8_t* re =(int8_t*)samples_md->re;
	int8_t* im =(int8_t*)samples_md->im;

	for(i=0;i<samples_md->length;i++){
		re[i] = 0;
		im[i] = 0;
	}

	uint32_t vector[2] = {3,3};
	uint32_t pos = get_memory_index(vector,samples_md->dimension_strides,samples_md->dimension); 
	re[pos] = 10;

	vector[0] = 4;
	vector[1] = 4;
	//vector[2] = 4;
	pos = get_memory_index(vector,samples_md->dimension_strides,samples_md->dimension); 
	re[pos] = 10;
	//re[pos] *= -0.2f;

	vector[0] = 3;
	vector[1] = 4;
	//vector[2] = 4;
	pos = get_memory_index(vector,samples_md->dimension_strides,samples_md->dimension); 
	re[pos] = 10;

	vector[0] = 4;
	vector[1] = 3;
	//vector[2] = 4;
	pos = get_memory_index(vector,samples_md->dimension_strides,samples_md->dimension); 
	re[pos] = 10;

	printf("Input\n");
	lsfft_printl_samples(samples_md);

	printf("performing MD FFT\n");
	perform_fft_md(fft_context,samples_md);

 	printf("\nOutputting Result\n");
	lsfft_printl_samples(samples_md);

	printf("performing MD iFFT\n");
	perform_fft_md(ifft_context,samples_md);

 	printf("\nOutputting Result\n");	
	lsfft_printl_samples(samples_md);

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
