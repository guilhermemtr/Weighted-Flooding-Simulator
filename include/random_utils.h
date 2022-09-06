#include <random>
#include <unordered_set>
#include <string.h>

#include "yal_log.h"
#include "scoped_lock.h"


#ifndef __RANDOM_UTILS__
#define __RANDOM_UTILS__

class random_generator
{
    private:
  pthread_mutex_t mutex;
  std::mt19937_64 re;

  // to check for re-used seeds (for debugging purposes)
  std::unordered_set<uint64_t> seeds;

    public:
  random_generator ();
  virtual ~random_generator ();

  std::mt19937_64 *
  get_random_engine ();
};

void
init_random_utils ();

std::mt19937_64 *
get_random_generator ();

#endif    // __RANDOM_UTILS__
