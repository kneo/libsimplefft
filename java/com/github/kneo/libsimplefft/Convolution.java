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

public class Convolution{
	/**Create a new Convolution context using dimension and fftSize. The data types will be choose by the delivered type of data.
	*Existing FFT contexts may be shared with the convolution for optimal memory management.
	*@param dimension dimension of the convolution
	*@param fftSize size of the founding FFTs. Has to be a power of two.
	*param kernelRe real part of the filter kernel. May be smaller than fftSize but not greater!
	*param kernelIm imaginary part of the filter kernel. May be smaller than fftSize but not greater!
	*@return integer identifier for this convolution
	*/
	public static native int init(int dimension, int fftSize, byte[]   kernelRe, byte[]   kernelIm);
	/**Create a new Convolution context using dimension and fftSize. The data types will be choose by the delivered type of data.
	*Existing FFT contexts may be shared with the convolution for optimal memory management.
	*@param dimension dimension of the convolution
	*@param fftSize size of the founding FFTs. Has to be a power of two.
	*param kernelRe real part of the filter kernel. May be smaller than fftSize but not greater!
	*param kernelIm imaginary part of the filter kernel. May be smaller than fftSize but not greater!
	*@return integer identifier for this convolution
	*/
	public static native int init(int dimension, int fftSize, short[]  kernelRe, short[]  kernelIm);
	/**Create a new Convolution context using dimension and fftSize. The data types will be choose by the delivered type of data.
	*Existing FFT contexts may be shared with the convolution for optimal memory management.
	*@param dimension dimension of the convolution
	*@param fftSize size of the founding FFTs. Has to be a power of two.
	*param kernelRe real part of the filter kernel. May be smaller than fftSize but not greater!
	*param kernelIm imaginary part of the filter kernel. May be smaller than fftSize but not greater!
	*@return integer identifier for this convolution
	*/
	public static native int init(int dimension, int fftSize, int[]    kernelRe, int[]    kernelIm);
	/**Create a new Convolution context using dimension and fftSize. The data types will be choose by the delivered type of data.
	*Existing FFT contexts may be shared with the convolution for optimal memory management.
	*@param dimension dimension of the convolution
	*@param fftSize size of the founding FFTs. Has to be a power of two.
	*param kernelRe real part of the filter kernel. May be smaller than fftSize but not greater!
	*param kernelIm imaginary part of the filter kernel. May be smaller than fftSize but not greater!
	*@return integer identifier for this convolution
	*/
	public static native int init(int dimension, int fftSize, float[]  kernelRe, float[]  kernelIm);
	/**Create a new Convolution context using dimension and fftSize. The data types will be choose by the delivered type of data.
	*Existing FFT contexts may be shared with the convolution for optimal memory management.
	*@param dimension dimension of the convolution
	*@param fftSize size of the founding FFTs. Has to be a power of two.
	*param kernelRe real part of the filter kernel. May be smaller than fftSize but not greater!
	*param kernelIm imaginary part of the filter kernel. May be smaller than fftSize but not greater!
	*@return integer identifier for this convolution
	*/
	public static native int init(int dimension, int fftSize, double[] kernelRe, double[] kernelIm);

	/**Perform the actual fast convolution operation on the data assigned.
	*@param handle id of the Convolution context
	*@param signalRe real part of the signal
	*@param signalIm imaginary part of the signal
	*/
	public static native void perform(int handle, byte[]   signalRe, byte[]   signalIm);
	/**Perform the actual fast convolution operation on the data assigned.
	*@param handle id of the Convolution context
	*@param signalRe real part of the signal
	*@param signalIm imaginary part of the signal
	*/
	public static native void perform(int handle, short[]  signalRe, short[]  signalIm);
	/**Perform the actual fast convolution operation on the data assigned.
	*@param handle id of the Convolution context
	*@param signalRe real part of the signal
	*@param signalIm imaginary part of the signal
	*/
	public static native void perform(int handle, int[]    signalRe, int[]    signalIm);
	/**Perform the actual fast convolution operation on the data assigned.
	*@param handle id of the Convolution context
	*@param signalRe real part of the signal
	*@param signalIm imaginary part of the signal
	*/
	public static native void perform(int handle, float[]  signalRe, float[]  signalIm);
	/**Perform the actual fast convolution operation on the data assigned.
	*@param handle id of the Convolution context
	*@param signalRe real part of the signal
	*@param signalIm imaginary part of the signal
	*/
	public static native void perform(int handle, double[] signalRe, double[] signalIm);
	
	/**Destroy the convolution context. Do not use a destoyed handle for operation, except it's been returned by init.
	*@param handle of the convolution to be destroyed.
	*/
	public static native void destroy(int handle);
}
