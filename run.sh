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
cmake --install .
cd ../bin

# run
target=$1
shift
./$target $*

exit $?
