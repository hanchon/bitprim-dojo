# Bitprim dojo 
This is an example for educational purposes on how to use bitprim's code to solve the most common cryptocurrency use cases. It uses bitprim-core as a lib and also provides support for http requests in order to send RPC messages to a bitprim node.

This project can generate two binaries files: 
* bitprim-dojo is just an example on how to call the functions using cpp code.
* bitprim-cli (WIP) is an interface to call the functions directly from the console.

# Requirements:

* A C++11 compiler, currently minimum [GCC 4.8.0](https://gcc.gnu.org/projects/cxx0x.html) or Clang based on [LLVM 3.5](http://llvm.org/releases/3.5.0/docs/ReleaseNotes.html).
* [CMake](https://cmake.org/) minimum 3.7
* [Conan](https://conan.io/) minimum 1.2.3. [Conan installation guide](http://docs.conan.io/en/latest/installation.html) (Conan itself requires Python, Pip and CMake)
* Add the Bitprim's Conan Remote
 ```
conan remote add bitprim https://api.bintray.com/conan/bitprim/bitprim
```

# Build and run:
```
git clone https://github.com/hanchon/bitprim-dojo
cd bitprim-dojo
mkdir build
cd build
conan install ..
```
Two binaries can be built:
* bitprim-dojo:
```
cmake --build ./ --target bitprim-cli -- -j 2
./bitprim-dojo
```
* bitprim-cli:
```
cmake --build ./ --target bitprim-cli -- -j 2
./bitprim-cli
```

# Libs used:
* [Bitprim-core](https://github.com/bitprim/bitprim-core) for the crypto-currency functions.
* [Simple-web-server](https://github.com/eidheim/Simple-Web-Server) for the http requests.
* Some clases imported in the utils files from [Libbitcoin-Explorer](https://github.com/libbitcoin/libbitcoin-explorer).