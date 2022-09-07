# Weighted-Flooding-Simulator
Simulator for message flooding protocols on graphs where nodes (i.e. parties) have weights. This simulator is intended to evaluate the performance of the `WFF` protocol given in [paper](https://eprint.iacr.org/2022/608). For more details on the scope of the simulator as well as on the `WFF` protocol and other considerations please refer to our [paper](https://eprint.iacr.org/2022/608).



## Documentation
The simulator was implemented in [C++](https://en.wikipedia.org/wiki/C%2B%2B), and makes use of [OpenMP](https://en.wikipedia.org/wiki/OpenMP).
The documentation of the code can be found inside the `./doc/html` folder.


## Building & Running The Simulator

### Building The Sources
We provide a `Makefile` for compiling the source code of the project.
The `Makefile` allows for compiling three distinct targets: 
- `make kout` compiles `main.cc` (and all other necessary files), generating executable `kout-bin`.
The purpose of this executable is running the `WFF` protocol given in the paper with more specific parameters.
- `make hybrid` compiles `hybrid_main.cc` (and all other necessary files), generating executable `hybrid-bin`.
The purpose of this executable is running a variant of the `WFF` protocol (more on this variant below) with specific parameters.
- `make bench` compiles `benchmark.cc` (and all other necessary files), generating executable `bench-bin`.
The purpose of this executable is benchmarking the performance of the kout protocol. The results of the benchmarks are written to [`.csv` files](https://en.wikipedia.org/wiki/Comma-separated_values),  (the benchmarks being the same as the ones plot in the paper[^1]).

To cleanup all compilation object files and executables, run `make clean`.





 from above where parties who didn't yet get the message look for it by requesting other parties.



For koutbin
`<#runs> <tests> <corrupt_parties> <corruption_threshold in [0,1]> <#min_parties> <#max_parties> <ratio_rich_poor> <#rich_parties> <to_show (a to show everything)> <#min_factor> <#max_factor>`



`000000001` - Deterministic Constant distribution
`000000010` - Deterministic Few-Fat distribution, thin sender
`000000100` - Deterministic Few-Fat distribution, fat sender
`000001000` - Deterministic Exponential distribution, thinnest sender
`000010000` - Deterministic Exponential distribution, median sender
`000100000` - Deterministic Exponential distribution, fattest sender
`001000000` - Random Exponential distribution
`010000000` - Random Uniform distribution
`100000000` - Random Geometric distribution


[^1]: [Paper](https://eprint.iacr.org/2022/608)
