# Weighted-Flooding-Simulator
Simulator for message flooding protocols on graphs where nodes (i.e. parties) have weights. This simulator is intended to evaluate the performance of the `WFF` protocol given in [paper](https://eprint.iacr.org/2022/608). For more details on the scope of the simulator as well as on the `WFF` protocol please refer to our [paper](https://eprint.iacr.org/2022/608).



## Documentation
The simulator was implemented in [C++](https://en.wikipedia.org/wiki/C%2B%2B), and makes use of [OpenMP](https://en.wikipedia.org/wiki/OpenMP).
The documentation of the code can be found inside the `./doc/html` folder.


## Building & Running The Simulator

### Building The Sources
We provide a `Makefile` for compiling the source code of the project.
The `Makefile` allows for compiling three distinct targets: 
- `make bench` compiles `benchmark.cc` (and all other necessary files), generating executable `bench-bin`.
The purpose of this executable is benchmarking the performance of the `WFF` protocol via simulations (the results of the benchmarks are written to [`.csv`](https://en.wikipedia.org/wiki/Comma-separated_values) files).
These are the same benchmarks considered in our [paper](https://eprint.iacr.org/2022/608)[^1].
- `make kout` compiles `main.cc` (and all other necessary files), generating executable `kout-bin`.
This executable allows running the simulator of the `WFF` protocol where the execution parameters are passed as command-line arguments.
- `make hybrid` compiles `hybrid_main.cc` (and all other necessary files), generating executable `hybrid-bin`.
This executable allows running the simulator of a variant of the `WFF` protocol (more on this variant below) where the execution parameters are passed as command-line arguments.

To cleanup all compilation object files and executables, run `make clean`.


### Running The Simulator
To execute either `kout-bin` or `hybrid-bin`, run `./[kout-bin|hybrid-bin] <#runs> <tests (8 bits)> <corrupt_parties> <corruption_threshold in [0,1]> <#min_parties> <#max_parties> <ratio_rich_poor> <#rich_parties> <to_show (a to show everything)> <#min_factor> <#max_factor>`.
This will run the simulator on each possible configuration for `<#runs>` times, each configuration being a different combination of the input parameters.
For instance, for `./kout-bin 10 000000001 1 0.5 512 512 1000000 20 a 20 30` there are only 11 possible configurations, one for the multiplicative factor set to `20`, another for the multiplicative factor set to `21`, all the way up to `30`.
In all configurations, the stake distribution is always the constant distribution (meaning all parties have exactly the same stake), parties are corrupted uniformly at random, the corruption threshold is `0.5` the total stake, there are `512` parties (both the minimum number of parties and the maximum number of parties is `512`), and the multiplicative factors vary from 20 (inclusive) to 30 (inclusive).
Since for all the configurations the stake distribution is always the constant stake distribution, the parameter `1000000` and the following parameter `20` are ignored (for more information on the stake distributions, refer to our [paper](https://eprint.iacr.org/2022/608)).











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



An alternative way for running the simulator is by simply running the benchmarks:
1. Run `./setup-bench clean-all` to erase folder `./results` and all its subfolders.
2. Run `./setup-bench setup` to create all necessary directories.
3. Run `./bench-bin` to run the benchmarks (beware, depending on the parameters defined in `benchmark.cc`, running the benchmarks may take a ==very long time==.


[^1]: One can actually replicate the results given in our [paper](https://eprint.iacr.org/2022/608) by running `bench-bin`. However, beware replicating our results takes a significant amount of computing time.
