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