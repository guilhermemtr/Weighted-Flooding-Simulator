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
  pthread_cond_t  *c;

    public:
  scoped_lock (pthread_mutex_t *l, pthread_cond_t *c = NULL);

  void
  wait ();
  void
  signal ();
  void
  broadcast ();

  virtual ~scoped_lock ();

  static void
  init_lock (pthread_mutex_t *l, pthread_cond_t *c = NULL);
  static void
  del_lock (pthread_mutex_t *l, pthread_cond_t *c = NULL);
};

#endif
