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
stake_distribution_generator::few_fat_stake (party_t nr_parties,
                                             double  rich_poor_ratio,
                                             size_t  nr_rich)
{
  std::vector<stake_t> stakes (nr_parties, 1);

  if (nr_rich >= nr_parties)
  {
    nr_rich = nr_parties;
  }

  for (party_t i = 0; i < nr_rich; i++)
  {
    stakes[i] = rich_poor_ratio;
  }

  return stakes;
}

std::vector<stake_t>
stake_distribution_generator::exponential_stake (party_t nr_parties,
                                                 double  richest_poorest_ratio)
{
  // stake of poorest party set to large value so that using integers does not
  // become a problem.
  std::vector<stake_t> stakes (nr_parties, 1000000);

  double r = pow (richest_poorest_ratio, (1.0 / ((double) nr_parties - 1.0)));

  for (party_t i = 1; i < nr_parties; i++)
  {
    stakes[i] = (stake_t) ((double) stakes[i - 1]) * r;
  }
  return stakes;
}

std::vector<stake_t>
stake_distribution_generator::few_fat_thin_stake (party_t nr_parties,
                                                  double  rich_poor_ratio,
                                                  size_t  nr_rich)
{
  std::vector<stake_t> stakes =
    few_fat_stake (nr_parties, rich_poor_ratio, nr_rich);
  stake_t rich_stake     = stakes[0];
  stakes[0]              = stakes[nr_parties - 1];
  stakes[nr_parties - 1] = rich_stake;
  return stakes;
}

std::vector<stake_t>
stake_distribution_generator::few_fat_fat_stake (party_t nr_parties,
                                                 double  rich_poor_ratio,
                                                 size_t  nr_rich)
{
  return few_fat_stake (nr_parties, rich_poor_ratio, nr_rich);
}

std::vector<stake_t>
stake_distribution_generator::exponential_thinnest_stake (
  party_t nr_parties, double richest_poorest_ratio)
{
  return exponential_stake (nr_parties, richest_poorest_ratio);
}

std::vector<stake_t>
stake_distribution_generator::exponential_median_stake (
  party_t nr_parties, double richest_poorest_ratio)
{
  std::vector<stake_t> stakes =
    exponential_stake (nr_parties, richest_poorest_ratio);

  stake_t median_stake   = stakes[nr_parties / 2];
  stakes[nr_parties / 2] = stakes[0];
  stakes[0]              = median_stake;
  return stakes;
}

std::vector<stake_t>
stake_distribution_generator::exponential_fattest_stake (
  party_t nr_parties, double richest_poorest_ratio)
{
  std::vector<stake_t> stakes =
    exponential_stake (nr_parties, richest_poorest_ratio);

  stake_t rich_stake     = stakes[nr_parties - 1];
  stakes[nr_parties - 1] = stakes[0];
  stakes[0]              = rich_stake;
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
