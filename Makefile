CC=g++
CFLAGS=-c -Wall -Wno-sign-compare
LFLAGS=-lz -Wall -Wno-sign-compare


# LIBRARIES
# all the libraries used
all: src/cmatrix.o src/gzip.o src/util.o src/ascii86.o src/mops.o

# everything that is from other dirs and not coded here
stuff: src/cmatrix.o src/gzip.o src/util.o src/ascii86.o

# matrix class
src/cmatrix.o: src/cmatrix.cpp src/cmatrix.hpp
	$(CC) $(CFLAGS) src/cmatrix.cpp -o src/cmatrix.o

# gzip wrapper
src/gzip.o: src/gzip.cpp src/gzip.hpp
	$(CC) $(CFLAGS) src/gzip.cpp -o src/gzip.o

# various useful functions
src/util.o: src/util.cpp src/util.hpp
	$(CC) $(CFLAGS) src/util.cpp -o src/util.o

# ascii85 implementation
src/ascii86.o: src/ascii86.cpp src/ascii86.hpp
	$(CC) $(CFLAGS) src/ascii86.cpp -o src/ascii86.o

# matrix operations, has the matrix encryption functions
src/mops.o: src/mops.cpp src/mops.hpp
	$(CC) $(CFLAGS) src/mops.cpp -o src/mops.o


# TESTS
# compile tests
src/test.o: src/test.cpp
	$(CC) $(CFLAGS) src/test.cpp -o src/test.o

# make a binary for test
bin/test: all src/test.o
	$(CC) $(LFLAGS) src/cmatrix.o src/gzip.o src/util.o src/ascii86.o src/mops.o src/test.o -o bin/test 

# ascii86 tests
src/ascii86_tests: src/ascii86_tests.cpp
	$(CC) $(CFLAGS) src/ascii86_tests.cpp -o src/ascii86_tests.o

# make tests for ascii86
bin/ascii86_tests: all src/ascii86_tests.o
	$(CC) $(LFLAGS) src/ascii86.o src/ascii86_tests.o -o bin/ascii86_tests 


# BINARIES
# compile the final product
src/mcrypt.o: src/mcrypt.cpp src/mcrypt.hpp
	$(CC) $(CFLAGS) src/mcrypt.cpp -o src/mcrypt.o

# make a binary for mcrypt
bin/mcrypt: all src/mcrypt.o
	$(CC) $(LFLAGS) src/cmatrix.o src/gzip.o src/util.o src/ascii86.o src/mops.o src/mcrypt.o -o bin/mcrypt


estimate:
	wc Makefile src/*.cpp src/*.hpp

clean:
	rm -f src/*.o
