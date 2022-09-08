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

/**
 * @class stake_distribution_generator
 * @brief Class for generating stake distributions.
 *
 * Class responsible for generating stake distributions.
 */
class stake_distribution_generator
{
    protected:
  //!< variables used to get randomness
  std::random_device         rd;    //!< random device
  std::default_random_engine re;    //!< default random engine

  /**
   * Generates a few heavy stake distribution with the given parameters.
   * @param the number of parties.
   * @param the ratio between the stake of the heaviest and lightest party(ies).
   * @param the number of heavy parties.
   * @return the corresponding stake distribution.
   */
  std::vector<stake_t>
  few_heavy_stake (party_t nr_parties,
                   double  heavy_light_ratio,
                   size_t  nr_heavy);

  /**
   * Generates an exponential stake distribution with the given parameters.
   * @param the number of parties.
   * @param the ratio between the stake of the heaviest and lightest party(ies).
   * @return the corresponding stake distribution.
   */
  std::vector<stake_t>
  exponential_stake (party_t nr_parties, double heaviest_lightest_ratio);

    public:
  stake_distribution_generator ();


  /**
   * Generates a constant stake distribution with the given parameters.
   * @param the number of parties.
   * @param the stake of each party.
   * @return the stake distribution.
   */
  std::vector<stake_t>
  constant_stake (party_t nr_parties, stake_t s = 100);

  /**
   * Generates a few heavy stake distribution with the given parameters, and
   * where the sender is the lightest party. By convention, the sender is the
   * first party in the vector output by this method.
   * @param the number of parties.
   * @param the ratio between the stake of the heaviest and lightest party(ies).
   * @param the number of heavy parties.
   * @return the corresponding stake distribution.
   */
  std::vector<stake_t>
  few_heavy_light_stake (party_t nr_parties,
                         double  heavy_light_ratio,
                         size_t  nr_heavy);

  /**
   * Generates a few heavy stake distribution with the given parameters, and
   * where the sender is the heaviest party. By convention, the sender is the
   * first party in the vector output by this method.
   * @param the number of parties.
   * @param the ratio between the stake of the heaviest and lightest party(ies).
   * @param the number of heavy parties.
   * @return the corresponding stake distribution.
   */
  std::vector<stake_t>
  few_heavy_heavy_stake (party_t nr_parties,
                         double  heavy_light_ratio,
                         size_t  nr_heavy);

  /**
   * Generates an exponential stake distribution with the given parameters, and
   * where the sender is the lightest party. By convention, the sender is the
   * first party in the vector output by this method.
   * @param the number of parties.
   * @param the ratio between the stake of the heaviest and lightest party(ies).
   * @return the corresponding stake distribution.
   */
  std::vector<stake_t>
  exponential_lightest_stake (party_t nr_parties,
                              double  heaviest_lightest_ratio);

  /**
   * Generates an exponential stake distribution with the given parameters, and
   * where the sender is the party with the median wealth/weight. By convention,
   * the sender is the first party in the vector output by this method.
   * @param the number of parties.
   * @param the ratio between the stake of the heaviest and lightest party(ies).
   * @return the corresponding stake distribution.
   */
  std::vector<stake_t>
  exponential_median_stake (party_t nr_parties, double heaviest_lightest_ratio);

  /**
   * Generates an exponential stake distribution with the given parameters, and
   * where the sender is the heaviest party. By convention, the sender is the
   * first party in the vector output by this method.
   * @param the number of parties.
   * @param the ratio between the stake of the heaviest and lightest party(ies).
   * @return the corresponding stake distribution.
   */
  std::vector<stake_t>
  exponential_heaviest_stake (party_t nr_parties,
                              double  heaviest_lightest_ratio);



  /**
   * Generates a stake distribution with the given number of parties, where the
   * stake of each party is picked according to a uniform distribution with
   * parameters <parameter lower_bound> and <parameter upper_bound>, and where
   * the sender is a random party. By convention, the sender is the first party
   * in the vector output by this method.
   * @param the number of parties.
   * @param the first parameter for the uniform distribution.
   * @param the second parameter for the uniform distribution.
   * @return the corresponding stake distribution.
   */
  std::vector<stake_t>
  sample_uniform_stake (party_t nr_parties,
                        stake_t lower_bound = 0,
                        stake_t upper_bound = 1000000);

  /**
   * Generates a stake distribution with the given number of parties, where the
   * stake of each party is picked according to a geometric distribution with
   * parameter <parameter p>, and where the sender is a random party. By
   * convention, the sender is the first party in the vector output by this
   * method.
   * @param the number of parties.
   * @param the parameter for the geometric distribution.
   * @return the corresponding stake distribution.
   */
  std::vector<stake_t>
  sample_geometric_stake (party_t nr_parties, double p = 0.5);

  /**
   * Generates a stake distribution with the given number of parties, where the
   * stake of each party is picked according to an exponential distribution with
   * parameter <parameter p>, and where the sender is a random party. By
   * convention, the sender is the first party in the vector output by this
   * method.
   * @param the number of parties.
   * @param the parameter for the exponential distribution.
   * @return the corresponding stake distribution.
   */
  std::vector<stake_t>
  sample_exponential_stake (party_t nr_parties, double lambda = 0.001);

  /**
   * Adds, for each party in the stake distribution, <parameter v> additional
   * stake.
   * @param the stake distribution.
   * @param the additional stake to be added to each party.
   */
  void
  add_to_stake_distribution (std::vector<stake_t> &stakes, stake_t v);

  /**
   * Multiplies the stake of each party in the stake distribution by a factor
   * <parameter factor>.
   * @param the stake distribution.
   * @param the multiplicative factor.
   */
  void
  mult_stake_distribution (std::vector<stake_t> &stakes, double factor);
};

#endif    // __GENERATE_STAKE_DISTRIBUTION__
