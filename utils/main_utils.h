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

/**
 * Runs initialization procedures.
 */
void
init ();

/**
 * Generates a stake distribution.
 * Flags should only have a single bit set to 1.
 * @param the number of parties.
 * @param the stake ratio between the poorest party(ies) and the richest
 * party(ies).
 * @param the number of rich parties.
 * @param the flags for generating the stake distribution.
 * @return the stake distribution.
 */
std::vector<stake_t>
generate_stake_distribution (party_t           n,
                             double            param1,
                             unsigned long int param2,
                             unsigned int      flags);

/**
 * Generates a corruption set.
 * @param the stake distribution.
 * @param a set of parties that cannot be corrupted.
 * @param the corruption strategy.
 * @param fraction of stake that can be corrupted.
 */
std::vector<corruption_t>
get_corrupted_parties (std::vector<stake_t> stakes,
                       std::set<party_t>    honest,
                       corruption_t         corruption_strategy,
                       double               corruption_threshold);

/**
 * Prints the distribution name for a given set of flags (which represent a
 * distribution).
 * @param flags: the type of stake distribution.
 * @param to_show: flags indicating which information to show.
 */
void
show_distribution_info (unsigned int flags, unsigned int to_show);

/**
 * Shows the data from a run.
 * @param the number of correct runs.
 * @param the number of runs.
 * @param the factor between the poorest and richest party(ies).
 * @param flag indicating whether to show a histogram of how many parties got
 * the message per round.
 * @param the histogram of which parties got the message per round.
 */
void
show_run_data (double                    correct_runs,
               double                    runs,
               size_t                    factor,
               unsigned int              show_histogram,
               std::map<round_t, size_t> histogram);

/**
 * Runs tests for a fixed set of parameters.
 * @param number of parties.
 * @param flags for which test to be run.
 * @param number of runs to make.
 * @param corruption strategy.
 * @param corruption threshold.
 * @param multiplicative factor for how many parties to target.
 * @param ratio between stake of richest and poorest party(ies).
 * @param number of richest parties.
 * @param constructor for the protocol simulator to be used.
 * @param function that extracts data from a set of runs (with the same
 * parameters).
 * @return a vector with the data from the tests run.
 */
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
void
run_command_parameters (
  int                                       argc,
  char                                    **argv,
  std::string                               prot_name,
  std::function<corruptions_stake_protocol_simulator *(
    std::vector<stake_t>, stake_t, size_t)> simulator_constructor);


#endif    // __MAIN_UTILS__
