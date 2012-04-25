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
#ifndef F4JDEFINES_H
#define F4JDEFINES_H

#define MAX_FFT_HANDLES  (100)
#define MAX_CONV_HANDLES (100)

#include<stdint.h>
#include<stdlib.h>
#include<stdio.h>
#include"../libsimplefft/defines.h"

typedef struct FFT_HANDLES{
	uint32_t c_handles;
	FFT_CONTEXT* storage[MAX_FFT_HANDLES];
}FFT_HANDLES;

typedef struct CONVOLUTION_HANDLES{
	uint32_t c_handles;
	CONVOLUTION_CONTEXT* storage[MAX_CONV_HANDLES];
}CONVOLUTION_HANDLES;

#endif
