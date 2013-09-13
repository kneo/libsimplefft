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
	return 0;
}

uint8_t bitcount(uint32_t num){
	if(num>0){
		uint8_t bits = 0;
		while(num>0){
			num=num>>1;
			if((num&1)>0){
				bits++;
			}
		}
		return bits;
	}
	return 0;
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

int8_t re_mul_b(int8_t re1,int8_t im1, float re2,float im2){
	return (int8_t) ((re1)*re2 - ((im1)*im2));
}

int8_t im_mul_b(int8_t re1,int8_t im1, float re2,float im2){
	return (int8_t) ((re1)*im2 + ((im1)*re2));
}

int16_t re_mul_i(int16_t re1,int16_t im1, float re2,float im2){
	return (int16_t) ((((float)re1)*re2 - ((float)im1)*im2));
}

int16_t im_mul_i(int16_t re1,int16_t im1, float re2,float im2){
	return (int16_t) ((((float)re1)*im2 + ((float)im1)*re2));
}

int32_t re_mul_i32(int32_t re1,int32_t im1, float re2,float im2){
	return (int32_t) ((((float)re1)*re2 - ((float)im1)*im2));
}

int32_t im_mul_i32(int32_t re1,int32_t im1, float re2,float im2){
	return (int32_t) ((((float)re1)*im2 + ((float)im1)*re2));
}

uint32_t bit_reversal(uint32_t base,uint32_t num){
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

//This function is an addition with carry, which can be provided with a skip mask
//to tell
uint32_t inc_vector(uint32_t* result, uint32_t* skip_mask, uint32_t base, uint32_t dimension){
	uint32_t lsb   = 0; //
	uint32_t carry = 0;

	do{
		start:
			if(lsb >= dimension){ //prevent overflow and possible buffer overrun
				return 1;
			}

			if(skip_mask && skip_mask[lsb])	{ //if there is a skip mask, skip this place and start over
				lsb++;
				goto start;
			}

			result[lsb] = (result[lsb]+1)%base; //add one add limit this place to be base max
			carry = result[lsb]==0; //if it returns 0 addition resulted a carry

			lsb=lsb+((1 & result[lsb])==0); // if there was a carry increment the lsb counter for the next iteration

	}while(carry && lsb < dimension); // while there is a carry redo loop

	return FLAG_OVERFLOW * carry; // if one the vector is overflown... use it to break a loop or something
}


uint32_t vector_lsh(uint32_t* vector, uint32_t dimension){
	uint32_t i;
	uint32_t flag=0;

	if(dimension>0){
		if(vector[dimension-1] != 0) flag = FLAG_OVERFLOW;
		//printf("%d, flag: %d\n",vector[dimension-1],flag);


		for(i = dimension-1; i > 0; i--){
			//printf("%d\n",vector[i]);
			vector[i] = vector[i - 1];
		}
		vector[0] = 0;
	}

	return flag;
}

uint32_t vector_rsh(uint32_t* vector, uint32_t dimension){
	uint32_t i;
	uint32_t flag=0;

	if(dimension>0){
		if(vector[0] != 0) flag = FLAG_CARRY;
		printf("%d, flag: %d\n",vector[dimension-1],flag);


		for(i = 0; i < dimension; i++){
			printf("%d\n",vector[i]);
			vector[i] = vector[i + 1];
		}
		vector[dimension-1] = 0;
	}

	return flag;
}


uint32_t get_memory_index(uint32_t* memory_vector, uint32_t* stride_array, uint32_t dimension){
	uint32_t i = 0;
	uint32_t res = 0;

	for(;i<dimension;i++){
		//printf("stride %d\n",stride_array[i]);
		res = res + memory_vector[i] * stride_array[i];
	}

	return res;
}

void get_memory_vector(uint32_t* memory_vector, uint32_t* stride_array, int32_t value, uint32_t dimension){
	int32_t i;
	uint32_t frac = value;
	//printf("dim :%d\n",dimension);

	for(i=dimension-1; i>-1; i--){
		//printf("dimension :%d\n",i);
		memory_vector[i] = frac / stride_array[i];
		frac  %= stride_array[i];
		//printf("frac: %d\n",frac);
	}
}

void lsfft_printl_samples(CPLX_SAMPLES* samples){
	if(samples){
		uint32_t i = 0;

		printf("\n");
		for(;i<samples->length;i++){
			if(i>0 && (i % samples->base_length == 0)){
				printf("\n");
				if(i>0 && (i % (samples->base_length*samples->base_length) == 0)){
					printf("\n");
				}
			}

			switch(samples->type){
				case CPLX_TYPE_SP:
					printf("%.2f + %.2fi ",((float*)samples->re)[i],((float*)samples->im)[i]);
				break;
				case CPLX_TYPE_DP:
					printf("%.2f + %.2fi ",((double*)samples->re)[i],((double*)samples->im)[i]);
				break;
				case CPLX_TYPE_INT:
					printf("%d + %di ",((int16_t*)samples->re)[i],((int16_t*)samples->im)[i]);
				break;
				case CPLX_TYPE_INT32:
					printf("%d + %di ",((int32_t*)samples->re)[i],((int32_t*)samples->im)[i]);
				break;
				case CPLX_TYPE_BYTE:
					printf("%d + %di ",((int8_t*)samples->re)[i],((int8_t*)samples->im)[i]);
				break;
			}
		}
		printf("\n");
	}
}
