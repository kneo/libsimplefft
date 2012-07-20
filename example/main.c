#include<stdio.h>
#include<libsimplefft/libsimplefft.h>

//compile with libsimplefft installed: gcc -o main main.c -lsimplefft

int main(void){
	CPLX_SAMPLES* samples = lsfft_alloc_complex_buffer(16,CPLX_TYPE_DP); //get the sample buffer structure
	FFT_CONTEXT* context = lsfft_init(16,CPLX_TYPE_DP,FFT_MODE_NORMAL); //get the FFT context
	
	int i=0;
	for(;i<16;i++){ //store a saw tooth wave
		((double*)samples->re)[i] = i%4;
	}
	
	lsfft_perform(context,samples); // perform FFT

	printf("result:\n"); //show the result
	
	for(i=0;i<16;i++){
		printf("%f + i * %f\n",((double*)samples->re)[i],((double*)samples->im)[i]);
	}
}
