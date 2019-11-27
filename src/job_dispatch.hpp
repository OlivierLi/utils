#pragma once

// Assumptions:
// 1) Taking a lock is considered costly on the platforms of interest to us,
//  even with minimal contention. This is not true on pthread I think.
// 2) Using atomic pointers/indices on the platforms of interest is considered
// cheap. Of course this is not to say that they are free.

// Requirements :

// 1) Ability to share the work between workers with minimal synchronization.
// 2) Each worker has to have the ability to pick up the slack of others if
// necessary.
// 3) We do not need to support retries of jobs within the job dispatching
// mechanism.
//  Assuming that most tasks succeed allows for the jobs to post retry requests
//  to a costly synchronized data structure if necessary on failure.

// Algo 1:
// 1) Each worker starts with a pointer/index to the start of its slice.
// 2) Workers increment that pointer until their is no more work left.

// This option has literally zero synchronization involved. It does not allow
// workers to pick up the slack of others though which can be very important
// with heterogenous workloads. This option also has very good temporal
// localilty because workers will work on the items that were very close to the
// previous one.

// Algo 2:
// 1) Workers pop off work by incrementing an atomic pointer/index into the
// work. They stop if there is no more work at all.

// This option has the advantage of simplicity and slack handling comes for
// free. It has the disadvantage of introducing constant contention which is can
// be costly if we cannot use atomics or if they are not cheap. It also has very
// poor per-worker locality by design,

// Algo 3:
// 1) Each worker recieves it's index at startup and a pointer to the running
// index of the next worker. Whether this is linked list or index based the last
// worker need to know how to wrap around. 2) As long as a worker does not go
// over its personal last item it just keeps going.
//  The worker probably needs a local non-atomic variable for the current
//  work-load. This is needed to prevent two workers to work on the same item at
//  the same time.
// 3) If the worker hits its limit it compares_and_exchanges the index of the
// next worker to the max of the next worker. This grabs some work for the next
// worker. 4) If there is no work in the next worker either the worker coul
// return or traverse the whole structure to find available work.

// This algorithm provides good locality and more coherent work history for the
// workers at the cost of added complexity. Contention is greatly reduced on the
// counter in most cases.

#include <atomic>

struct ControlBlock {
  std::atomic<int> index;
  int work_slice_size;

  ControlBlock* GetNextWorker() {}
};

template <typename T>
class JobDispatcher {
 public:
 private:
};
