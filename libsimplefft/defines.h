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

#ifndef DEFINES_H
#define DEFINES_H

#include<time.h>

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

/**Holds data required to perform a FFT.
*/
struct FFT_CONTEXT;

typedef struct FFT_CONTEXT{
	uint8_t type;
	uint8_t mode;
	uint32_t samples;
	uint32_t stages;
	CPLX_SAMPLES* twiddle_factors;
	uint32_t* bit_rev_indices;
}FFT_CONTEXT;


/**Holds the data required to perform a fast colvolution.
*/
struct CONVOLUTION_CONTEXT;
typedef struct CONVOLUTION_CONTEXT{
	uint8_t type;
	FFT_CONTEXT* fft_context;
	FFT_CONTEXT* ifft_context;
	uint32_t samples;
	CPLX_SAMPLES* kernel;
}

#endif
