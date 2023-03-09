/******************************************************************************/
/* Important Spring 2023 CSCI 402 usage information:                          */
/*                                                                            */
/* This fils is part of CSCI 402 kernel programming assignments at USC.       */
/*         53616c7465645f5fd1e93dbf35cbffa3aef28f8c01d8cf2ffc51ef62b26a       */
/*         f9bda5a68e5ed8c972b17bab0f42e24b19daa7bd408305b1f7bd6c7208c1       */
/*         0e36230e913039b3046dd5fd0ba706a624d33dbaa4d6aab02c82fe09f561       */
/*         01b0fd977b0051f0b0ce0c69f7db857b1b5e007be2db6d42894bf93de848       */
/*         806d9152bd5715e9                                                   */
/* Please understand that you are NOT permitted to distribute or publically   */
/*         display a copy of this file (or ANY PART of it) for any reason.    */
/* If anyone (including your prospective employer) asks you to post the code, */
/*         you must inform them that you do NOT have permissions to do so.    */
/* You are also NOT permitted to remove or alter this comment block.          */
/* If this comment block is removed or altered in a submitted file, 20 points */
/*         will be deducted.                                                  */
/******************************************************************************/

#include "proc/kmutex.h"

#include "errno.h"
#include "globals.h"
#include "proc/kthread.h"
#include "util/debug.h"

/** Helper function from sched.c */
extern kthread_t *ktqueue_dequeue(ktqueue_t *q);

/*
 * IMPORTANT: Mutexes can _NEVER_ be locked or unlocked from an
 * interrupt context. Mutexes are _ONLY_ lock or unlocked from a
 * thread context.
 */

void kmutex_init(kmutex_t *mtx) {
  // NOT_YET_IMPLEMENTED("PROCS: kmutex_init");

  sched_queue_init(&mtx->km_waitq);
  mtx->km_holder = NULL;
  dbg(DBG_PRINT, "(GRADING1C)\n");
}

/*
 * This should block the current thread (by sleeping on the mutex's
 * wait queue) if the mutex is already taken.
 *
 * No thread should ever try to lock a mutex it already has locked.
 */
void kmutex_lock(kmutex_t *mtx) {
  //  NOT_YET_IMPLEMENTED("PROCS: kmutex_lock");

  /* curthr must be valid and it must not be holding the mutex (mtx) already */
  KASSERT(curthr && (curthr != mtx->km_holder));
  dbg(DBG_PRINT, "(GRADING1A 6.a)\n");

  if (mtx->km_holder) {
    // Another thread is holding the mutex. We sleep this thread on the mutex
    // queue to be woken up when it gets to the front of the queue
    sched_sleep_on(&mtx->km_waitq);

    // The thread wakes up from sleep because it got woken up from the mutex
    // queue it will start executing from here. At this point, this thread would
    // already be the holder of the mutex, because before we moved this thread
    // from the mutex queue to the run queue, we would have made it the holder.
    dbg(DBG_PRINT, "(GRADING1C)\n");
  } else {
    // The mutex is available so we just assign the current thread as the owner.
    mtx->km_holder = curthr;
    dbg(DBG_PRINT, "(GRADING1C)\n");
  }
  dbg(DBG_PRINT, "(GRADING1C)\n");
}

/*
 * This should do the same as kmutex_lock, but use a cancellable sleep
 * instead. Also, if you are cancelled while holding mtx, you should unlock mtx.
 */
int kmutex_lock_cancellable(kmutex_t *mtx) {
  //   NOT_YET_IMPLEMENTED("PROCS: kmutex_lock_cancellable");

  /* curthr must be valid and it must not be holding the mutex (mtx) already */
  KASSERT(curthr && (curthr != mtx->km_holder));
  dbg(DBG_PRINT, "(GRADING1A 6.b)\n");
  dbg(DBG_PRINT, "(GRADING1C)\n");

  if (curthr->kt_cancelled) {
    // This function is a cancellation point in the weenix kernel. So if the
    // current thread is already cancelled when it enters this function, we
    // return immediately with -EINTR
    dbg(DBG_PRINT, "(GRADING1C)\n");
    return -EINTR;
  }

  if (mtx->km_holder) {
    // Another thread is holding the mutex. We sleep this thread on the mutex
    // queue to be woken up when it gets to the front of the queue. The sleep is
    // cancellable and if this thread could be cancelled while it was in its
    // sleep. The return value of the sleep call indicates whether the thread
    // was cancelled in its sleep.

    // If the thread woke up from the sleep with sched_cancellable_sleep_on
    // returning -EINTR, we know that the thread was cancelled while it was
    // sleeping. In this case, we don't acquire the mutex and return with
    // -EINTR.

    // Otherwise, the thread was woken up from sleep because it got to the front
    // of the mutex queue and it wasn't cancelled while it was sleeping. In this
    // case, we return 0 (which is the return value of
    // sched_cancellable_sleep_on if the sleep was not cancelled) At this point,
    // this thread would already be the holder of the mutex, because before we
    // moved this thread from the mutex queue to the run queue, we would have
    // made it the holder.

    dbg(DBG_PRINT, "(GRADING1C)\n");
    return sched_cancellable_sleep_on(&mtx->km_waitq);

  } else {
    // The mutex is available so we just assign the current thread as the owner.
    mtx->km_holder = curthr;
    dbg(DBG_PRINT, "(GRADING1C)\n");
  }

  dbg(DBG_PRINT, "(GRADING1C)\n");
  return 0;
}

/*
 * If there are any threads waiting to take a lock on the mutex, one
 * should be woken up and given the lock.
 *
 * Note: This should _NOT_ be a blocking operation!
 *
 * Note: Ensure the new owner of the mutex enters the run queue.
 *
 * Note: Make sure that the thread on the head of the mutex's wait
 * queue becomes the new owner of the mutex.
 *
 * @param mtx the mutex to unlock
 */
void kmutex_unlock(kmutex_t *mtx) {
  //   NOT_YET_IMPLEMENTED("PROCS: kmutex_unlock");

  /* curthr must be valid and it must currently holding the mutex (mtx) */
  KASSERT(curthr && (curthr == mtx->km_holder));
  dbg(DBG_PRINT, "(GRADING1A 6.c)\n");

  if (sched_queue_empty(&mtx->km_waitq)) {
    // There is no thread waiting for the mutex, we just set the holder to NULL.
    mtx->km_holder = NULL;
    dbg(DBG_PRINT, "(GRADING1C)\n");
  } else {
    // If there is at least one thread waiting for the mutex, we deque it from
    // the mutex queue, make it the holder of the mutex and make it runnable.

    // Technically, we pick the thread from the 'tail' of the queue and not the
    // 'head' because ktqueue_dequeue, dequeues from the tail. This is the
    // convention we follow in weenix where we enqueue to the head and dequeue
    // from the tail.
    kthread_t *thr = ktqueue_dequeue(&mtx->km_waitq);
    mtx->km_holder = thr;
    sched_make_runnable(thr);
    dbg(DBG_PRINT, "(GRADING1C)\n");
  }

  /* on return, curthr must not be the mutex (mtx) holder */
  KASSERT(curthr != mtx->km_holder);
  dbg(DBG_PRINT, "(GRADING1A 6.c)\n");
}
