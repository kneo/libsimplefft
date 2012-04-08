#ifndef F4JDEFINES_H
#define F4JDEFINES_H

#define MAX_FFT_HANDLES (100)

#include<stdint.h>
#include<stdlib.h>
#include<stdio.h>
#include"../libsimplefft/defines.h"

typedef struct FFT_HANDLES{
	uint32_t c_handles;
	FFT_CONTEXT* storage[MAX_FFT_HANDLES];
}FFT_HANDLES;

#endif
