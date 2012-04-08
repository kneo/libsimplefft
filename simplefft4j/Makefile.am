lib_LTLIBRARIES = libsimplefft4j.la

libsimplefft4j_la_LIBADD = ../libsimplefft/.libs/libsimplefft.so
libsimplefft4j_la_SOURCES = f4jdefines.h in.waslos_kneo_libsimplefft_FFTProcessor.c in.waslos_kneo_libsimplefft_FFTProcessor.h

libsimplefft4j_la_CPPFLAGS = -I$(top_srcdir)/libsimplefft -I$(JDK_HOME)include

libsimplefft4j_la_LDFLAGS = -lm --shared -fPIC -lsimplefft

