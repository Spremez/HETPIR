# HETPIR-: Practical Keyword PIR via a Novel Homomorphic Equality Test Algorithm
This is just an experimental example to show that the article works.
## Requirements
The runnig of this code requires a basic c++ toolchain, including
g++ compiler (clang++12 or newer is optimal)
make
cmake
## Build & Run
We adapt Intel HEXL library to implement the NTTs. Therefore, please install it firstly. We recommend that install it to your default user's program path /usr/local, and the suggeted version of them are showed in following table. What's more, using clang++12 or newer compiler can achieve better performance.

```bash
# When you have installed hexl, you can run in the current directory
mkdir build
cd build
cmake ..
make
