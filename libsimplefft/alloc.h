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

#ifndef ALLOC_H
#define ALLOC_H

#include "libsimplefft.h"

//allocating functions

/**allocates a buffer of complex numbers of type where type is one of the CPLX_TYPE_ defines above
*/
CPLX_SAMPLES* lsfft_alloc_complex_buffer(uint32_t samples, uint8_t type);

/**allocates a buffer of complex numbers of type where type is one of the CPLX_TYPE_ defines above
*Its purpose is to allocate buffers for multi dimensional FFTs
*/
CPLX_SAMPLES* lsfft_alloc_complex_buffer_md(uint32_t samples, uint8_t type, uint32_t dimensions);

/**frees a complex number buffer
*/
void lsfft_free_complex_buffer(CPLX_SAMPLES* buffer);

#endif		
