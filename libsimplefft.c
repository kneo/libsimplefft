#include "libsimplefft.h"

FFT_CONTEXT* lsfft_init(uint32_t samples, uint8_t type, uint8_t mode){
	
	FFT_CONTEXT* res_context = NULL;
	
	uint32_t samp = 0;
	if(samples>2){ //at least 2 samples required to perform a fft 
		if(bitcount(samples)==1){ //samples is power of 2	
			samp = samples;
			
			//printf("samples is a power of 2!\n");
		} else { //samples is not a power of 2 find its next power of to and pad nonexisting samples with 0
			int exp = log_2(samples);
			samp = exp_2(exp+1);
		}
	} else {
		return NULL;
	}
	
	res_context = (FFT_CONTEXT*) calloc(1,sizeof(FFT_CONTEXT));
	
	res_context->samples = samp;
	
	res_context->stages = log_2(samp);
	
	if(type>=0&&type<4){ //determine the data type of the FFT
		res_context->type = type;
		res_context->twiddle_factors = compute_twiddles(samp,type,mode);
	} else
		return NULL;

	if(mode>=0 && mode<2){ //determines the mode of the FFT the normal or its inverse
		res_context->mode = mode;
	} else {
		return res_context;
	}

	return res_context;
}

void lsfft_destroy_context(FFT_CONTEXT* context){
	if(context){
		lsfft_free_complex_buffer(context->twiddle_factors);
		free(context);
	}
}
