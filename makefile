CC=gcc
RM=rm
CP=cp

INSTALL_PREFIX=/usr/lib/

ARGS=--shared -fPIC -lm

libsimplefft.so: alloc.o util.o fft.o twiddle.o libsimplefft.o
	$(CC) $(ARGS) -o libsimplefft.so alloc.o util.o fft.o twiddle.o libsimplefft.o
	
libsimplefft.o:
	$(CC) $(ARGS) -o libsimplefft.o -c libsimplefft.c
	
twiddle.o:
	$(CC) $(ARGS) -o twiddle.o -c twiddle.c

util.o:
	$(CC) $(ARGS) -o util.o -c util.c
	
fft.o:
	$(CC) $(ARGS) -o fft.o -c fft.c
	
alloc.o:
	$(CC) $(ARGS) -o alloc.o -c alloc.c
	
clean:
	$(RM) -f *.o *.so
	
install: libsimplefft.so
	$(CP) -f libsimplefft.so $(INSTALL_PREFIX)
