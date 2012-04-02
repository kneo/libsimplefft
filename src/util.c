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

#include "util.h"

uint32_t exp_2(uint32_t num){
	return 1<<num;
}

uint32_t log_2(uint32_t num){
	if(num>0){
		unsigned int log = 0;
		
		while(num>1){
			num=num>>1;
			log++;
		}
		
		return log;
	}
}

uint8_t bitcount(uint32_t num){
	if(num>0){
		uint8_t bits = 0;
		while(num>0){
			num=num>>1;
			if(num&1>0){
				bits++;
			}
		}		
		return bits;
	}
}

float re_mul_f(float re1,float im1, float re2,float im2){
	return re1*re2 - im1*im2;
}

float im_mul_f(float re1,float im1, float re2,float im2){
	return re1*im2 + im1*re2;
}

double re_mul_d(double re1,double im1, double re2,double im2){
	return re1*re2 - im1*im2;
}

double im_mul_d(double re1,double im1, double re2,double im2){
	return re1*im2 + im1*re2;
}

int16_t re_mul_i(int16_t re1,int16_t im1, float re2,float im2){
	return re1*re2 - im1*im2;
}

int16_t im_mul_i(int16_t re1,int16_t im1, float re2,float im2){
	return re1*im2 + im1*re2;
}


uint32_t bit_reversal(uint32_t base,uint32_t num){
	uint32_t res = 0;
	
	if(bitcount(base)==1 && base > num && num > 0){
	
		uint32_t count = log_2(base);
		
		//printf("count -> %d\n",count);
		
		uint32_t rev = 0;
		uint32_t i   = 0;
		
		for(;i<count;i++){
			rev |= (((1 << i) & num) != 0) ? (1 << (count-i-1)) : 0; // write each bit reverse
			//printf("\trev -> %d\n",rev);
		}
		
		return rev;
	
	}
	return 0;
}


void lsfft_printl_samples(CPLX_SAMPLES* samples){
	if(samples){
		uint32_t i = 0;
		for(;i<samples->length;i++){
			switch(samples->type){
				case CPLX_TYPE_SP:
					printf("%f + %fi\n",((float*)samples->re)[i],((float*)samples->im)[i]);
				break;
				case CPLX_TYPE_DP:
					printf("%f + %fi\n",((double*)samples->re)[i],((double*)samples->im)[i]);
				break;
				case CPLX_TYPE_INT:
					printf("%d + %di\n",((int16_t*)samples->re)[i],((int16_t*)samples->im)[i]);
				break;
			}		
		}
	}
}
