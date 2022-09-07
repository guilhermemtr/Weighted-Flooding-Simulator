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
To execute either `kout-bin` or `hybrid-bin`, run `./[kout-bin|hybrid-bin] <#runs> <tests (9 bits)> <corrupt_parties> <corruption_threshold in [0,1]> <#min_parties> <#max_parties> <ratio_rich_poor> <#rich_parties> <to_show (5 bits)> <#min_factor> <#max_factor>`.
This will run the simulator on each possible configuration for `<#runs>` times, each configuration being a different combination of the input parameters.
For instance, for `./kout-bin 10 000000001 1 0.5 512 512 1000000 20 a 20 30` there are only 11 possible configurations, one for the multiplicative factor set to `20`, another for the multiplicative factor set to `21`, all the way up to `30`.
In all configurations, the stake distribution is always the constant distribution (meaning all parties have exactly the same stake), parties are corrupted uniformly at random, the corruption threshold is `0.5` the total stake, there are `512` parties (both the minimum number of parties and the maximum number of parties is `512`), and the multiplicative factors vary from 20 (inclusive) to 30 (inclusive).
Since for all the configurations the stake distribution is always the constant stake distribution, the parameters `1000000 20` are ignored, as they represent the ratio between the stake of the richest party(ies) and the poorest party(ies) (for more information on the stake distributions, refer to our [paper](https://eprint.iacr.org/2022/608)).

**Parameter Description**
- `[kout-bin|hybrid-bin]`: Indicates which protocol to simulate; `kout-bin` simulates the `WFF` protocol presented in the [paper](https://eprint.iacr.org/2022/608), whereas `hybrid-bin` simulates a variant of `WFF` where parties who did not (yet) get the message query other parties for incoming messages.
- `<#runs>`: The number of runs to make for each possible configuration.
- `<tests (9 bits)>`: Each bit indicates whether to benchmark the protocol against a certain stake distribution (see below for a list of what each bit means).
- `<corrupt_parties>`: If set to `0` parties are not corrupted; if set to `1` parties are corrupted in a random fashion; if set to `2` rich parties are corrupted first; if set to `3` poor parties are corrupted first.
- `<corruption_threshold in [0,1]>`: Indicates the fraction of stake that can be corrupted.
- `<#min_parties>` and `<#max_parties>`: Indicate the minimum and maximum number of parties; both values should be a power of `2`; the possible numbers of parties are each power of `2` that is greater or equal to `<#min_parties>` and smaller or equal to `<#max_parties>`.
- `<ratio_rich_poor>` and `<#rich_parties>`: Indicate, respectively, the ratio of stake between the richest party(ies) and the poorest party(ies), and the number of rich parties. Both parameters only apply to Few-Fat and Exponential stake distributions. The second parameter only applies to Few-Fat stake distributions.
- `<to_show (5 bits)>`: Each bit indicates whether to display certain information when running the protocol on each configuration (see below for a list of possible values).  Value `a` will set all bits to `1` and thus will make all possible information be output.
- `<#min_factor>` and `<#max_factor>`: Indicates the minimum and maximum multiplicative factors; the possible values for the factor are all integers that are greater or equal to `<#min_factor>` and smaller or equal to `<#max_factor>`.


*`<tests (9 bits)>` Parameter Description*
`000000001` - Deterministic Constant distribution; all parties have stake `1`.
`000000010` - Deterministic Few-Fat distribution, thin sender; all thin parties have stake `1`, whereas all fat parties have stake `<ratio_rich_poor>`; the sender has stake `1`; there are `<#rich_parties>` parties with stake `<ratio_rich_poor>`.
`000000100` - Deterministic Few-Fat distribution, fat sender; all thin parties have stake `1`, whereas all fat parties have stake `<ratio_rich_poor>`; the sender has stake `<ratio_rich_poor>`; there are `<#rich_parties>` parties with stake `<ratio_rich_poor>`.
`000001000` - Deterministic Exponential distribution, thinnest sender; thinnest party has stake `1000000`, second thinnest party has stake `f * 1000000`, third thinnest party has stake `f * (f * 1000000)`, ..., fattest party has stake `<ratio_rich_poor> * 1000000`; the sender has stake `1000000`.
`000010000` - Deterministic Exponential distribution, median sender; thinnest party has stake `1000000`, second thinnest party has stake `f * 1000000`, third thinnest party has stake `f * (f * 1000000)`, ..., fattest party has stake `<ratio_rich_poor> * 1000000`; the sender has stake `f^(<nr_parties/2>) * 1000000`.
`000100000` - Deterministic Exponential distribution, fattest sender; thinnest party has stake `1000000`, second thinnest party has stake `f * 1000000`, third thinnest party has stake `f * (f * 1000000)`, ..., fattest party has stake `<ratio_rich_poor> * 1000000`; the sender has stake `<ratio_rich_poor> * 1000000`.
`001000000` - Random Exponential distribution; stakes of each party are picked according to a (random) exponential distribution with parameter `<ratio_rich_poor>`; the sender is picked uniformly at random.
`010000000` - Random Uniform distribution; stakes of each party are picked according to a (random)
uniform distribution with parameters `(1,<ratio_rich_poor>+1)`; the sender is picked uniformly at random.
`100000000` - Random Geometric distribution; stakes of each party are picked according to a random geometric distribution with parameter `<ratio_rich_poor>`; the sender is picked uniformly at random.


*`<to_show (5 bits)>` Parameter Description*
`00001` - Show Distribution: shows which (type of) stake distribution is being used for the test being executed.
`00010` - Show Factor: shows which multiplicative factor is being used for the test being executed.
`00100` - Show Success: shows the success rate of the test (i.e. how often all parties received the message).
`01000` - Show Number Of Parties: shows the number of parties for the current test being executed.
`10000` - Show Test: shows the bitstring identifying which (type of) stake distribution is being used for the test being executed.


### Running The Benchmarks
An alternative way for running the simulator is by simply running the benchmarks:
1. Run `./setup-bench clean-all` to erase folder `./results` and all its subfolders.
2. Run `./setup-bench setup` to create all necessary directories.
3. Run `./bench-bin` to run the benchmarks (beware, depending on the parameters defined in `benchmark.cc`, running the benchmarks may take a ==very long time==).
4. The benchmark results will be stored inside the `results` folder.

[^1]: One can actually replicate the results given in our [paper](https://eprint.iacr.org/2022/608) by running `bench-bin`. However, beware replicating our results takes a significant amount of computing time.
