#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <bitset>

#include "types.h"
#include "yal_log.h"

#ifndef __SIM_READ_ARGS__
#define __SIM_READ_ARGS__


#define RAND_TEST_TYPE_OFFSET 7
#define TEST_TYPES (RAND_TEST_TYPE_OFFSET + 3)

#define NR_ARGS 12

#define NR_OUTPUTS_SHOW 6
#define SHOW_DISTRIBUTION (1 << 0)
#define SHOW_FACTOR (1 << 1)
#define SHOW_SUCCESS (1 << 2)
#define SHOW_NR_PARTIES (1 << 3)
#define SHOW_TEST (1 << 4)
#define SHOW_HISTOGRAM (1 << 5)


typedef struct
{
  counter_t runs;

  unsigned int tests;

  corruption_t corruption_strategy;
  double       corruption_threshold;

  party_t min_nr_parties;
  party_t max_nr_parties;

  double  ratio_rich_poor;
  party_t nr_rich_parties;

  unsigned int to_show;

  size_t min_factor;
  size_t max_factor;

} simulation_parameters_t;

// prints the arguments expected as parameters
void
print_arguments ();

// prints the generated parameters for the run.
void
print_parameters (simulation_parameters_t params);

// reads the input parameters
simulation_parameters_t
read_parameters (int argc, char **argv);

#endif    // __SIM_READ_ARGS__
