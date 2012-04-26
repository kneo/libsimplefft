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


JNIEXPORT jint JNICALL Java_in_waslos_kneo_libsimplefft_FFTProcessor_createFFTContext(JNIEnv * env, jobject class, jint samples, jbyte mode, jbyte type){
	//printf("initializing fft...\n");
	if(!fft_handles){
		//printf("first initialization allocating fft context\n");
		fft_handles = (FFT_HANDLES*)calloc(1,sizeof(FFT_HANDLES));
	}
	
	if(fft_handles->c_handles<MAX_FFT_HANDLES){

		FFT_CONTEXT* context = lsfft_init(samples,type,mode);

		if(!context) return -1;

		uint32_t i = 0;

		for(;i<MAX_FFT_HANDLES;i++) //find a free pointer
			if(fft_handles->storage[i]==NULL)
				break;

		fft_handles->storage[i] = context;
		fft_handles->c_handles++;
		
		return i;
	}
	
	return -1;
}

JNIEXPORT void JNICALL Java_in_waslos_kneo_libsimplefft_FFTProcessor_destroyFFTContext(JNIEnv * env, jobject class, jint handle){
	if(fft_handles->c_handles>handle && fft_handles->storage[handle] != NULL){
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

void init_conv_handles(void){
	conv_handles = (CONVOLUTION_HANDLES*) calloc(1,sizeof(CONVOLUTION_HANDLES));
}

JNIEXPORT jint JNICALL Java_in_waslos_kneo_libsimplefft_FFTProcessor_createFastConvolutionContext__I_3F(JNIEnv* env, jclass object, jint samples, jfloatArray kernel){
	if(!conv_handles){
		init_conv_handles();
	}
	
	if((conv_handles->c_handles+1) > MAX_CONV_HANDLES) return -1;
	
	//retrieve kernel
	jsize len = (*env)->GetArrayLength(env, kernel);
	float* fkernel = (float*) (*env)->GetFloatArrayElements(env, kernel, 0);

	//create a new kernel buffer for complex processing
	CPLX_SAMPLES* c_kernel = lsfft_alloc_complex_buffer(samples,CPLX_TYPE_SP);

	//pointer to real part of the real part
	float* re = (float*) c_kernel->re;

	int i;

	for(i=0;i<len;i++){
		re[i] = fkernel[i];
	}
	
	CONVOLUTION_CONTEXT* context = lsfft_init_convolution(c_kernel);
	
	for(i=0;i<MAX_CONV_HANDLES;i++){
		if(conv_handles->storage[i]==NULL) break;
	}
	
	conv_handles->storage[i] = context;
	conv_handles->c_handles++;
	
	(*env)->ReleaseFloatArrayElements(env, kernel, fkernel, 0);
	
	return i;
}

JNIEXPORT jint JNICALL Java_in_waslos_kneo_libsimplefft_FFTProcessor_createFastConvolutionContext__I_3D(JNIEnv* env, jclass object, jint samples, jdoubleArray kernel){
	
}

JNIEXPORT jint JNICALL Java_in_waslos_kneo_libsimplefft_FFTProcessor_createFastConvolutionContext__I_3S(JNIEnv* env, jclass object, jint samples, jshortArray kernel){

}

JNIEXPORT void JNICALL Java_in_waslos_kneo_libsimplefft_FFTProcessor_performFastConvolution__I_3F(JNIEnv* env, jclass object, jint handle, jfloatArray signal){

	if(!conv_handles) return;
	
	if(conv_handles->c_handles>0 && conv_handles->storage[handle] != NULL){
		jsize len = (*env)->GetArrayLength(env,signal);
		
		CONVOLUTION_CONTEXT* context = conv_handles->storage[handle];
		CPLX_SAMPLES csignal;
		
		int copy_switch = 0;
		float* re = (float*) (*env)->GetFloatArrayElements(env, signal, 0);
		
		float tre[context->samples];
		float tim[context->samples];
		
		memset(tre,0,sizeof(tre));
		memset(tim,0,sizeof(tim));
		
		if(len == context->samples){			
			csignal.re = re;
			csignal.im = &tim;
		} else {
			copy_switch=1;

			uint32_t length;
			if(context->samples > len){
				length = context->samples;
			} else {
				length = len;
			}
			
			int i=0;
			for(;i<length;i++){
				tre[i] = re[i];
				tim[i] = 0;
			}
			
			csignal.re = &tre;
			csignal.im = &tim;
		}
		
		csignal.length = context->samples;
		csignal.type   = CPLX_TYPE_SP;
		
		lsfft_perform_convolution(context,&csignal);
		
		if(copy_switch){
			uint32_t length;
			
			if(context->samples > len){
				length = context->samples;
			} else {
				length = len;
			}
			
			int i=0;
			
			for(;i<length;i++){
				re[i] = ((float*)csignal.re)[i];
			}
		}
		
		(*env)->ReleaseFloatArrayElements(env, signal,  re, 0);
	}
}

JNIEXPORT void JNICALL Java_in_waslos_kneo_libsimplefft_FFTProcessor_performFastConvolution__I_3D(JNIEnv* env, jclass object, jint handle, jdoubleArray signal){

}

JNIEXPORT void JNICALL Java_in_waslos_kneo_libsimplefft_FFTProcessor_performFastConvolution__I_3S(JNIEnv* env, jclass object, jint handle, jshortArray signal){

}

JNIEXPORT void JNICALL Java_in_waslos_kneo_libsimplefft_FFTProcessor_destroyFastConvolution(JNIEnv* env, jclass object, jint handle){

}

