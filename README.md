# Bitprim dojo <a target="_blank" href="https://bintray.com/hanchon/hanchon/bitprim-dojo%3Ahanchon">![Bintray][badge.Bintray]</a> <a target="_blank" href="https://travis-ci.org/hanchon/bitprim-dojo">![Travis status][badge.Travis]</a> <a target="_blank" href="https://ci.appveyor.com/projects/hanchon/bitprim-dojo">![Appveyor Status][badge.Appveyor] </a>

This is an example for educational purposes on how to use bitprim's code to solve the most common cryptocurrency use cases. It uses bitprim-core as a lib and also provides support for http requests in order to send RPC messages to a bitprim node.

This project can generate two binaries files: 
* bitprim-dojo is just an example on how to call the functions using cpp code.
* bitprim-cli (WIP) is an interface to call the functions directly from the console.

# Requirements:

* A C++11 compiler, currently minimum [GCC 4.8.0](https://gcc.gnu.org/projects/cxx0x.html) or Clang based on [LLVM 3.5](http://llvm.org/releases/3.5.0/docs/ReleaseNotes.html).
* [CMake](https://cmake.org/) minimum 3.7
* [Conan](https://conan.io/) minimum 1.4.0. [Conan installation guide](http://docs.conan.io/en/latest/installation.html) (Conan itself requires Python, Pip and CMake)
* Add the Bitprim's Conan Remote
 ```
conan remote add bitprim https://api.bintray.com/conan/bitprim/bitprim
```

# Download the binary files from bintray
* Currency can be set to:
 * BCH
 * BTC
 * LTC
```
conan install bitprim-dojo/0.1.1@hanchon/stable -o *:currency=BCH
```

# Build locally using conan create (Windows/Linux/OSX):
Build the binary files using conan create, this step will generate the binary files and save them to your local conan cache folder.
```
git clone https://github.com/hanchon/bitprim-dojo
cd bitprim-dojo
conan create . bitprim-dojo/0.1.1@hanchon/stable -o *:currency=BCH
```

# Run (Windows/Linux/OSX):
Install the binary files the previous step builds (it'll get the files from your local conan cache)
```
mkdir bin
cd bin
conan install bitprim-dojo/0.1.1@hanchon/stable -o *:currency=BCH
```
The biptirm-dojo and bitprim-cli files will be in the bin folder.

# Build and run (Debian):
```
git clone https://github.com/hanchon/bitprim-dojo
cd bitprim-dojo
mkdir build
cd build
conan install ..
cmake ..
```
Two binaries can be built:
* bitprim-dojo:
```
cmake --build ./ --target bitprim-dojo -- -j 2
./bin/bitprim-dojo
```
* bitprim-cli:
```
cmake --build ./ --target bitprim-cli -- -j 2
./bin/bitprim-cli
```

# Usage examples

## Create and sign a transaction (bitprim-cli)

* Create the wallet compressed for testnet
  * Parameters:
    * Seed (base 16 encoded and at least 32 char long)
    * Compressed (true/false)
    * Mainnet (true/false)
```
./bitprim-cli seed_to_wallet fffb587496cc54912bbcef874fa9a61a true false
```

```
mwx2YDHgpdfHUmCpFjEi9LarXf7EkQN6YG
```

* Create a txn using an utxo from that wallet
  * Parameters:
    * -i (inputs) = Transaction hash : index
    * -o (outputs) = wallet:amount
    * It supports comma-delimited parameters

```
./bitprim-cli create_txn -i "980de6ce12c29698d54323c6b0f358e1a9ae867598b840ee0094b9df22b07393:1" -o "mwx2YDHgpdfHUmCpFjEi9LarXf7EkQN6YG:199999000"
```

```
01000000019373b022dfb99400ee40b8987586aea9e158f3b0c62343d59896c212cee60d980100000000ffffffff0118beeb0b000000001976a914b43ff4532569a00bcab4ce60f87cdeebf985b69a88ac00000000
```

* Sign the transaction
  * Parameters:
    * Seed (base 16 encoded and at least 32 char long)
    * The script (output_script) can be found on blockdozer pressing the + icon https://tbch.blockdozer.com/tx/980de6ce12c29698d54323c6b0f358e1a9ae867598b840ee0094b9df22b07393
    * Amount is value of the previous output
    * Txn is the result of the last function

```
./bitprim-cli create_signature -seed "fffb587496cc54912bbcef874fa9a61a" -script "dup hash160 [b43ff4532569a00bcab4ce60f87cdeebf985b69a] equalverify checksig" -amount "200000000" -txn "01000000019373b022dfb99400ee40b8987586aea9e158f3b0c62343d59896c212cee60d980100000000ffffffff0118beeb0b000000001976a914b43ff4532569a00bcab4ce60f87cdeebf985b69a88ac00000000"
```

```
30440220433c405e4cb7698ad5f58e0ea162c3c3571d46d96ff1b3cb9232a06eba3b444d02204bc5f48647c0f052ade7cf85eac3911f7afbfa69fa5ebd92084191a5da33f88d41
```

* Assign the script to the transaction
  * Parameters:
    * Seed (base 16 encoded and at least 32 char long)
    * Signature is the result of the last function
    * Txn is the transaction created in the second step
```
./bitprim-cli set_input_script -seed "fffb587496cc54912bbcef874fa9a61a" -signature "30440220433c405e4cb7698ad5f58e0ea162c3c3571d46d96ff1b3cb9232a06eba3b444d02204bc5f48647c0f052ade7cf85eac3911f7afbfa69fa5ebd92084191a5da33f88d41" -txn "01000000019373b022dfb99400ee40b8987586aea9e158f3b0c62343d59896c212cee60d980100000000ffffffff0118beeb0b000000001976a914b43ff4532569a00bcab4ce60f87cdeebf985b69a88ac00000000"
```

```
01000000019373b022dfb99400ee40b8987586aea9e158f3b0c62343d59896c212cee60d98010000006a4730440220433c405e4cb7698ad5f58e0ea162c3c3571d46d96ff1b3cb9232a06eba3b444d02204bc5f48647c0f052ade7cf85eac3911f7afbfa69fa5ebd92084191a5da33f88d4121027a45d4abb6ebb00214796e2c7cf61d18c9185ba771fe9ed75b303eb7a8e9028bffffffff0118beeb0b000000001976a914b43ff4532569a00bcab4ce60f87cdeebf985b69a88ac00000000
```

* Notes:
  * The current implementation only support one input and can sign using compressed format wallets only.
  * The functionality can be extended adding the respective parameters to the functions on the process_requests.hpp file.


# Libs used:
* [Bitprim-core](https://github.com/bitprim/bitprim-core) for the crypto-currency functions.
* [Simple-web-server](https://github.com/eidheim/Simple-Web-Server) for the http requests.
* Some clases imported in the utils files from [Libbitcoin-Explorer](https://github.com/libbitcoin/libbitcoin-explorer).


<!-- Links -->
[badge.Travis]: https://api.travis-ci.org/hanchon/bitprim-dojo.svg?branch=master
[badge.Bintray]: https://api.bintray.com/packages/hanchon/hanchon/bitprim-dojo%3Ahanchon/images/download.svg?version=0.1.1%3Astable
[badge.Appveyor]: https://ci.appveyor.com/api/projects/status/github/hanchon/bitprim-dojo?svg=true&branch=master
