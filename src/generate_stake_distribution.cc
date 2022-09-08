#include "generate_stake_distribution.h"

#ifdef __GENERATE_STAKE_DISTRIBUTION__

stake_distribution_generator::stake_distribution_generator ()
{
  this->re = std::default_random_engine (this->rd ());
}

std::vector<stake_t>
stake_distribution_generator::constant_stake (party_t nr_parties, stake_t s)
{
  return std::vector<stake_t> (nr_parties, s);
}

std::vector<stake_t>
stake_distribution_generator::few_heavy_stake (party_t nr_parties,
                                               double  heavy_light_ratio,
                                               size_t  nr_heavy)
{
  std::vector<stake_t> stakes (nr_parties, 1);

  if (nr_heavy >= nr_parties)
  {
    nr_heavy = nr_parties;
  }

  for (party_t i = 0; i < nr_heavy; i++)
  {
    stakes[i] = heavy_light_ratio;
  }

  return stakes;
}

std::vector<stake_t>
stake_distribution_generator::exponential_stake (party_t nr_parties,
                                                 double  heaviest_lightest_ratio)
{
  // stake of lightest party set to large value so that using integers does not
  // become a problem.
  std::vector<stake_t> stakes (nr_parties, 1000000);

  double r = pow (heaviest_lightest_ratio, (1.0 / ((double) nr_parties - 1.0)));

  for (party_t i = 1; i < nr_parties; i++)
  {
    stakes[i] = (stake_t) ((double) stakes[i - 1]) * r;
  }
  return stakes;
}

std::vector<stake_t>
stake_distribution_generator::few_heavy_light_stake (party_t nr_parties,
                                                     double  heavy_light_ratio,
                                                     size_t  nr_heavy)
{
  std::vector<stake_t> stakes =
    few_heavy_stake (nr_parties, heavy_light_ratio, nr_heavy);
  stake_t heavy_stake     = stakes[0];
  stakes[0]              = stakes[nr_parties - 1];
  stakes[nr_parties - 1] = heavy_stake;
  return stakes;
}

std::vector<stake_t>
stake_distribution_generator::few_heavy_heavy_stake (party_t nr_parties,
                                                     double  heavy_light_ratio,
                                                     size_t  nr_heavy)
{
  return few_heavy_stake (nr_parties, heavy_light_ratio, nr_heavy);
}

std::vector<stake_t>
stake_distribution_generator::exponential_lightest_stake (
  party_t nr_parties, double heaviest_lightest_ratio)
{
  return exponential_stake (nr_parties, heaviest_lightest_ratio);
}

std::vector<stake_t>
stake_distribution_generator::exponential_median_stake (
  party_t nr_parties, double heaviest_lightest_ratio)
{
  std::vector<stake_t> stakes =
    exponential_stake (nr_parties, heaviest_lightest_ratio);

  stake_t median_stake   = stakes[nr_parties / 2];
  stakes[nr_parties / 2] = stakes[0];
  stakes[0]              = median_stake;
  return stakes;
}

std::vector<stake_t>
stake_distribution_generator::exponential_heaviest_stake (
  party_t nr_parties, double heaviest_lightest_ratio)
{
  std::vector<stake_t> stakes =
    exponential_stake (nr_parties, heaviest_lightest_ratio);

  stake_t heavy_stake     = stakes[nr_parties - 1];
  stakes[nr_parties - 1] = stakes[0];
  stakes[0]              = heavy_stake;
  return stakes;
}

std::vector<stake_t>
stake_distribution_generator::sample_uniform_stake (party_t nr_parties,
                                                    stake_t lower_bound,
                                                    stake_t upper_bound)
{
  // random weights in [lower_bound, upper_bound)
  std::uniform_int_distribution<stake_t> urd (lower_bound, upper_bound);

  std::vector<stake_t> stakes (nr_parties);

  for (party_t i = 0; i < nr_parties; ++i)
  {
    stakes[i] = urd (this->re);
  }

  return stakes;
}

std::vector<stake_t>
stake_distribution_generator::sample_geometric_stake (party_t nr_parties,
                                                      double  p)
{
  std::geometric_distribution<stake_t> grd (p);

  std::vector<stake_t> stakes (nr_parties);

  for (party_t i = 0; i < nr_parties; ++i)
  {
    stakes[i] = grd (this->re);
  }

  return stakes;
}

std::vector<stake_t>
stake_distribution_generator::sample_exponential_stake (party_t nr_parties,
                                                        double  lambda)
{
  std::exponential_distribution<double> erd (lambda);

  std::vector<stake_t> stakes (nr_parties);

  for (party_t i = 0; i < nr_parties; ++i)
  {
    stakes[i] = (stake_t) erd (this->re);
  }

  return stakes;
}


void
stake_distribution_generator::add_to_stake_distribution (
  std::vector<stake_t> &stakes, stake_t v)
{
  for (party_t i = 0; i < stakes.size (); ++i)
  {
    stakes[i] += v;
  }
}

void
stake_distribution_generator::mult_stake_distribution (
  std::vector<stake_t> &stakes, double factor)
{
  for (party_t i = 0; i < stakes.size (); ++i)
  {
    stakes[i] = (stake_t) (((double) stakes[i]) * factor);
  }
}


#endif    // __GENERATE_STAKE_DISTRIBUTION__
