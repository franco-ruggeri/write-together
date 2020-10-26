#!/bin/bash

if [ $# -lt 1 ]; then
	echo "usage: $0 target arguments"
	exit 1
fi

# compile and test
cd ..
mkdir -p build
cd build
cmake ..
make
ctest .

# install and run
make install
cd ../bin
target=$1
shift
./$target $*

exit $?
