rm -rf *.o *.so *.py *.pyc
swig -python sample.i
gcc -c sample_wrap.c -fpic -I/sw/packages/python/2.7.6/include/python2.7
gcc -c sample.c -fpic
gcc -shared -o libsample.so sample.o
gcc -shared -o _sample.so sample_wrap.o -lsample -L/ws/jkolanje-sjc/sandbox/swig
