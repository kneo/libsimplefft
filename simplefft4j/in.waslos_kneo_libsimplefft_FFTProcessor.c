#include "f4jdefines.h"

#include "../libsimplefft/libsimplefft.h"
#include "in_waslos_kneo_libsimplefft_FFTProcessor.h"

static FFT_HANDLES* fft_handles;

JNIEXPORT jint JNICALL Java_in_waslos_kneo_libsimplefft_FFTProcessor_createFFTContext(JNIEnv * env, jobject class, jint samples, jbyte mode, jbyte type){
	printf("initializing fft...\n");
	if(!fft_handles){
		printf("first initialization allocating fft context\n");
		fft_handles = (FFT_HANDLES*)calloc(1,sizeof(FFT_HANDLES));
	}
	
	if(fft_handles->c_handles<MAX_FFT_HANDLES){
		FFT_CONTEXT* context = lsfft_init(samples,type,mode);
		
		if(!context) return -1;
		
		uint32_t handle = fft_handles->c_handles;
		fft_handles->storage[handle] = context;
		fft_handles->c_handles++;
		
		return handle;
	}
	
	return -1;
}

JNIEXPORT void JNICALL Java_in_waslos_kneo_libsimplefft_FFTProcessor_destroyFFTContext(JNIEnv * env, jobject class, jint handle){
	if(fft_handles->c_handles>handle && fft_handles->storage[handle] != NULL){
	
		lsfft_destroy_context(fft_handles->storage[handle]);
		
		fft_handles->storage[handle]=NULL;
	}

}

JNIEXPORT void JNICALL Java_in_waslos_kneo_libsimplefft_FFTProcessor_performFFTf(JNIEnv * env, jobject class, jint handle, jfloatArray re, jfloatArray im){
	printf("performing FFT...");
	if(fft_handles->c_handles > handle && fft_handles->storage[handle] != NULL){
		printf("all set!\n");
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
