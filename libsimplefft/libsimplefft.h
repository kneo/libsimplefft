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

#include "convolution.h"

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
