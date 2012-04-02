#ifndef FFT_H
#define FFT_H

#include "libsimplefft.h"


/**Perform the fast fourier transform with assigned context to passed buffer
* According to the initialization of the FFT the choice between inverse and normal has already been made.
* So there is no fft or ifft function.
* The transform how ever performes inplace with pre-computed twiddle factors, so the result is stored in the buffer.
* Remind that before performing FFT on data not supposed to be transformed
* @param context of the FFT, its been generated by calling the init method
* @param buffer of samples ready to be transformed. This is an inplace operation so the result will be stored within the same buffer
*/
void lsfft_perform(FFT_CONTEXT* context, CPLX_SAMPLES* buffer);

#endif