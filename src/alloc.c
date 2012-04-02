#include "alloc.h"

CPLX_SAMPLES* alloc_samples_struct(){
	CPLX_SAMPLES* res = (CPLX_SAMPLES*) calloc(1,sizeof(struct CPLX_SAMPLES));

	if(res)
		return res;
	else
		return NULL;
}

CPLX_SAMPLES* lsfft_alloc_complex_buffer(uint32_t samples, uint8_t type){
	if(type>-1 && type<3){
		unsigned int samp;
		
		//allocate 2^n samples
		//if 2 is not the nth root of samples
		//generate a buffer with 2^(log_2(samples)+1) samples
		
		if(samples % 2 == 0 && bitcount(samples)==1){
			samp = samples;
		} else {
			samp = exp_2(log_2(samples)+1);
		}
		
		//allocate the basic complex sample buffer structure
		CPLX_SAMPLES* res = alloc_samples_struct();
		res->type   = type;
		res->length = samp;
		
		//distinguish the type of the fft, and allocate memory accordingly to the type of the fft
		
		switch(type){
			case CPLX_TYPE_SP: //single precision FFT
				res->re = (void*) calloc(samp,sizeof(float));
				res->im = (void*) calloc(samp,sizeof(float));
			break;
			
			case CPLX_TYPE_DP://double precision FFT
				res->re = (void*) calloc(samp,sizeof(double));
				res->im = (void*) calloc(samp,sizeof(double));
			break;
			
			case CPLX_TYPE_INT://integer FFT
				res->re = (void*) calloc(samp,sizeof(int16_t));
				res->im = (void*) calloc(samp,sizeof(int16_t));
			break;
		}
		
		return res;
	} return NULL;
}

void lsfft_free_complex_buffer(CPLX_SAMPLES* buffer){
	if(buffer){
		free(buffer->re);
		free(buffer->im);
		free(buffer);
	}
}
