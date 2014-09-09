/*
*This is a part of libsimplefft
*
* Copyright (C) 2013 Kevin Krüger (kkevin@gmx.net)
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

package com.github.kneo.libsimplefft;

public class FFT{
	/**Initialize a new FFT context. It returns a handle identifier used to perform actions on or destroy the created data structures.
	*Performance Hint: even though there are overloaded functions ready to be used, only the matching data types should be concidered optimal.
	*if the data provided does not match the FFT data type, a cast to the matching type is performed.
	*@param dimension dimension of the FFT (mind your computer resources! Explicit: HANDLE WITH CARE! The memory usage grows exponentially!)
	*@param fftSize size of the FFT has to be a power of two!
	*@param dataType precision type of the FFT (as there are integer and floating point data types)
	*@param inverse tell if the FFT assigned to this handle should be inverse or normal
	*@return integer identifying the FFT context structure im memory
	*/
	public static native int init(int dimension,int fftSize, int dataType, boolean inverse);

	/**Performs the FFT with the data provided. If data type does not match a cast is performed before and after the operation.
	*Mind this as an performance bottleneck. Always use matching data types used with FFT initialization.
	*@param handle id of the FFT context retrieved by init().
	*@param re real part of the data
	*@param im imaginary part of the data 
	*/
	public static native void perform(int handle, byte[]   re, byte[]   im);
	/**Performs the FFT with the data provided. If data type does not match a cast is performed before and after the operation.
	*Mind this as an performance bottleneck. Always use matching data types used with FFT initialization.
	*@param handle id of the FFT context retrieved by init().
	*@param re real part of the data
	*@param im imaginary part of the data 
	*/
	public static native void perform(int handle, short[]  re, short[]  im);
	/**Performs the FFT with the data provided. If data type does not match a cast is performed before and after the operation.
	*Mind this as an performance bottleneck. Always use matching data types used with FFT initialization.
	*@param handle id of the FFT context retrieved by init().
	*@param re real part of the data
	*@param im imaginary part of the data 
	*/
	public static native void perform(int handle, int[]    re, int[]    im);
	/**Performs the FFT with the data provided. If data type does not match a cast is performed before and after the operation.
	*Mind this as an performance bottleneck. Always use matching data types used with FFT initialization.
	*@param handle id of the FFT context retrieved by init().
	*@param re real part of the data
	*@param im imaginary part of the data 
	*/
	public static native void perform(int handle, float[]  re, float[]  im);
	/**Performs the FFT with the data provided. If data type does not match a cast is performed before and after the operation.
	*Mind this as an performance bottleneck. Always use matching data types used with FFT initialization.
	*@param handle id of the FFT context retrieved by init().
	*@param re real part of the data
	*@param im imaginary part of the data 
	*/
	public static native void perform(int handle, double[] re, double[] im);

	/**Destroy a FFT context in memory. Do not use a handle destroyed with this function.
	*/
	public static native void destroy(int handle);

	/**Use this to request a single floating point precision FFT
	*/
	public static final byte SINGLE_PRECISION		= 0;

	/**Use this to request a double floating point precision FFT
	*/
	public static final byte DOUBLE_PRECISION		= 1;

	/**Use this to request a 16 Bit integer FFT
	*/
	public static final byte INTEGER_16_BIT			= 2;

	/**Use this to request a 8 Bit integer FFT
	*/
	public static final byte INTEGER_8_BIT			= 3;

	/**Use this to request a 32 Bit integer FFT
	*/
	public static final byte INTEGER_32_BIT			= 4;
}
