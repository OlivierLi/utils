#pragma once

class SignalTimer {
 public:

   void Start(){
   }

   void Stop(){
   }
 private:
};

class ScopedSignalTrigger {
 public:
  ScopedSignalTrigger(SignalTimer* timer) : timer_(timer) {
    timer->Start();
  }
  
  ~ScopedSignalTrigger(){
    timer_->Stop();
  }

 private:
  // non-owning
  SignalTimer* timer_;
};
