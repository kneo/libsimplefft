#include "twiddle.h"

#define PI (3.14159265358979)


void calculate_float(CPLX_SAMPLES* samples,uint8_t mode){
	//initialize SP floats
	float* re = (float*)samples->re;
	float* im = (float*)samples->im;
	
	uint32_t N = samples->length << 1;
	
	//distinguish between inverse and normal FFT
	int8_t sign = mode==0 ? 1 : -1;
	
	int i=0;
	for(;i<samples->length;i++){
		float fak = (2*sign*PI*i)/N;
		re[i] = cos(fak);
		im[i] = sin(fak);
	}
}

void calculate_double(CPLX_SAMPLES* samples,uint8_t mode){
	double* re = (double*)samples->re;
	double* im = (double*)samples->im;
	
	uint32_t N = samples->length << 1;
	
	int8_t sign = mode?-1:1;
	
	int i=0;
	for(;i<samples->length;i++){
		double fak = (2*sign*PI*i)/N;
		re[i] = cos(fak);
		im[i] = sin(fak);
	}
}

CPLX_SAMPLES* compute_twiddles(uint32_t samples,uint8_t type, uint8_t mode){

	uint32_t samp;

	if(samples>1){
		if(bitcount(samples)==1){
			samp = samples >> 1;
		} else {
			samp = exp_2(log_2(samples) + 1) >> 1;
		}
	} else {
		return NULL;
	}
	
	CPLX_SAMPLES* res;
	//distinguish between integer and floating point fft, since integer unity roots will cause a lot of zeros.
	if(type == CPLX_TYPE_INT){
		res = lsfft_alloc_complex_buffer(samp,CPLX_TYPE_SP);		
	} else {
		res = lsfft_alloc_complex_buffer(samp,type);
	}
	
	switch(type){
		case CPLX_TYPE_INT:
		case CPLX_TYPE_SP:
			calculate_float(res,mode);
		break;
		
		case CPLX_TYPE_DP:
			calculate_double(res,mode);
		break;
	}

	return res;
}


