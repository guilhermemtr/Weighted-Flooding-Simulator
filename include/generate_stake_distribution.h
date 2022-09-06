#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <cmath>

#include <random>
#include <algorithm>

#include "types.h"

#ifndef __GENERATE_STAKE_DISTRIBUTION__
#define __GENERATE_STAKE_DISTRIBUTION__

class stake_distribution_generator
{
    protected:
  // variables used to get randomness
  std::random_device         rd;
  std::default_random_engine re;

  std::vector<stake_t>
  few_fat_stake (party_t nr_parties, double rich_poor_ratio, size_t nr_rich);

  std::vector<stake_t>
  exponential_stake (party_t nr_parties, double richest_poorest_ratio);

    public:
  stake_distribution_generator ();


  std::vector<stake_t>
  constant_stake (party_t nr_parties, stake_t s = 100);

  std::vector<stake_t>
  few_fat_thin_stake (party_t nr_parties,
                      double  rich_poor_ratio,
                      size_t  nr_rich);

  std::vector<stake_t>
  few_fat_fat_stake (party_t nr_parties,
                     double  rich_poor_ratio,
                     size_t  nr_rich);

  std::vector<stake_t>
  exponential_thinnest_stake (party_t nr_parties, double richest_poorest_ratio);

  std::vector<stake_t>
  exponential_median_stake (party_t nr_parties, double richest_poorest_ratio);

  std::vector<stake_t>
  exponential_fattest_stake (party_t nr_parties, double richest_poorest_ratio);




  std::vector<stake_t>
  sample_uniform_stake (party_t nr_parties,
                        stake_t lower_bound = 0,
                        stake_t upper_bound = 1000000);

  std::vector<stake_t>
  sample_geometric_stake (party_t nr_parties, double p = 0.5);

  std::vector<stake_t>
  sample_exponential_stake (party_t nr_parties, double lambda = 0.001);

  void
  add_to_stake_distribution (std::vector<stake_t> &stakes, stake_t v);

  void
  mult_stake_distribution (std::vector<stake_t> &stakes, double factor);
};

#endif    // __GENERATE_STAKE_DISTRIBUTION__
