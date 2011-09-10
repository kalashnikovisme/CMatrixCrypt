# VARIABLES
# compiler
CC=g++
# compile flags
CFLAGS=-c -O0 -g -Wall -Wno-sign-compare 
# link flags
LFLAGS=-lz -O0 -g -Wall -Wno-sign-compare
# names
LIBRARY_HEADERS=src/ascii86.hpp src/cmatrix.hpp src/deflate.hpp src/mops.hpp src/sha512.hpp src/util.hpp
LIBRARY_FILES=src/ascii86.cpp src/cmatrix.cpp src/deflate.cpp src/mops.cpp src/sha512.cpp src/util.cpp
LIBRARY_OBJS=obj/ascii86.o obj/cmatrix.o obj/deflate.o obj/mops.o obj/sha512.o obj/util.o


# LIBRARIES (anything that compiles to *.o)
# all the libraries used
all: cmatrixcrypt.a

# matrix class
obj/cmatrix.o: src/cmatrix.cpp src/cmatrix.hpp
	$(CC) $(CFLAGS) -o obj/cmatrix.o src/cmatrix.cpp

# deflate wrapper
obj/deflate.o: src/deflate.cpp src/deflate.hpp
	$(CC) $(CFLAGS) -o obj/deflate.o src/deflate.cpp

# various useful functions
obj/util.o: src/util.cpp src/util.hpp
	$(CC) $(CFLAGS) -o obj/util.o src/util.cpp

# ascii85 implementation
obj/ascii86.o: src/ascii86.cpp src/ascii86.hpp
	$(CC) $(CFLAGS) -o obj/ascii86.o src/ascii86.cpp

# matrix operations, has the matrix encryption functions
obj/mops.o: src/mops.cpp src/mops.hpp
	$(CC) $(CFLAGS) -o obj/mops.o src/mops.cpp

# some experimentations regarding password hashing
obj/phash.o: src/phash.cpp src/phash.hpp
	$(CC) $(CFLAGS) -o obj/phash.o src/phash.cpp

# PolarSSL sha4 implementation
obj/sha4.o: src/polarssl/config.h src/polarssl/sha4.h src/polarssl/sha4.cpp
	$(CC) $(CFLAGS) -o obj/sha4.o src/polarssl/sha4.cpp

obj/sha512.o: src/sha512.cpp src/sha512.hpp src/polarssl/sha4.h
	$(CC) $(CFLAGS) -o obj/sha512.o src/sha512.cpp


# TESTS (object files)
# compile tests
obj/test.o: src/test.cpp
	$(CC) $(CFLAGS) -o obj/test.o src/test.cpp

# ascii86 tests
obj/ascii86_tests.o: src/ascii86_tests.cpp
	$(CC) $(CFLAGS) -o obj/ascii86_tests.o src/ascii86_tests.cpp

# deflate tests
obj/deflate_tests.o: src/deflate_tests.cpp
	$(CC) $(CFLAGS) -o obj/deflate_tests.o src/deflate_tests.cpp

# cmatrix tests
obj/cmatrix_tests.o: src/cmatrix_tests.cpp
	$(CC) $(CFLAGS) -o obj/cmatrix_tests.o src/cmatrix_tests.cpp

# util tests
obj/util_tests.o: src/util_tests.cpp
	$(CC) $(CFLAGS) -o obj/util_tests.o src/util_tests.cpp

# matrix operations tests
obj/mops_tests.o: src/mops_tests.cpp src/mops.hpp src/cmatrix.hpp src/ascii86.hpp src/deflate.hpp src/util.hpp
	$(CC) $(CFLAGS) -o obj/mops_tests.o src/mops_tests.cpp

# make deflate86.o
obj/deflate86.o: src/deflate86.cpp
	$(CC) $(CFLAGS) -o obj/deflate86.o src/deflate86.cpp

# make inflate86.0
obj/inflate86.o: src/inflate86.cpp
	$(CC) $(CFLAGS) -o obj/inflate86.o src/inflate86.cpp

obj/sha4_tests.o: src/sha4_tests.cpp
	$(CC) $(CFLAGS) -o obj/sha4_tests.o src/sha4_tests.cpp

obj/sha512_tests.o: src/sha512_tests.cpp src/sha512.hpp
	$(CC) $(CFLAGS) -o obj/sha512_tests.o src/sha512_tests.cpp


