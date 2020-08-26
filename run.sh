if [ $# -lt 1 ]; then
	echo "usage: $0 target arguments"
	exit 1
fi

# compile and test
mkdir -p build
cd build
cmake ..
cmake --build .
ctest .

# install and run
cmake --install .
cd ../bin
target=$1
shift
./$target $*

exit $?
