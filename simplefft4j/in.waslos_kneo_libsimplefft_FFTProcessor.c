/*
*This is a part of libsimplefft
*
* Copyright (C) 2012  Kevin Krüger (kkevin@gmx.net)
* 
* libsimplefft is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
* 
* libsimplefft is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
* 
* You should have received a copy of the GNU Lesser General Public
* License along with libsimplefft; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/
#include "f4jdefines.h"

#include "../libsimplefft/libsimplefft.h"
#include "in_waslos_kneo_libsimplefft_FFTProcessor.h"

static FFT_HANDLES* fft_handles;
static CONVOLUTION_HANDLES* conv_handles;

int32_t look_up_compatible_fft_context(int size, int type,int mode);


void init_fft_handles(){
	fft_handles = (FFT_HANDLES*)calloc(1,sizeof(FFT_HANDLES));
}

JNIEXPORT jint JNICALL Java_in_waslos_kneo_libsimplefft_FFTProcessor_createFFTContext(JNIEnv * env, jobject class, jint samples, jbyte mode, jbyte type){
	//printf("initializing fft...\n");
	if(!fft_handles){
		//printf("first initialization allocating fft context\n");
		init_fft_handles();
	}
	
	int32_t handle = look_up_compatible_fft_context(samples,type,mode);
	
	if(handle<=0){
		if(fft_handles->c_handles<MAX_FFT_HANDLES){
			uint32_t i = 0;

			for(;i<MAX_FFT_HANDLES;i++) //find a free pointer
				if(fft_handles->storage[i]==NULL)
					break;
			//no more space for ffts
			if(i<MAX_FFT_HANDLES){
				FFT_CONTEXT* context = lsfft_init(samples,type,mode);

				if(!context) return -1;

				fft_handles->storage[i] = context;
				fft_handles->c_handles++;

				return i;
			}
		}
	} else return handle;
	
	return -1;
}

JNIEXPORT void JNICALL Java_in_waslos_kneo_libsimplefft_FFTProcessor_destroyFFTContext(JNIEnv * env, jobject class, jint handle){
	if(fft_handles->c_handles>handle && fft_handles->storage[handle] != NULL){
		
		if(conv_handles){ //look for cross references of the same fft context
			int32_t i = 0;
			for( ;i<conv_handles->c_handles;i++){
				CONVOLUTION_CONTEXT* context = conv_handles->storage[i];
				FFT_CONTEXT* fft_context = fft_handles->storage[handle];
				if(fft_context == context->ifft_context || fft_context==context->fft_context){
					//fft contexts in use so it is unsafe to remove a return
					//is performed and a warning is displayed
					printf("Warning: FFT Context %d is still used by Convolution context %d! \n Please destroy this convolution before destroying the fft.",handle,i);
					//do nothing and return 
					return;
				}
			}
		}
		
		lsfft_destroy_context(fft_handles->storage[handle]);
		fft_handles->storage[handle]=NULL;
		fft_handles->c_handles--;
	}
}

JNIEXPORT void JNICALL Java_in_waslos_kneo_libsimplefft_FFTProcessor_performFFTf(JNIEnv * env, jobject class, jint handle, jfloatArray re, jfloatArray im){
	//printf("performing FFT...");
	if(fft_handles->c_handles > handle && fft_handles->storage[handle] != NULL){
		//printf("all set!\n");
		jfloat* fre = (*env)->GetFloatArrayElements(env, re, 0);
		jfloat* fim = (*env)->GetFloatArrayElements(env, im, 0);
		
		FFT_CONTEXT* context = fft_handles->storage[handle];
		
		CPLX_SAMPLES samples;
		
		samples.type=context->type;
		samples.length=context->samples;
		
		samples.re = (float*)fre;
		samples.im = (float*)fim;
		
		lsfft_perform(context,&samples);
		
		(*env)->ReleaseFloatArrayElements(env, re, fre, 0);
		(*env)->ReleaseFloatArrayElements(env, im, fim, 0);
	}
}

JNIEXPORT void JNICALL Java_in_waslos_kneo_libsimplefft_FFTProcessor_performFFTd(JNIEnv * env, jobject class, jint handle, jdoubleArray re, jdoubleArray im){
	if(fft_handles->c_handles > handle && fft_handles->storage[handle] != NULL){
		jdouble* fre = (*env)->GetDoubleArrayElements(env, re, 0);
		jdouble* fim = (*env)->GetDoubleArrayElements(env, im, 0);
		
		FFT_CONTEXT* context = fft_handles->storage[handle];
		
		CPLX_SAMPLES samples;
		
		samples.type=context->type;
		samples.length=context->samples;
		
		samples.re = (double*)fre;
		samples.im = (double*)fim;
		
		lsfft_perform(context,&samples);
		
		(*env)->ReleaseDoubleArrayElements(env, re, fre, 0);
		(*env)->ReleaseDoubleArrayElements(env, im, fim, 0);
	}
}

JNIEXPORT void JNICALL Java_in_waslos_kneo_libsimplefft_FFTProcessor_performFFTi(JNIEnv * env, jobject class , jint handle, jshortArray re, jshortArray im){
	if(fft_handles->c_handles > handle && fft_handles->storage[handle] != NULL){
	
		jshort* fre = (*env)->GetShortArrayElements(env, re, 0);
		jshort* fim = (*env)->GetShortArrayElements(env, im, 0);
		
		FFT_CONTEXT* context = fft_handles->storage[handle];

		CPLX_SAMPLES samples;
		
		samples.type=context->type;
		samples.length=context->samples;
		
		samples.re = (short*)fre;
		samples.im = (short*)fim;

		lsfft_perform(context,&samples);
		
		(*env)->ReleaseShortArrayElements(env, re, fre, 0);
		(*env)->ReleaseShortArrayElements(env, im, fim, 0);
	}
}

int32_t look_up_compatible_fft_context(int size, int type,int mode){
	
	if(fft_handles){
		uint32_t i = 0;
		FFT_CONTEXT* context;
		
		for(i=0;i<MAX_FFT_HANDLES;i++){
			context = fft_handles->storage[i];
			
			if(context){
				if(context->type==type && context->mode==mode && context->samples==size){
					//printf("found reusable fft!\n");
					return i;
				}
			}
		}
		
		//printf("did not found reusable fft!\n");	
	}
	return -1;
	
}


FFT_CONTEXT* look_up_or_create_compatible_fft_context(int size, int type,int mode){
	
	if(fft_handles){
		uint32_t i = 0;
		FFT_CONTEXT* context;
		
		for(i=0;i<MAX_FFT_HANDLES;i++){
			context = fft_handles->storage[i];
			
			if(context){
				if(context->type==type && context->mode==mode && context->samples==size){
					//printf("found reusable fft!\n");
					return context;
				}
			}
		}
		
		//printf("did not found reusable fft! creating ...\n");
		
		if(fft_handles->c_handles<MAX_FFT_HANDLES){
			for(i=0;i<MAX_FFT_HANDLES;i++) //find a free pointer
				if(fft_handles->storage[i]==NULL)
					break;
				
			FFT_CONTEXT* context = lsfft_init(size,type,mode);
			
			if(context){
				fft_handles->storage[i]=context;
				fft_handles->c_handles++;
			}
			return context;
		}
	}
	return NULL;
	
}

void init_conv_handles(void){
	conv_handles = (CONVOLUTION_HANDLES*) calloc(1,sizeof(CONVOLUTION_HANDLES));
}

JNIEXPORT jint JNICALL Java_in_waslos_kneo_libsimplefft_FFTProcessor_createFastConvolutionContext__I_3F(JNIEnv* env, jclass object, jint samples, jfloatArray kernel_re, jfloatArray kernel_im){
	
	if(!fft_handles){
		init_fft_handles();
	}
	
	if(!conv_handles){
		init_conv_handles();
	}
	
	if((conv_handles->c_handles+1) > MAX_CONV_HANDLES) return -1;
	
	jsize len_re = (*env)->GetArrayLength(env, kernel_re);
	jsize len_im = (*env)->GetArrayLength(env, kernel_im);
	
	if(len_im!=len_re) return -1;
	
	//retrieve kernel
	float* fkernel_re = (float*) (*env)->GetFloatArrayElements(env, kernel_re, 0);
	float* fkernel_im = (float*) (*env)->GetFloatArrayElements(env, kernel_im, 0);
	
	if(len_re>samples){
		len_re = samples;
	}

	//create a new kernel buffer for complex processing
	CPLX_SAMPLES* c_kernel = lsfft_alloc_complex_buffer(samples,CPLX_TYPE_SP);

	//pointer to real part of the real part
	float* re = (float*) c_kernel->re;
	float* im = (float*) c_kernel->im;

	int i;

	for(i=0;i<len_re;i++){
		re[i] = fkernel_re[i];
		im[i] = fkernel_im[i];
	}
	
	//reuse compatible fft contexts
	FFT_CONTEXT* fft  = look_up_or_create_compatible_fft_context(samples,CPLX_TYPE_SP,FFT_MODE_NORMAL);
	FFT_CONTEXT* ifft = look_up_or_create_compatible_fft_context(samples,CPLX_TYPE_SP,FFT_MODE_INVERSE);
	

	//get an unused slot for the convolution
	for(i=0;i<MAX_CONV_HANDLES;i++){
		if(conv_handles->storage[i]==NULL) break;
	}
	
	//create new context
	CONVOLUTION_CONTEXT* context = lsfft_init_convolution_using_fft_context(fft,ifft,c_kernel);
	//assing new colvolution context to the empty spot
	
	conv_handles->storage[i] = context;
	conv_handles->c_handles++;
	
	(*env)->ReleaseFloatArrayElements(env, kernel_re, fkernel_re, 0);
	(*env)->ReleaseFloatArrayElements(env, kernel_im, fkernel_im, 0);
	//return handle for java library
	return i;
}

JNIEXPORT jint JNICALL Java_in_waslos_kneo_libsimplefft_FFTProcessor_createFastConvolutionContext__I_3D(JNIEnv* env, jclass object, jint samples, jdoubleArray kernel_re,jdoubleArray kernel_im){
	
	if(!fft_handles){
		init_fft_handles();
	}
	
	if(!conv_handles){
		init_conv_handles();
	}
	
	if((conv_handles->c_handles+1) > MAX_CONV_HANDLES) return -1;
	
	jsize len_re = (*env)->GetArrayLength(env, kernel_re);
	jsize len_im = (*env)->GetArrayLength(env, kernel_im);
	
	if(len_im!=len_re) return -1;
	
	//retrieve kernel
	double* fkernel_re = (double*) (*env)->GetDoubleArrayElements(env, kernel_re, 0);
	double* fkernel_im = (double*) (*env)->GetDoubleArrayElements(env, kernel_im, 0);
	
	if(len_re>samples){
		len_re = samples;
	}

	//create a new kernel buffer for complex processing
	CPLX_SAMPLES* c_kernel = lsfft_alloc_complex_buffer(samples,CPLX_TYPE_SP);

	//pointer to real part of the real part
	double* re = (double*) c_kernel->re;
	double* im = (double*) c_kernel->im;

	int i;

	for(i=0;i<len_re;i++){
		re[i] = fkernel_re[i];
		im[i] = fkernel_im[i];
	}
	
	//reuse compatible fft contexts
	FFT_CONTEXT* fft  = look_up_or_create_compatible_fft_context(samples,CPLX_TYPE_DP,FFT_MODE_NORMAL);
	FFT_CONTEXT* ifft = look_up_or_create_compatible_fft_context(samples,CPLX_TYPE_DP,FFT_MODE_INVERSE);
	

	//get an unused slot for the convolution
	for(i=0;i<MAX_CONV_HANDLES;i++){
		if(conv_handles->storage[i]==NULL) break;
	}
	
	//create new context
	CONVOLUTION_CONTEXT* context = lsfft_init_convolution_using_fft_context(fft,ifft,c_kernel);
	//assing new colvolution context to the empty spot
	
	conv_handles->storage[i] = context;
	conv_handles->c_handles++;
	
	(*env)->ReleaseDoubleArrayElements(env, kernel_re, fkernel_re, 0);
	(*env)->ReleaseDoubleArrayElements(env, kernel_im, fkernel_im, 0);
	//return handle for java library
	return i;
}

JNIEXPORT jint JNICALL Java_in_waslos_kneo_libsimplefft_FFTProcessor_createFastConvolutionContext__I_3S(JNIEnv* env, jclass object, jint samples, jshortArray kernel_re,jshortArray kernel_im){
	if(!fft_handles){
		init_fft_handles();
	}
	
	if(!conv_handles){
		init_conv_handles();
	}
	
	if((conv_handles->c_handles+1) > MAX_CONV_HANDLES) return -1;
	
	jsize len_re = (*env)->GetArrayLength(env, kernel_re);
	jsize len_im = (*env)->GetArrayLength(env, kernel_im);
	
	if(len_im!=len_re) return -1;
	
	//retrieve kernel
	int16_t* fkernel_re = (int16_t*) (*env)->GetShortArrayElements(env, kernel_re, 0);
	int16_t* fkernel_im = (int16_t*) (*env)->GetShortArrayElements(env, kernel_im, 0);
	
	if(len_re>samples){
		len_re = samples;
	}

	//create a new kernel buffer for complex processing
	CPLX_SAMPLES* c_kernel = lsfft_alloc_complex_buffer(samples,CPLX_TYPE_SP);

	//pointer to real part of the real part
	int16_t* re = (int16_t*) c_kernel->re;
	int16_t* im = (int16_t*) c_kernel->im;

	int i;

	for(i=0;i<len_re;i++){
		re[i] = fkernel_re[i];
		im[i] = fkernel_im[i];
	}
	
	//reuse compatible fft contexts
	FFT_CONTEXT* fft  = look_up_or_create_compatible_fft_context(samples,CPLX_TYPE_DP,FFT_MODE_NORMAL);
	FFT_CONTEXT* ifft = look_up_or_create_compatible_fft_context(samples,CPLX_TYPE_DP,FFT_MODE_INVERSE);
	

	//get an unused slot for the convolution
	for(i=0;i<MAX_CONV_HANDLES;i++){
		if(conv_handles->storage[i]==NULL) break;
	}
	
	//create new context
	CONVOLUTION_CONTEXT* context = lsfft_init_convolution_using_fft_context(fft,ifft,c_kernel);
	//assing new colvolution context to the empty spot
	
	conv_handles->storage[i] = context;
	conv_handles->c_handles++;
	
	(*env)->ReleaseShortArrayElements(env, kernel_re, fkernel_re, 0);
	(*env)->ReleaseShortArrayElements(env, kernel_im, fkernel_im, 0);
	//return handle for java library
	return i;
}

JNIEXPORT void JNICALL Java_in_waslos_kneo_libsimplefft_FFTProcessor_performFastConvolution__I_3F(JNIEnv* env, jclass object, jint handle, jfloatArray signal_re,jfloatArray signal_im){
	
	if(!conv_handles) return;
	
	if(conv_handles->c_handles > 0 && conv_handles->storage[handle] != NULL){
		jsize len_re  = (*env)->GetArrayLength(env,signal_re);
		jsize len_im  = (*env)->GetArrayLength(env,signal_im);
		
		float* re = (float*) (*env)->GetFloatArrayElements(env, signal_re, 0);
		float* im = (float*) (*env)->GetFloatArrayElements(env, signal_im, 0);
		
		if(len_re!=len_re) return;
		
		CONVOLUTION_CONTEXT* context = conv_handles->storage[handle];
		
		CPLX_SAMPLES csignal;
		
		int copy_switch = 0;
		
		float tre[context->samples];
		float tim[context->samples];

		//clear temp arrays		
		memset(tre,0,sizeof(tre));
		memset(tim,0,sizeof(tim));
		
		if(len_re == context->samples){
			csignal.re = re;
			csignal.im = im;
		} else {
			copy_switch=1;
			uint32_t length;
			//crop if signal is too long
			if(context->samples < len_re){
				length = context->samples;
			} else {
				length = len_re;
			}
			
			//copy
			int i=0;
			for(;i<length;i++){
				tre[i] = re[i];
				tim[i] = im[i];
			}
			//assign
			csignal.re = &tre;
			csignal.im = &tim;
		}
		
		csignal.length = context->samples;
		csignal.type   = CPLX_TYPE_SP;
		
		lsfft_perform_convolution(context,&csignal);
		
		//copy back the result if necessary
		if(copy_switch){
			uint32_t length;
			if(context->samples < len_re){
				length = context->samples;
			} else {
				length = len_re;
			}
			
			int i=0;
			
			for(;i<length;i++){
				re[i] = ((float*)csignal.re)[i];
				im[i] = ((float*)csignal.im)[i];
			}
		}
		
		(*env)->ReleaseFloatArrayElements(env, signal_re,  re, 0);
		(*env)->ReleaseFloatArrayElements(env, signal_im,  im, 0);
	}
}

JNIEXPORT void JNICALL Java_in_waslos_kneo_libsimplefft_FFTProcessor_performFastConvolution__I_3D(JNIEnv* env, jclass object, jint handle, jdoubleArray signal_re,jdoubleArray signal_im){
	
	if(!conv_handles) return;
	
	if(conv_handles->c_handles > 0 && conv_handles->storage[handle] != NULL){
		jsize len_re  = (*env)->GetArrayLength(env,signal_re);
		jsize len_im  = (*env)->GetArrayLength(env,signal_im);
		
		double* re = (double*) (*env)->GetDoubleArrayElements(env, signal_re, 0);
		double* im = (double*) (*env)->GetDoubleArrayElements(env, signal_im, 0);
		
		if(len_re!=len_re) return;
		
		CONVOLUTION_CONTEXT* context = conv_handles->storage[handle];
		
		CPLX_SAMPLES csignal;
		
		int copy_switch = 0;
		
		double tre[context->samples];
		double tim[context->samples];

		//clear temp arrays		
		memset(tre,0,sizeof(tre));
		memset(tim,0,sizeof(tim));
		
		if(len_re == context->samples){
			csignal.re = re;
			csignal.im = im;
		} else {
			copy_switch=1;
			uint32_t length;
			//crop if signal is too long
			if(context->samples < len_re){
				length = context->samples;
			} else {
				length = len_re;
			}
			
			//copy
			int i=0;
			for(;i<length;i++){
				tre[i] = re[i];
				tim[i] = im[i];
			}
			//assign
			csignal.re = &tre;
			csignal.im = &tim;
		}
		
		csignal.length = context->samples;
		csignal.type   = CPLX_TYPE_SP;
		
		lsfft_perform_convolution(context,&csignal);
		
		//copy back the result if necessary
		if(copy_switch){
			uint32_t length;
			if(context->samples < len_re){
				length = context->samples;
			} else {
				length = len_re;
			}
			
			int i=0;
			
			for(;i<length;i++){
				re[i] = ((double*)csignal.re)[i];
				im[i] = ((double*)csignal.im)[i];
			}
		}
		
		(*env)->ReleaseDoubleArrayElements(env, signal_re,  re, 0);
		(*env)->ReleaseDoubleArrayElements(env, signal_im,  im, 0);
	}
}

JNIEXPORT void JNICALL Java_in_waslos_kneo_libsimplefft_FFTProcessor_performFastConvolution__I_3S(JNIEnv* env, jclass object, jint handle, jshortArray signal_re,jshortArray signal_im){
	
	if(!conv_handles) return;
	
	if(conv_handles->c_handles > 0 && conv_handles->storage[handle] != NULL){
		jsize len_re  = (*env)->GetArrayLength(env,signal_re);
		jsize len_im  = (*env)->GetArrayLength(env,signal_im);
		
		int16_t* re = (int16_t*) (*env)->GetShortArrayElements(env, signal_re, 0);
		int16_t* im = (int16_t*) (*env)->GetShortArrayElements(env, signal_im, 0);
		
		if(len_re!=len_re) return;
		
		CONVOLUTION_CONTEXT* context = conv_handles->storage[handle];
		
		CPLX_SAMPLES csignal;
		
		int copy_switch = 0;
		
		int16_t tre[context->samples];
		int16_t tim[context->samples];

		//clear temp arrays		
		memset(tre,0,sizeof(tre));
		memset(tim,0,sizeof(tim));
		
		if(len_re == context->samples){
			csignal.re = re;
			csignal.im = im;
		} else {
			copy_switch=1;
			uint32_t length;
			//crop if signal is too long
			if(context->samples < len_re){
				length = context->samples;
			} else {
				length = len_re;
			}
			
			//copy
			int i=0;
			for(;i<length;i++){
				tre[i] = re[i];
				tim[i] = im[i];
			}
			//assign
			csignal.re = &tre;
			csignal.im = &tim;
		}
		
		csignal.length = context->samples;
		csignal.type   = CPLX_TYPE_SP;
		
		lsfft_perform_convolution(context,&csignal);
		
		//copy back the result if necessary
		if(copy_switch){
			uint32_t length;
			if(context->samples < len_re){
				length = context->samples;
			} else {
				length = len_re;
			}
			
			int i=0;
			
			for(;i<length;i++){
				re[i] = ((int16_t*)csignal.re)[i];
				im[i] = ((int16_t*)csignal.im)[i];
			}
		}
		
		(*env)->ReleaseShortArrayElements(env, signal_re,  re, 0);
		(*env)->ReleaseShortArrayElements(env, signal_im,  im, 0);
	}
}

JNIEXPORT jint JNICALL Java_in_waslos_kneo_libsimplefft_FFTProcessor_destroyFastConvolution(JNIEnv* env, jclass object, jint handle){
	if(!conv_handles) return -1;
	
	if(handle<MAX_CONV_HANDLES && conv_handles->storage[handle] != NULL){
		CONVOLUTION_CONTEXT* context = conv_handles->storage[handle];
		
		//lsfft_destroy_context(context->fft_context);
		//lsfft_destroy_context(context->ifft_context);
		
		lsfft_free_complex_buffer(context->kernel);
		
		free(context);
		
		conv_handles->storage[handle] = NULL;
		
		return 0;
	}
	
	return -1;
}


JNIEXPORT jint JNICALL Java_in_waslos_kneo_libsimplefft_FFTProcessor_getTransformedConvolutionKernel__I_3S(JNIEnv * env, jclass class, jint type, jshortArray kernel_re, jshortArray kernel_im){
	
	return -1;
}

JNIEXPORT jint JNICALL Java_in_waslos_kneo_libsimplefft_FFTProcessor_getTransformedConvolutionKernel__I_3F(JNIEnv * env, jclass class, jint type, jfloatArray kernel_re, jfloatArray kernel_im){
	
	
	
	return -1;
}

JNIEXPORT jint JNICALL Java_in_waslos_kneo_libsimplefft_FFTProcessor_getTransformedConvolutionKernel__I_3D(JNIEnv * env, jclass class, jint type, jdoubleArray kernel_re, jdoubleArray kernel_im){
	
	return -1;
}

JNIEXPORT jint JNICALL Java_in_waslos_kneo_libsimplefft_FFTProcessor_getConvolutionSize (JNIEnv * env, jclass class, jint handle){
	 if(conv_handles!=NULL){
		 if(conv_handles->storage[handle] != NULL){
			 return conv_handles->storage[handle]->samples;
		}
	}
	return -1;
}

JNIEXPORT jint JNICALL Java_in_waslos_kneo_libsimplefft_FFTProcessor_findFFTHandle (JNIEnv * env, jclass class, jint type, jint size , jint mode){
	  return look_up_compatible_fft_context(size, type,mode);
}

