#include "main_utils.h"
#include "random_utils.h"

int
main (int argc, char **argv)
{
  init ();

  run_command_parameters (
    argc,
    argv,
    std::string ("hybrid"),
    [&] (std::vector<stake_t> stakes, stake_t total_stake, size_t factor) {
      return new hybrid_k_out_protocol_simulator (
        [&, stakes, total_stake, factor] (
          party_t i) {    // number parties to flood message to
          return (size_t) ((ceil ((((double) (stakes[i] * stakes.size ()))
                                   / ((double) total_stake))))
                           * (double) factor);
        },
        [&, stakes, total_stake, factor] (party_t i) {    // imitation function
          return (size_t) (ceil ((((double) (stakes[i] * stakes.size ()))
                                  / ((double) total_stake))));
        },
        [&, stakes, total_stake, factor] (
          party_t i) {    // number of requests to make
          return factor;
        },
        [&, stakes, total_stake, factor] (
          party_t i) {    // number of requests to handle
          return factor;
        },
        stakes);
    });
}
