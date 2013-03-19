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

package in.waslos.kneo.libsimplefft;

public class FFTProcessor{
	public static final byte FFT_MODE_INVERSE = 1;
	public static final byte FFT_MODE_NORMAL  = 0;

	public static final byte CPLX_TYPE_SP  = 0;
	public static final byte CPLX_TYPE_DP  = 1;
	public static final byte CPLX_TYPE_INT = 2;
	
	static{
		System.loadLibrary("simplefft");
		System.loadLibrary("simplefft4j");
	}

	private static native int createFFTContext(int samples, byte mode, byte type);
	private static native void destroyFFTContext(int handle);

	private static native void performFFTf(int handle, float[]  re, float[]  im);
	private static native void performFFTd(int handle, double[] re, double[] im);
	private static native void performFFTi(int handle, short[]  re, short[]  im);
	
	public static native int findFFTHandle(int type,int size,int mode);
	
	public static native int createFastConvolutionContext(int samples, float[] kernel_re,float[] kernel_im);
	public static native int createFastConvolutionContext(int samples, double[] kernel_re,double[] kernel_im);
	public static native int createFastConvolutionContext(int samples, short[] kernel_re,short[] kernel_im);
	
	public static native int getConvolutionSize(int handle);
	
	public static native int getTransformedConvolutionKernel(int handle, short[] kernel_re,short[] kernel_im);
	public static native int getTransformedConvolutionKernel(int handle, float[] kernel_re,float[] kernel_im);
	public static native int getTransformedConvolutionKernel(int handle, double[] kernel_re,double[] kernel_im);
	
	public static native void performFastConvolution(int handle, float[]  signal_re,float[]  signal_im);
	public static native void performFastConvolution(int handle, double[] signal_re,double[] signal_im);
	public static native void performFastConvolution(int handle, short[]  signal_re, short[] signal_im);
	
	public static native int destroyFastConvolution(int handle);

	public static int initializeFFT(int samples,byte mode, byte type){
		return FFTProcessor.createFFTContext(samples,mode,type);
	}

	public static void destroyFFT(int handle){
		FFTProcessor.destroyFFTContext(handle);
	}

	public static void performFFT(int handle, float[] re, float[] im){
		performFFTf(handle,re,im);
	}

	public static void performFFT(int handle, double[] re, double[] im){
		performFFTd(handle,re,im);
	}

	public static void performFFT(int handle, short[] re, short[] im){
		performFFTi(handle,re,im);
	}
	
	public static void main(String[] argV){
		//System.loadLibrary("simplefft");

		int size = 8;
	
		int handle = FFTProcessor.initializeFFT(size,FFTProcessor.FFT_MODE_NORMAL,FFTProcessor.CPLX_TYPE_INT);
		//int ihandle = FFTProcessor.initializeFFT(size,FFTProcessor.FFT_MODE_INVERSE,FFTProcessor.CPLX_TYPE_INT);
		System.out.println("handle retrieved : "+handle/*/+" inverse "+ihandle*/);

		short[] re = new short[size];
		short[] im = new short[size];
		
		for(int i = 0;i<size;i++){
			re[i] = (short)i;
		}
		
		short[] kernel_re = {-1,0,1};
		short[] kernel_im = {0, 0,0};
		
		int conhandle = FFTProcessor.createFastConvolutionContext(8,kernel_re,kernel_im);
		
		long time = System.currentTimeMillis();
		performFFT(handle,re,im);
		
		time = System.currentTimeMillis() - time;
		System.out.println("runtime : "+time+"ms");
		
		for(int i = 0;i<size;i++){
			System.out.printf("%d + %d * i\n",re[i],im[i]);
		}
		
		
		performFastConvolution(conhandle,re,im);
		
		for(int i = 0;i<size;i++){
			System.out.printf("%d + %d * i\n",re[i],im[i]);
		}
		
		int ihandle = FFTProcessor.initializeFFT(size,FFTProcessor.FFT_MODE_INVERSE,FFTProcessor.CPLX_TYPE_INT);
	}
}

