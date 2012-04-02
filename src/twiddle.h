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
