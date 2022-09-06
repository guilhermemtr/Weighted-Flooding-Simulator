#include "random_utils.h"

#ifdef __RANDOM_UTILS__

random_generator *rg = NULL;

static uint64_t
get_64_seed ()
{
  // supposed to be thread safe
  std::random_device rd ("/dev/urandom");

  double entropy = rd.entropy ();

  __yal_log (
    __YAL_DBG, "<random_utils.cc:get_64_seed()>: %lf bits of entropy", entropy);
  if (!entropy)
  {
    throw std::string ("Low entropy:") + std::to_string (entropy);
  }

  uint32_t shift     = entropy;
  uint32_t seed_size = 0;
  uint64_t seed      = 0;

  while (seed_size < 64)
  {
    seed               = seed << shift;
    uint64_t seed_part = rd ();
    seed               = seed | seed_part;
    seed_size += shift;
  }

  return seed;
}

void
init_random_utils ()
{
  if (rg == NULL)
  {
    rg = new random_generator ();
  }
}

std::mt19937_64 *
get_random_generator ()
{
  return rg->get_random_engine ();
}

random_generator::random_generator ()
{
  this->re    = std::mt19937_64 (get_64_seed ());
  this->seeds = {};
  scoped_lock::init_lock (&(this->mutex));
}

random_generator::~random_generator ()
{
  scoped_lock::del_lock (&(this->mutex));
}

std::mt19937_64 *
random_generator::get_random_engine ()
{
  scoped_lock sl (&(this->mutex));

  uint64_t seed = this->re ();
  if (this->seeds.count (seed))
  {
    throw std::string ("<random_utils.cc:get_random_engine()>: SEED COLLISION");
  }
  this->seeds.insert (seed);

  return new std::mt19937_64 (seed);
}

#endif    // __RANDOM_UTILS__
