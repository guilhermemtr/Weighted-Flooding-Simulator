#include "scoped_lock.h"

#ifdef __SCOPED_LOCK__

scoped_lock::scoped_lock (pthread_mutex_t *l, pthread_cond_t *c) : l (l), c (c)
{
#ifdef __SL_ASSERT__
  assert (!pthread_mutex_lock (l));
#else
  pthread_mutex_lock (l);
#endif
}

void
scoped_lock::wait ()
{
#ifdef __SL_ASSERT__
  if (c == NULL)
    throw "condition is null";
  assert (!pthread_cond_wait (c, l));
#else
  pthread_cond_wait (c, l);
#endif
}

void
scoped_lock::signal ()
{
#ifdef __SL_ASSERT__
  if (c == NULL)
    throw "condition is null";
  assert (!pthread_cond_signal (c));
#else
  pthread_cond_signal (c);
#endif
}

void
scoped_lock::broadcast ()
{
#ifdef __SL_ASSERT__
  if (c == NULL)
    throw "condition is null";
  assert (!pthread_cond_broadcast (c));
#else
  pthread_cond_broadcast (c);
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
scoped_lock::init_lock (pthread_mutex_t *l, pthread_cond_t *c)
{
#ifdef __SL_ASSERT__
  assert (!pthread_mutex_init (l, NULL));
  if (c != NULL)
    assert (!pthread_cond_init (c, NULL));
#else
  pthread_mutex_init (l, NULL);
  if (c != NULL)
    pthread_cond_init (c, NULL);
#endif
}

void
scoped_lock::del_lock (pthread_mutex_t *l, pthread_cond_t *c)
{
#ifdef __SL_ASSERT__
  if (c != NULL)
    assert (!pthread_cond_destroy (c));
  assert (!pthread_mutex_destroy (l));
#else
  if (c != NULL)
    pthread_cond_destroy (c);
  pthread_mutex_destroy (l);
#endif
}

#endif
