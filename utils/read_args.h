#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <bitset>

#include "types.h"
#include "yal_log.h"

#ifndef __SIM_READ_ARGS__
#define __SIM_READ_ARGS__


// definitions related with stake distribution flags
#define DET_CONST_DIST          (1 << 0) // Deterministic Constant Distribution
#define DET_FF_THIN_DIST        (1 << 1) // Deterministic Few-Fat     Thin   Sender Distribution
#define DET_FF_FAT_DIST         (1 << 2) // Deterministic Few-Fat     Fat    Sender Distribution
#define DET_EXP_THIN_DIST       (1 << 3) // Deterministic Exponential Thin   Sender Distribution
#define DET_EXP_MEDN_DIST       (1 << 4) // Deterministic Exponential Median Sender Distribution
#define DET_EXP_FAT_DIST        (1 << 5) // Deterministic Exponential Fat    Sender Distribution
#define RAND_EXP_RAND_SND_DIST  (1 << 6) // Random Exponential        Random Sender Distribution
#define RAND_UNIF_RAND_SND_DIST (1 << 7) // Random Uniform            Random Sender Distribution
#define RAND_GEOM_RAND_SND_DIST (1 << 8) // Random Geometric          Random Sender Distribution

#define TEST_TYPES 9 // Number of different types of distribution


// definitions related with type of corruption
#define NO_CORR         0 // No corruption
#define CORR_RAND       1 // Corrupt parties at random
#define CORR_RICH_FIRST 2 // Corrupt rich parties first
#define CORR_POOR_FIRST 3 // Corrupt poor parties first

#define CORR_TYPES 4 // number of types of corruption




// definitions related with output flags (each value below specifies a flag)
#define SHOW_DISTRIBUTION (1 << 0) // Show distribution
#define SHOW_FACTOR       (1 << 1) // Show nr targets multiplicative factor
#define SHOW_SUCCESS      (1 << 2) // Show success rate of tests
#define SHOW_NR_PARTIES   (1 << 3) // Show number of parties
#define SHOW_TEST         (1 << 4) // Show test being run
#define SHOW_HISTOGRAM    (1 << 5) // Show histogram of how many parties got the message per round

// number of different flags
#define NR_OUTPUTS_SHOW 6



// total number of arguments (not counting with the executable file name)
#define NR_ARGS 11


/**
 * @struct simulation_parameters_t
 * @brief Structure that contains all the relevant information for running the
 * simulator for a fixed protocol.
 *
 * Structure that holds all relevant information for running the simulator for a
 * fixed protocol.
 */
typedef struct
{
  counter_t runs; // number of runs to make

  unsigned int tests; // which tests to run

  corruption_t corruption_strategy; // how parties should be corrupted
  double       corruption_threshold; // upper bound on corruption fraction

  /**
   * The parameters below are only relevant for the generation of the stake distributions.
   * The simulator is run with (2^i) * min_nr_parties, for i from 0 up to the
   * largest value for which (2^i) * min_nr_parties <= max_nr_parties
   */
  party_t min_nr_parties; // minimum number of parties
  party_t max_nr_parties; // maximum number of parties

  double  ratio_rich_poor; // ratio between richest party(ies) and poorest party(ies).
  party_t nr_rich_parties; // number of rich parties (only relevant for the few
                           // fat stake distribution

  unsigned int to_show; // the flags that indicate what information to display
                        // when running the simulator

  /**
   * The minimum and maximum factors to test:
   * The simulator is run for a factor starting at factor = min_factor, up to
   * factor <= max_factor, with increments of 1.
   */
  size_t min_factor; // the minimum factor to test
  size_t max_factor; // the maximum factor to test

} simulation_parameters_t;

/**
 * Prints the arguments expected as parameters.
 */
void
print_arguments ();

/**
 * Prints the parameters for the run.
 * @param the parameters.
 */
void
print_parameters (simulation_parameters_t params);

/**
 * Reads the input parameters.
 * @param the number of input arguments + 1.
 * @param the input arguments (the first argument is the name of the executable).
 * @return the parameters to be used for the simulation.
 */
simulation_parameters_t
read_parameters (int argc, char **argv);

#endif    // __SIM_READ_ARGS__
