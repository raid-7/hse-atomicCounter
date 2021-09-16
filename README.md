Contention-aware counter
========================

This is a test task for a research project in HSE.

# Task 1

* C implementation: counter.hpp
* C++ implementation: cpp_impl/counter.cpp

C++ version is fancier but incompatible with GenMC.

* C version tests: test.cpp
* C++ version tesets: cpp_impl/test.cpp

C++ tests contain concurrent stress tests. C tests are adopted to model checking.

## Build and run

Assuming `make` and `ctest` backends:

```sh
# build C tests
mkdir build
cd build
cmake ..
make

# build C++ tests
mkdir build
cd build
cmake -DCPP=ON ..
make

# run tests
ctest --verbose
```

# Task 2

```sh
GMC=/path/to/genmc ./genmc
```

Model checking takes some time.

# Task 3

See [PROOF.pdf](https://github.com/raid-7/hse-atomicCounter/blob/master/PROOF.pdf).

# Task 4

See [DISPROOF.pdf](https://github.com/raid-7/hse-atomicCounter/blob/master/DISPROOF.pdf).
