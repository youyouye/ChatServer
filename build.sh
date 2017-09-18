mkdir build
rm -fr release
mkdir release
cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=../release ..
make -j
make > a.txt 2>&1
make install
