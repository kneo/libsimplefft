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

#ifndef UTIL_H
#define UTIL_H

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

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

/**Base n vector incrementation. 
*@param result pointer to the vector to increment 
*@param skip_mask pointer to the vector containing the skip mask (places to ignore). Might be NULL.
*@param base the base of the number system the result vector is representing
*@param dimension of the vector.
*@return FLAG_OVERFLOW if operation overflows result then contains zeros only
*/
uint32_t inc_vector(uint32_t* result, uint32_t* skip_mask, uint32_t base, uint32_t dimension);

/**Shifts all places in the vector by one place from the least significant place to the most significant one
*@param vector to shift
*@param dimension of the vector
*@returns FLAG_OVERFLOW if most significant place dropps out of the vector
*/
uint32_t vector_lsh(uint32_t* vector, uint32_t dimension);

/**Shifts all places in the vector by one place from the most significant place to the least significant one
*@param vector to shift
*@param dimension of the vector
*@returns FLAG_CARRY if least significant place dropps out of the vector
*/
uint32_t vector_rsh(uint32_t* vector, uint32_t dimension);

/** Compute the memory vector from the linear counter value
*@param memory_vector to assign the the linear index to
*@param stride_array stride array for dimension strides
*@param value of the linear index to be assigned to the memory_vector
*@param dimension of the memory_vector
*/
void get_memory_vector(uint32_t* memory_vector, uint32_t* stride_array, int32_t value, uint32_t dimension);

/**Compute the index of the counter array within the linear memory block
*@param memory_vector to calculate the linear index from
*@param stride_array dimension stride array of the memory layout
*@param dimension of the vector
*@return the linear index of the memory cell in the memory layout represented by the memory vector
*/
uint32_t get_memory_index(uint32_t* memory_vector, uint32_t* stride_array, uint32_t dimension);

/**print sample buffer to stdout for debugging purposes
*@param samples samples to print
*/
void lsfft_printl_samples(CPLX_SAMPLES* samples);

#endif
