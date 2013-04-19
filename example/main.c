#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<libsimplefft/libsimplefft.h>

//compile with libsimplefft installed: gcc -o main main.c -lsimplefft

//This function is an addition with carry, which can be provided with a skip mask
//to tell 
int inc_vector(uint32_t* result, uint32_t* skip_mask, uint32_t base, uint32_t dimension){
	uint32_t lsb   = 0; //
	uint32_t carry = 0;

	do{
		start:
			if(lsb >= dimension){ //prevent overflow and possible buffer overrun
				return 1;
			}

			if(skip_mask && skip_mask[lsb])	{ //if there is a skip mask, skip this place and start over
				lsb++;
				goto start;
			}
		
			result[lsb] = (result[lsb]+1)%base; //add one add limit this place to be base max
			carry = result[lsb]==0; //if it returns 0 addition resulted a carry

			lsb=lsb+(1 & result[lsb]==0); // if there was a carry increment the lsb counter for the next iteration

	}while(carry && lsb < dimension); // while there is a carry redo loop

	return carry; // if one the vector is overflown... use it to break a loop or something
}



int main(void){
	//CPLX_SAMPLES* samples = lsfft_alloc_complex_buffer(16,CPLX_TYPE_INT); //get the sample buffer structure
	
	//get the sample buffer structure for multi dimension processing
	//it allocates a 8 bin fft with 2 dimensions

	CPLX_SAMPLES* samples_md = lsfft_alloc_complex_buffer_md(16,CPLX_TYPE_INT,1);

	FFT_CONTEXT* fft_context = lsfft_init(samples_md->base_length,CPLX_TYPE_INT,FFT_MODE_NORMAL|FFT_MODE_MD); //get the FFT context
	//FFT_CONTEXT* ifft_context = lsfft_init(16,CPLX_TYPE_INT,FFT_MODE_INVERSE); //get the FFT context

	uint32_t i;
	for(i=0;i<samples_md->length;i++){
		((uint16_t*)samples_md->re)[i] = i;
		((uint16_t*)samples_md->im)[i] = i;
	}
	
	uint32_t memory_vector[1] = {0};
	uint32_t mask_vector[1]   = {0};
	uint32_t axis    = 2;
	
	do{

		for(i=0;i<samples_md -> dimension;i++){
			printf("%d ",memory_vector[i]);
		}

		printf("\n");
	}while(!inc_vector(memory_vector,mask_vector,samples_md->base_length,samples_md->dimension));
//	printf("axis %d\n",axis);
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
