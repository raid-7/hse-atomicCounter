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

# Task 2

```sh
GMC=/path/to/genmc ./genmc
```

Model checking takes some time.

# Task 3

See PROOF.md.

#Task 4

See DISPROOF.md.