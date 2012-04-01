#ifndef UTIL_H
#define UTIL_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "libsimplefft.h"

/**calculate the base 2 logarithm of num
*@param num 
*@return 
*/
uint32_t log_2(uint32_t num);

/**calculate the numth power of 2
*@param num exponent to compute the power of 2 from needs to be greater than 0
*@return 
*/
uint32_t exp_2(uint32_t num);

/**count setted bits of an integer. Indicator if num its a power of 2
*@param num bit pattern to count
*@return number of bits set to 1
*/
uint8_t bitcount(uint32_t num);

/**Real part of a multiplication of two complex numbers (single precision)
*@param re1 real part of the first number
*@param im1 imaginary part of the first number
*@param re2 real part of the second number
*@param im2 imaginary part of the second number
*@return real part of the result
*/
float re_mul_f(float re1,float im1, float re2,float im2);

/**Real part of a multiplication of two complex numbers (single precision)
*@param re1 real part of the first number
*@param im1 imaginary part of the first number
*@param re2 real part of the second number
*@param im2 imaginary part of the second number
*@return imaginary part of the result
*/
float im_mul_f(float re1,float im1, float re2,float im2);

/**Real part of a multiplication of two complex numbers (double precision)
*@param re1 real part of the first number
*@param im1 imaginary part of the first number
*@param re2 real part of the second number
*@param im2 imaginary part of the second number
*@return real part of the result
*/
double re_mul_d(double re1,double im1, double re2,double im2);

/**Real part of a multiplication of two complex numbers (double precision)
*@param re1 real part of the first number
*@param im1 imaginary part of the first number
*@param re2 real part of the second number
*@param im2 imaginary part of the second number
*@return imaginary part of the result
*/
double im_mul_d(double re1,double im1, double re2,double im2);

/**Real part of a multiplication of two complex numbers (mixed precision)
*@param re1 real part of the first number
*@param im1 imaginary part of the first number
*@param re2 real part of the second number
*@param im2 imaginary part of the second number
*@return real part of the result
*/
int16_t re_mul_i(int16_t re1,int16_t im1, float re2,float im2);

/**Real part of a multiplication of two complex numbers (mixed precision)
*@param re1 real part of the first number
*@param im1 imaginary part of the first number
*@param re2 real part of the second number
*@param im2 imaginary part of the second number
*@return imaginary part of the result
*/
int16_t im_mul_i(int16_t re1,int16_t im1, float re2,float im2);


/**computes the bitreversal of num to the base base
*base = 8 -> 1000
*num    3 -> 0011
*result 6 -> 0110
*@param base limiter to avoid num exceeding this base
*@param num number to be 
*@return the bit reverse of num
*/
uint32_t bit_reversal(uint32_t base,uint32_t num);

/**print sample buffer to stdout for debugging purposes
*@param samples samples to print
*/
void lsfft_printl_samples(CPLX_SAMPLES* samples);

#endif
