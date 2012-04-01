#ifndef DEFINES_H
#define DEFINES_H

#include<time.h>

struct CPLX_FLOAT;
struct CPLX_DOUBLE;
struct CPLX_INTEGER;
struct CPLX_SAMPLES;

/**Type holding a buffer of complex samples
*/
typedef struct CPLX_SAMPLES{
	uint8_t type;
	void* buffer;
	
	clock_t exec_time;
	
	void* re;
	void* im;
	
	uint32_t length;
}CPLX_SAMPLES;

/**Marker for a normal time domain to frequency domain transform
*/
#define FFT_MODE_NORMAL  (0)

/**Marker for a inverse frequency domain to time domain transform
*/
#define FFT_MODE_INVERSE (1)

/**marker for single precision fft
*/
#define CPLX_TYPE_SP  (0)
/**marker for double precision fft
*/
#define CPLX_TYPE_DP  (1)
/**marker for integer based fft
*/
#define CPLX_TYPE_INT (2)

struct FFT_CONTEXT;

typedef struct FFT_CONTEXT{
	uint8_t type;
	uint8_t mode;
	uint32_t samples;
	uint32_t stages;
	CPLX_SAMPLES* twiddle_factors;
}FFT_CONTEXT;

#endif
