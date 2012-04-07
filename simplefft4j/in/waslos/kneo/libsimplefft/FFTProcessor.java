package in.waslos.kneo.libsimplefft;

public class FFTProcessor{
	public static final byte FFT_MODE_NORMAL  = 0;
	public static final byte FFT_MODE_INVERSE = 1;

	public static final byte CPLX_TYPE_SP  = 0;
	public static final byte CPLX_TYPE_DP  = 1;
	public static final byte CPLX_TYPE_INT = 2;

	static{
		System.loadLibrary("simplefft4j");
	}

	private native int createFFTContext(int samples, byte mode, byte type);
	private native void destroyFFTContext(int handle);

	private native void performFFTf(int handle, float[]  re, float[]  im);
	private native void performFFTd(int handle, double[] re, double[] im);
	private native void performFFTi(int handle, short[]  re, short[]  im);


	public static int initializeFFT(int samples,byte mode, byte type){
		return 0;
	}

	public static void destroyFFT(int handle){

	}

	public static void performFFT(int handle, float[] re, float[] im){

	}

	public static void performFFT(int handle, double[] re, double[] im){

	}

	public static void performFFT(int handle, short[] re, short[] im){

	}
}
