CC=g++
CFLAGS=-c -Wall
LFLAGS=-lz -Wall

# default
all: cmatrix.o gzip.o util.o ascii86.o mops.o

# everything that is from other dirs and not coded here
stuff: cmatrix.o gzip.o util.o ascii86.o

# matrix class
cmatrix.o: src/cmatrix.cpp src/cmatrix.hpp
	$(CC) $(CFLAGS) src/cmatrix.cpp -o src/cmatrix.o

# gzip wrapper
gzip.o: src/gzip.cpp src/gzip.hpp
	$(CC) $(CFLAGS) src/gzip.cpp -o src/gzip.o

# various useful functions
util.o: src/util.cpp src/util.hpp
	$(CC) $(CFLAGS) src/util.cpp -o src/util.o

# ascii85 implementation
ascii86.o: src/ascii86.cpp src/ascii86.hpp
	$(CC) $(CFLAGS) src/ascii86.cpp -o src/ascii86.o

# matrix operations, has the matrix encryption functions
mops.o: src/mops.cpp src/mops.hpp
	$(CC) $(CFLAGS) src/mops.cpp -o src/mops.o

test: all src/test.cpp
	$(CC) $(LFLAGS) src/cmatrix.o src/gzip.o src/util.o src/ascii86.o src/mops.o src/test.cpp -o bin/test

clean:
	rm -f src/*.o
