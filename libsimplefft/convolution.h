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

#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include "libsimplefft.h"

/**initialize a fast convolution using kernel.
*/
CONVOLUTION_CONTEXT* lsfft_init_convolution(CPLX_SAMPLES* kernel);

/**Create a fact convolution using kernel and pre initialized fft contexts
 * They will be created if not existing.
 */
CONVOLUTION_CONTEXT* lsfft_init_convolution_using_fft_context(FFT_CONTEXT* fft, FFT_CONTEXT* ifft, CPLX_SAMPLES* kernel);

/**perform a fast convolution using context and apply it to the signal.
*/
void lsfft_perform_convolution(CONVOLUTION_CONTEXT* context, CPLX_SAMPLES* signal);

/**free allocated context and data.
*/
void lsfft_destroy_covolution_context(CONVOLUTION_CONTEXT* context);

#endif
