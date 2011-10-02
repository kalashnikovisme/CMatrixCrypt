#!/bin/bash
# check if cryptest exists (since this depends on it)
if [ ! -f ./cryptest ]; then
	echo "cryptest is missing, compiling now..."
	cd ..
	make cryptest
	cd bin
fi

echo -n $2 | ./cryptest $1 | inflate86 | hexdump
