CC=gcc
RM=rm
CP=cp
MKDIR=mkdir

INSTALL_PREFIX=/usr/lib/
INCLUDE_PREFIX=/usr/include/

ARGS=--shared -fPIC

libsimplefft.so: alloc.o util.o fft.o twiddle.o libsimplefft.o
	$(MKDIR) -p build/lib/
	$(MKDIR) -p build/include/libsimplefft/
	$(CC) $(ARGS) -lm -o build/lib/libsimplefft.so alloc.o util.o fft.o twiddle.o libsimplefft.o

	$(CP) -f src/*.h build/include/libsimplefft/
	
libsimplefft.o:
	$(CC) $(ARGS) -o libsimplefft.o -c src/libsimplefft.c
	
twiddle.o:
	$(CC) $(ARGS) -o twiddle.o -c src/twiddle.c

util.o:
	$(CC) $(ARGS) -o util.o -c src/util.c
	
fft.o:
	$(CC) $(ARGS) -o fft.o -c src/fft.c
	
alloc.o:
	$(CC) $(ARGS) -o alloc.o -c src/alloc.c
	
clean:
	$(RM) -f *.o build/include/libsimplefft/*.h build/lib/*.so
	
install: libsimplefft.so
	$(CP) -f build/lib/libsimplefft.so $(INSTALL_PREFIX)
	$(CP) -r -f build/include/libsimplefft/ $(INCLUDE_PREFIX)
