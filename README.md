# Varcoin
Similar CryptoNote protocol implementation. The next generation of real
anonymity and application level encrypted currency. 

## How to Build
Dependencies: GCC 4.7.3 or later, CMake 2.8.6 or later, and Boost 1.55.</br>

Download to: </br></br>
http://gcc.gnu.org/</br>
http://www.cmake.org/</br>
http://www.boost.org/</br>

To build, change to a directory where this file is located, and run `make`. </br>
The resulting executables can be found in build/release/src.</br></br>

Advanced options:</br>
Parallel build: run `make -j<number of threads>` instead of `make`.</br>
Debug build: run `make build-debug`.</br>
Test suite: run `make test-release` to run tests in addition to building. </br>
Running `make test-debug` will do the same to the debug version.</br>
Building with Clang: it may be possible to use Clang instead of GCC, but this may not work everywhere. </br>
To build, run `export CC=clang CXX=clang++` before running `make`.</br></br>

On Windows:</br>
Dependencies: MSVC 2013 or later, CMake 2.8.6 or later, and Boost 1.55. </br>
You may download them from:</br>
http://www.microsoft.com/</br>
http://www.cmake.org/</br>
http://www.boost.org/</br>

To build, change to a directory where this file is located, and run this commands: </br>
mkdir build</br>
cd build</br>
cmake -G "Visual Studio 12 Win64" ..</br>

Thank you for your support.