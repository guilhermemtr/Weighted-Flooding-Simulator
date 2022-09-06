#include <iostream>
#include <iomanip>
#include <math.h>
#include <bitset>
#include <map>
#include <utility>

#include "types.h"
#include "hybrid_protocol_simulator.h"
#include "receiver_protocol_simulator.h"
#include "send_protocol_simulator.h"
#include "k_out_protocol_simulator.h"
#include "hybrid_k_out_protocol_simulator.h"
#include "io_stake_distribution.h"
#include "generate_stake_distribution.h"


#include "yal_log.h"
#include "read_args.h"
#include "parallel_utils.h"
#include "random_utils.h"

#ifndef __MAIN_UTILS__
#define __MAIN_UTILS__

// Runs initialization procedures
void
init ();

// Generates a stake distribution.
// Flags should only have a single bit set to 1.
std::vector<stake_t>
generate_stake_distribution (party_t           n,
                             double            param1,
                             unsigned long int param2,
                             unsigned int      flags);

// Generates a corruption set.
std::vector<corruption_t>
get_corrupted_parties (std::vector<stake_t> stakes,
                       std::set<party_t>    honest,
                       corruption_t         corruption_strategy,
                       double               corruption_threshold);

// Shows the distribution info for a series of runs
void
show_distribution_info (unsigned int flags, unsigned int to_show);

// Shows the data from a run
void
show_run_data (double                    correct_runs,
               double                    runs,
               size_t                    factor,
               unsigned int              show_histogram,
               std::map<round_t, size_t> histogram);

// Runs tests for a fixed set of parameters.
template <typename T>
std::vector<T>
run_tests (party_t                                   n,
           unsigned int                              flags,
           size_t                                    runs,
           corruption_t                              corruption_strategy,
           double                                    corruption_threshold,
           size_t                                    factor,
           double                                    param1,
           long unsigned int                         param2,
           std::function<corruptions_stake_protocol_simulator *(
             std::vector<stake_t>, stake_t, size_t)> simulator_constructor,
           std::function<T (corruptions_stake_protocol_simulator *)> get_data);

// program essentially starts here; simulator_constructor just allows having
// multiple different main files for testing different protocols
/*void
run_command_parameters (
  int                                       argc,
  char                                    **argv,
  std::function<corruptions_stake_protocol_simulator *(
    std::vector<stake_t>, stake_t, size_t)> simulator_constructor);
*/

#endif    // __MAIN_UTILS__
