#include<stdio.h>
#include<libsimplefft/libsimplefft.h>

//compile with libsimplefft installed: gcc -o main main.c -lsimplefft

int main(void){
	CPLX_SAMPLES* samples = lsfft_alloc_complex_buffer(16,CPLX_TYPE_DP); //get the sample buffer structure
	FFT_CONTEXT* fft_context = lsfft_init(16,CPLX_TYPE_DP,FFT_MODE_NORMAL); //get the FFT context
	FFT_CONTEXT* ifft_context = lsfft_init(16,CPLX_TYPE_DP,FFT_MODE_INVERSE); //get the FFT context
	
	
	//--FFT AND iFFT
	int i=4;
	for(;i<12;i++){ //store a saw tooth wave
		((double*)samples->re)[i] = 1;
	}
	
	lsfft_perform(fft_context,samples); // perform FFT

	printf("FFT result:\n"); //show the result of the FFT
	
	for(i=0;i<16;i++){
		printf("%f + i * %f\n",((double*)samples->re)[i],((double*)samples->im)[i]);
	}
	
	lsfft_perform(ifft_context,samples); // perform FFT
	
	printf("\niFFT result:\n"); //show the result of the inverse DFT
	
	for(i=0;i<16;i++){
		printf("%f + i * %f\n",((double*)samples->re)[i],((double*)samples->im)[i]);
	}
	
	//--filtering using fast convolution
	CPLX_SAMPLES* kernel = lsfft_alloc_complex_buffer(16,CPLX_TYPE_DP); //get the buffer for the convolution kernel

	((double*)kernel->re)[0] = -1; // perform an "edge detection
	((double*)kernel->re)[1] = -0;
	((double*)kernel->re)[2] =  1;
	
	CONVOLUTION_CONTEXT* conv_context = lsfft_init_convolution(kernel);
	
	lsfft_perform_convolution(conv_context,samples); // compute the convolution
	
	printf("\nConvolution result:\n"); //show the result of the Convolution
	
	for(i=0;i<16;i++){
		printf("%f + i * %f\n",((double*)samples->re)[i],((double*)samples->im)[i]);
	}
}
