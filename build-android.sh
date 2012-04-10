#!/bin/sh
# FourierTest/build.sh
# Compiles fftw3 for Android
# Make sure you have NDK_ROOT defined in .bashrc or .bash_profile

NDK_ROOT="/home/kneo/Documents/Android/android_sdk_ndk/android-ndk-r7b"

INSTALL_DIR="../jni/libsimplefft"
SRC_DIR="."

cd $SRC_DIR

export PATH="$NDK_ROOT/toolchains/arm-linux-androideabi-4.4.3/prebuilt/linux-x86/bin/:$PATH"
export SYS_ROOT="$NDK_ROOT/platforms/android-8/arch-arm/"
export CC="arm-linux-androideabi-gcc --sysroot=$SYS_ROOT"
export LD="arm-linux-androideabi-ld"
export AR="arm-linux-androideabi-ar"
export RANLIB="arm-linux-androideabi-ranlib"
export STRIP="arm-linux-androideabi-strip"

mkdir -p $INSTALL_DIR
./configure --host=arm-eabi --prefix=$INSTALL_DIR LIBS="-lc -lgcc"

make
make install

exit 0
