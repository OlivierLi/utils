#pragma once

#include <bits/types/sigevent_t.h>
#include <chrono>
#include <iostream>
#include <vector>

#include <signal.h>
#include <time.h>

using namespace std::chrono_literals;

// Default behavior we want is to get a signal sent to the thread that created
// the timer. This variable gets initialized from ScopedSignalTrigger.
pthread_t building_thread_id = 0;

// In this toy example we do not have the necessary machinery to get the real
// ids of all threads. Substite for a function that does that in a real setting.
std::vector<pthread_t> get_all_thread_ids() {
  assert(building_thread_id != 0);
  return {building_thread_id};
}

// TODO: Make all of this into a function local static Singleton to make it
// cleaner.
int signal_to_send_to_all_threads = 0;
void timer_func(union sigval) {
  assert(signal_to_send_to_all_threads != 0);
  for (pthread_t thread_id : get_all_thread_ids()) {
    pthread_kill(thread_id, signal_to_send_to_all_threads);
  }
}

class SignalTimer {
 public:
  enum class State { Uninitialized, Initialized, Started, Paused, Stopped };

  SignalTimer(int sig, std::chrono::nanoseconds interval) {
    // Set the global signal value to know which to trigger on all threads.
    signal_to_send_to_all_threads = sig;

    // Indicate the event we want to invoke is a function ran as if from a new
    // thread.
    sev_.sigev_notify = SIGEV_THREAD;

    // Select the function to use.
    sev_.sigev_notify_function = timer_func;

    // Create the timer. We want a non-settable clock with maximum precision.
    // This does not actually start the timer;
    if (timer_create(CLOCK_MONOTONIC, &sev_, &timer_id_) == -1) {
      Error("Timer creation failed. Aborting");
    }

    // Set the interval of the timer. This also does not start it.
    // This only indicates the frequency at which the timer will repeat
    // if we never pause/stop it.
    timer_spec_.it_interval.tv_nsec = (interval % 1s).count();
    timer_spec_.it_interval.tv_sec = (interval / 1s);

    state_ = State::Initialized;
  }

  ~SignalTimer() { Stop(); }

  // Have the timer trigger
  void Start() {
    if (state_ == State::Uninitialized) {
      Error("Timer stopping before starting. Aborting");
    }

    timer_spec_.it_value.tv_sec = timer_spec_.it_interval.tv_sec;
    timer_spec_.it_value.tv_nsec = timer_spec_.it_interval.tv_nsec;

    if (timer_settime(timer_id_, 0, &timer_spec_, nullptr) == -1) {
      Error("Timer setting failed. Aborting");
    }
    state_ = State::Started;
  }

  // Timer
  void Pause() {
    // Timer was already paused or stopped somewhere else.
    if (state_ != State::Started) {
      return;
    }

    timer_spec_.it_value.tv_sec = 0;
    timer_spec_.it_value.tv_nsec = 0;

    if (timer_settime(timer_id_, 0, &timer_spec_, nullptr) == -1) {
      Error("Timer setting failed. Aborting");
    }
    state_ = State::Paused;
  }

  void Stop() {
    // Calling stop before starting is not a problem.
    if (state_ != State::Started) {
      return;
    }

    if (timer_delete(timer_id_) == -1) {
      Error("Timer deleting failed. Aborting");
    }
    state_ = State::Stopped;
  }

 private:
  // This is not for prod use so simply bail on error.
  void Error(const char* msg) {
    std::cerr << msg << std::endl;
    std::abort();
  }

  // Information on the event to trigger.
  struct sigevent sev_;

  // ID of the underlying Linux timer.
  timer_t timer_id_;

  // Info for the timer;
  struct itimerspec timer_spec_;

  // Current state of the timer.
  State state_ = State::Uninitialized;
};

class ScopedSignalTrigger {
 public:
  ScopedSignalTrigger(SignalTimer* timer) : timer_(timer) {
    timer->Start();

    // Have the default behaviour be that the thread that activates the timer is
    // the one that gets signaled.
    building_thread_id = pthread_self();
  }

  ~ScopedSignalTrigger() { timer_->Pause(); }

 private:
  // non-owning
  SignalTimer* timer_;
};
