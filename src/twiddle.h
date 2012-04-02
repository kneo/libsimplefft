#ifndef TWIDDLE_H
#define TWIDDLE_H

#include "libsimplefft.h"

/** Computes the unitary root (twiddle) factors for a FFT of size samples.
* All factors a stored in an array of half size of the FFT.
* @param Samples of the FFT, this function initializes an array of the half size of samples
* @param type data type of the fft, single, double, integer precision
* @param mode inverse or normal FFT
* @return a samples buffer with precalculated twiddle factors
*/
CPLX_SAMPLES* compute_twiddles(uint32_t samples,uint8_t type, uint8_t mode);

#endif
