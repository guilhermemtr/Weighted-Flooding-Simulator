#include "main_utils.h"
#include "random_utils.h"

int
main (int argc, char **argv)
{
  init ();

  std::string prot_name("k-out protocol");

  
  run_command_parameters (
    argc,
    argv,
    prot_name,
    [&] (std::vector<stake_t> stakes, stake_t total_stake, size_t factor) {
      return new k_out_protocol_simulator (
        [&, stakes, total_stake, factor] (party_t i) {    // imitation function
          return (size_t) ((ceil ((((double) (stakes[i] * stakes.size ()))
                                   / ((double) total_stake))))
                           * (double) factor);
        },
        [&, stakes, total_stake, factor] (party_t i) {    // nr flooding targets
          return (size_t) (ceil ((((double) (stakes[i] * stakes.size ()))
                                  / ((double) total_stake))));
        },
        stakes);
    });

  return 0;
}
