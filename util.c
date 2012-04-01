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