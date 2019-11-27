#pragma once

#include <chrono>
#include <iostream>

#include <signal.h>
#include <time.h>

using namespace std::chrono_literals;

class SignalTimer {
 public:
  enum class State { Uninitialized, Initialized, Started, Paused, Stopped };

  // This is not for prod use so simply bail on error.
  void Error(const char* msg) {
    std::cerr << msg << std::endl;
    std::abort();
  }

  SignalTimer(int sig, std::chrono::nanoseconds interval)
      : interval_(interval) {
    // Indicate the event we want to send is a signal.
    sev_.sigev_notify = SIGEV_SIGNAL;

    // Set the signal number.
    sev_.sigev_signo = sig;

    // Create the timer. We want a non-settable clock with maximum precision.
    // This does not actually start the timer;
    if (timer_create(CLOCK_MONOTONIC, &sev_, &timer_id_) == -1) {
      Error("Timer creation failed. Aborting");
    }

    // Set the interval of the timer. This also does not start it.
    // This only indicates the frequency at which the timer will repeat
    // if we never pause/stop it.
    timer_spec_.it_interval.tv_nsec = (interval_ % 1s).count();
    timer_spec_.it_interval.tv_sec = (interval_ / 1s);

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
  // Information on the event to trigger.
  struct sigevent sev_;

  // ID of the underlying Linux timer.
  timer_t timer_id_;

  // Info for the timer;
  struct itimerspec timer_spec_;

  // Interval at which the timer repeats.
  std::chrono::nanoseconds interval_;

  // Current state of the timer.
  State state_ = State::Uninitialized;
};

class ScopedSignalTrigger {
 public:
  ScopedSignalTrigger(SignalTimer* timer) : timer_(timer) { timer->Start(); }
  ~ScopedSignalTrigger() { timer_->Pause(); }

 private:
  // non-owning
  SignalTimer* timer_;
};
