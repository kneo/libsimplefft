#ifndef F4JDEFINES_H
#define F4JDEFINES_H

#define MAX_FFT_HANDLES (100)

typedef struct FFT_HANDLES{
	uint32_t c_handles;
	void* storage[MAX_FFT_HANDLES];
}FFT_HANDLES;

extern static FFT_HANDLES* fft_handles;

#endif
