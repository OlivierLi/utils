#pragma once

class SignalTimer {
 public:
  SignalTimer() {}

  void Start() {}

  void Pause() {}

  void Stop() {}

 private:
};

class ScopedSignalTrigger {
 public:
  ScopedSignalTrigger(SignalTimer* timer) : timer_(timer) { timer->Start(); }

  ~ScopedSignalTrigger() { timer_->Pause(); }

 private:
  // non-owning
  SignalTimer* timer_;
};
