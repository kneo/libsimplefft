#ifndef LIBSIMPLEFFT_H
#define LIBSIMPLEFFT_H

#include<stdint.h>
#include<stdlib.h>
#include<stdio.h>
#include<math.h>

#include "defines.h"

#include "util.h"
#include "alloc.h"
#include "twiddle.h"

#include "fft.h"

/**Initializes a FFT context with samples and number type and mode
*Specify the number of samples the FFT should perform. The more samples the better the frequency resolution is provided
*Use the type and mode markers above to initialize the desired Fast Fourier Transform
*Use:
*CPLX_TYPE_SP to perform a single precision FFT
*CPLX_TYPE_DP to perform a double precision FFT
*CPLX_TYPE_INT to perform an integer precision FFT (should be the fastest, may be used to process raw signed PCM 16 data)
*/
FFT_CONTEXT* lsfft_init(uint32_t samples, uint8_t type, uint8_t mode);

/**Deallocates a initialized FFT_CONTEXT and frees assigned twiddle factor buffer
*@param context buffer to be deallocated
*/
void lsfft_destroy_context(FFT_CONTEXT* context);

#endif
