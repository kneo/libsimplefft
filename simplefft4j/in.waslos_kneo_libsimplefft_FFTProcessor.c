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
	if(fft_handles>handle){
			
	}

}

JNIEXPORT void JNICALL Java_in_waslos_kneo_libsimplefft_FFTProcessor_performFFTf(JNIEnv * env, jobject class, jint handle, jfloatArray re, jfloatArray im){
	if(fft_handles>handle){
		
	}
}

JNIEXPORT void JNICALL Java_in_waslos_kneo_libsimplefft_FFTProcessor_performFFTd(JNIEnv * env, jobject class, jint handle, jdoubleArray re, jdoubleArray im){

}

JNIEXPORT void JNICALL Java_in_waslos_kneo_libsimplefft_FFTProcessor_performFFTi(JNIEnv * env, jobject class , jint handle, jshortArray re, jshortArray im){

}
