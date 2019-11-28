#include <iomanip>
#include <iostream>
#include <thread>

#include "constexpr_array.hpp"
#include "is_in.hpp"
#include "nlz.hpp"
#include "scoped_signal.hpp"
#include "string_templating.hpp"
#include "type_print.hpp"
#include "variadic-tuple.hpp"

using namespace std::chrono_literals;

namespace {

// Simply kill after 500 millis.
SignalTimer signal_timer(SIGKILL, 500ms);

}  // namespace

int main(int, char**) {
  ScopedSignalTrigger trigger(&signal_timer);

  std::this_thread::sleep_for(1s);

  return 0;
}
