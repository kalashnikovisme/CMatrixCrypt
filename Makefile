CC=g++
CFLAGS=-c -Wall -Wno-sign-compare 
LFLAGS=-lz -Wall -Wno-sign-compare


# LIBRARIES
# all the libraries used
all: src/cmatrix.o src/deflate.o src/util.o src/ascii86.o src/mops.o

# everything that is from other dirs and not coded here
stuff: src/cmatrix.o src/deflate.o src/util.o src/ascii86.o

# matrix class
src/cmatrix.o: src/cmatrix.cpp src/cmatrix.hpp
	$(CC) $(CFLAGS) -o src/cmatrix.o src/cmatrix.cpp

# deflate wrapper
src/deflate.o: src/deflate.cpp src/deflate.hpp
	$(CC) $(CFLAGS) -o src/deflate.o src/deflate.cpp

# various useful functions
src/util.o: src/util.cpp src/util.hpp
	$(CC) $(CFLAGS) -o src/util.o src/util.cpp

# ascii85 implementation
src/ascii86.o: src/ascii86.cpp src/ascii86.hpp
	$(CC) $(CFLAGS) -o src/ascii86.o src/ascii86.cpp

# matrix operations, has the matrix encryption functions
src/mops.o: src/mops.cpp src/mops.hpp
	$(CC) $(CFLAGS) -o src/mops.o src/mops.cpp


# TESTS
# compile tests
src/test.o: src/test.cpp
	$(CC) $(CFLAGS) -o src/test.o src/test.cpp

# ascii86 tests
src/ascii86_tests.o: src/ascii86_tests.cpp
	$(CC) $(CFLAGS) -o src/ascii86_tests.o src/ascii86_tests.cpp

# deflate tests
src/deflate_tests.o: src/deflate_tests.cpp
	$(CC) $(CFLAGS) -o src/deflate_tests.o src/deflate_tests.cpp

# cmatrix tests
src/cmatrix_tests.o: src/cmatrix_tests.cpp
	$(CC) $(CFLAGS) -o src/cmatrix_tests.o src/cmatrix_tests.cpp

# util tests
src/util_tests.o: src/util_tests.cpp
	$(CC) $(CFLAGS) -o src/util_tests.o src/util_tests.cpp

# matrix operations tests
src/mops_tests.o: src/mops_tests.cpp
	$(CC) $(CFLAGS) -o src/mops_tests.o src/mops_tests.cpp

# make deflate86.o
src/deflate86.o: src/deflate86.cpp
	$(CC) $(CFLAGS) -o src/deflate86.o src/deflate86.cpp

# make inflate86.0
src/inflate86.o: src/inflate86.cpp
	$(CC) $(CFLAGS) -o src/inflate86.o src/inflate86.cpp


# TESTS - link the compiled tests
# make a binary for test
bin/test: all src/test.o
	$(CC) $(LFLAGS) -o bin/test src/cmatrix.o src/deflate.o src/util.o src/ascii86.o src/mops.o src/test.o

# make util test binaries
bin/util_tests: src/util.o src/util_tests.o
	$(CC) $(LFLAGS) -o bin/util_tests src/util.o src/util_tests.o

# make tests for matrix operations
bin/mops_tests: src/mops.o src/mops_tests.o
	$(CC) $(LFLAGS) -o bin/mops_tests src/mops.o src/mops_tests.o

# make cmatrix test binaries
bin/cmatrix_tests: src/cmatrix.o src/cmatrix_tests.o
	$(CC) $(LFLAGS) -o bin/cmatrix_tests src/cmatrix.o src/cmatrix_tests.o

# make tests for deflate
bin/deflate_tests: src/deflate.o src/deflate_tests.o
	$(CC) $(LFLAGS) -o bin/deflate_tests src/deflate.o src/deflate_tests.o

# make tests for ascii86
bin/ascii86_tests: src/ascii86.o src/ascii86_tests.o
	$(CC) $(LFLAGS) -o bin/ascii86_tests src/ascii86.o src/ascii86_tests.o

# make deflate86 (a test for ascii86.cpp and deflate.cpp)
bin/deflate86: src/deflate.o src/ascii86.o src/deflate86.o
	$(CC) $(LFLAGS) -o bin/deflate86 src/deflate.o src/ascii86.o src/deflate86.o

bin/inflate86: src/deflate.o src/ascii86.o src/inflate86.o
	$(CC) $(LFLAGS) -o bin/inflate86 src/deflate.o src/ascii86.o src/inflate86.o


# BINARIES
# compile the final product
src/mcrypt.o: src/mcrypt.cpp src/mcrypt.hpp
	$(CC) $(CFLAGS) src/mcrypt.cpp -o src/mcrypt.o

# make a binary for mcrypt
bin/mcrypt: all src/mcrypt.o
	$(CC) $(LFLAGS) src/cmatrix.o src/deflate.o src/util.o src/ascii86.o src/mops.o src/mcrypt.o -o bin/mcrypt


# ALIASES
test: bin/test
util_tests: bin/util_tests
mops_tests: bin/mops_tests
cmatrix_tests: bin/cmatrix_tests
deflate_tests: bin/deflate_tests
ascii86_tests: bin/ascii86_tests
deflate86: bin/deflate86
inflate86: bin/inflate86
flate86: inflate86 deflate86
mcrypt: bin/mcrypt


# MISC
# make wc is faster to type than make estimate, and more accurate too
wc:
	wc Makefile src/*.cpp src/*.hpp

# remove all .o files
clean:
	rm -f src/*.o

# clean the bin folder
cleanbin:
	rm -f bin/*

# shortcut for compiling all tests
tests: ascii86_tests deflate_tests cmatrix_tests util_tests

# shortcut to compile & run all tests
run_tests:
	ruby run_tests

# targets that don't really exist
.PHONY: wc clean tests all stuff tests test util_tests cmatrix_tests deflate_tests ascii86_tests mops_tests mcrypt deflate86 inflate86 flate86