# TESTS (binaries)
# make a binary for test
bin/test: all obj/test.o
	$(CC) $(LFLAGS) -o bin/test obj/cmatrix.o obj/deflate.o obj/util.o obj/ascii86.o obj/mops.o obj/test.o

# make util test binaries
bin/util_tests: obj/util.o obj/util_tests.o
	$(CC) $(LFLAGS) -o bin/util_tests obj/util.o obj/util_tests.o

# make tests for matrix operations
bin/mops_tests: obj/mops.o obj/cmatrix.o obj/deflate.o obj/ascii86.o obj/util.o obj/mops_tests.o
	$(CC) $(LFLAGS) -o bin/mops_tests obj/mops.o obj/cmatrix.o obj/deflate.o obj/ascii86.o obj/util.o obj/mops_tests.o

# make cmatrix test binaries
bin/cmatrix_tests: obj/cmatrix.o obj/cmatrix_tests.o
	$(CC) $(LFLAGS) -o bin/cmatrix_tests obj/cmatrix.o obj/cmatrix_tests.o

# make tests for deflate
bin/deflate_tests: obj/deflate.o obj/deflate_tests.o
	$(CC) $(LFLAGS) -o bin/deflate_tests obj/deflate.o obj/deflate_tests.o

# make tests for ascii86
bin/ascii86_tests: obj/ascii86.o obj/ascii86_tests.o
	$(CC) $(LFLAGS) -o bin/ascii86_tests obj/ascii86.o obj/ascii86_tests.o

# make deflate86 (a test for ascii86.cpp and deflate.cpp)
bin/deflate86: obj/deflate.o obj/ascii86.o obj/deflate86.o
	$(CC) $(LFLAGS) -o bin/deflate86 obj/deflate.o obj/ascii86.o obj/deflate86.o

bin/inflate86: obj/deflate.o obj/ascii86.o obj/inflate86.o
	$(CC) $(LFLAGS) -o bin/inflate86 obj/deflate.o obj/ascii86.o obj/inflate86.o

bin/sha4_tests: obj/sha4.o obj/sha4_tests.o
	$(CC) $(LFLAGS) -o bin/sha4_tests obj/sha4.o obj/sha4_tests.o

bin/sha512_tests: obj/sha512.o obj/sha4.o obj/sha512_tests.o
	$(CC) $(LFLAGS) -o bin/sha512_tests obj/sha512.o obj/sha4.o obj/sha512_tests.o


# BINARIES (end product)
# compile the final product
obj/mcrypt.o: $(LIBRARY_OBJS)
	$(CC) $(CFLAGS) -o obj/mcrypt.o src/mcrypt.cpp $(LIBRARY_OBJS)

obj/cmatrixcrypt.a: $(LIBRARY_OBJS)
	$(AR) -r obj/cmatrixcrypt.a $(LIBRARY_OBJS)

# make a binary for mcrypt
bin/mcrypt: all obj/mcrypt.o
	$(CC) $(LFLAGS) -o bin/mcrypt obj/cmatrix.o obj/deflate.o obj/util.o obj/ascii86.o obj/mops.o obj/mcrypt.o


# ALIASES
test: bin/test
util_tests: bin/util_tests
mops_tests: bin/mops_tests
sha4_tests: bin/sha4_tests
sha512_tests: bin/sha512_tests
cmatrix_tests: bin/cmatrix_tests
deflate_tests: bin/deflate_tests
ascii86_tests: bin/ascii86_tests
deflate86: bin/deflate86
inflate86: bin/inflate86
flate86: inflate86 deflate86
mcrypt: bin/mcrypt
cmatrixcrypt.a: obj/cmatrixcrypt.a

# all tests
tests: ascii86_tests deflate_tests cmatrix_tests util_tests mops_tests sha4_tests sha512_tests


# MISC
# make wc is faster to type than make estimate, and more accurate too
wc:
	find src -type f -exec wc {} +

# remove all .o files
cleanobj:
	rm -f obj/*.o
	rm -f obj/*.a

clean: cleanobj

# clean the bin folder
cleanbin:
	rm -f bin/*

# clean everything
cleanall: cleanbin cleanobj

# shortcut to compile & run all tests
run_tests: src/*.cpp src/*.hpp
	ruby run_tests | less

# targets that don't really exist
.PHONY: wc clean tests all stuff tests test util_tests cmatrix_tests deflate_tests ascii86_tests mops_tests mcrypt deflate86 inflate86 flate86
