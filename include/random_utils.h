#include <random>
#include <unordered_set>
#include <string.h>

#include "yal_log.h"
#include "scoped_lock.h"


#ifndef __RANDOM_UTILS__
#define __RANDOM_UTILS__

/**
 * @class random_generator
 * @brief class used for creating random generators.
 *
 * Class for creating randomness generators.
 */
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

  /**
   * Generates a new random engine with a fresh seed.
   * @return a new random engine.
   */
  std::mt19937_64 *
  get_random_engine ();
};

/**
 * Initializes the randomness utility functions.
 */
void
init_random_utils ();

/**
 * Generates a new random generator with a fresh seed.
 * @return a pointer to a new random generator.
 */
std::mt19937_64 *
get_random_generator ();

#endif    // __RANDOM_UTILS__
