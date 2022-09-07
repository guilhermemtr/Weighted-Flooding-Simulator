# Weighted-Flooding-Simulator
Simulator for message flooding protocols on graphs where nodes (i.e. parties) have weights.


The `Makefile` allows for compiling three distinct targets: 
- `make kout` compiles `main.cc` (and all other necessary files), generating executable `koutbin`.
The purpose of this executable is running the kout protocol from the paper[^1] with more specific parameters.
- `make hybrid` compiles `hybrid_main.cc` (and all other necessary files), generating executable `hybin`.
The purpose of this executable is running a variant of the kout protocol from above where parties who didn't yet get the message look for it by requesting other parties.
- `make kout-bench` compiles `benchmark.cc` (and all other necessary files), generating executable `koutbenchbin`.
The purpose of this executable is benchmarking the performance of the kout protocol (the benchmarks being the same as the ones plot in the paper[^1]).

To cleanup all compilation object files and executables, run `make clean`.









[^1]: [Paper](https://eprint.iacr.org/2022/608)
