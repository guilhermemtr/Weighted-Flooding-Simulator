#include "scoped_lock.h"

#ifdef __SCOPED_LOCK__

scoped_lock::scoped_lock (pthread_mutex_t *l) : l (l)
{
#ifdef __SL_ASSERT__
  assert (!pthread_mutex_lock (l));
#else
  pthread_mutex_lock (l);
#endif
}

scoped_lock::~scoped_lock ()
{
#ifdef __SL_ASSERT__
  assert (!pthread_mutex_unlock (l));
#else
  pthread_mutex_unlock (l);
#endif
}

void
scoped_lock::init_lock (pthread_mutex_t *l)
{
#ifdef __SL_ASSERT__
  assert (!pthread_mutex_init (l, NULL));
#else
  pthread_mutex_init (l, NULL);
#endif
}

void
scoped_lock::del_lock (pthread_mutex_t *l)
{
#ifdef __SL_ASSERT__
  assert (!pthread_mutex_destroy (l));
#else
  pthread_mutex_destroy (l);
#endif
}

#endif
