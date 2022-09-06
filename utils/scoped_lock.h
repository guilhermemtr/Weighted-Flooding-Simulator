#ifndef __SCOPED_LOCK__
#define __SCOPED_LOCK__

#include <pthread.h>
#include <assert.h>

// If __SL_ASSERT__ is defined, each method uses assert on operations,
// and also checks if the condition variable was set.
// otherwise, no assertions are made.

class scoped_lock
{
private:
  pthread_mutex_t *l;

public:
  scoped_lock (pthread_mutex_t *l);

  virtual ~scoped_lock ();

  static void
  init_lock (pthread_mutex_t *l);
  static void
  del_lock (pthread_mutex_t *l);
};

#endif
