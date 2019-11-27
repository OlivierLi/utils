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

namespace {
SignalTimer signal_timer;
}  // namespace

int main(int, char**) {
  ScopedSignalTrigger trigger(&signal_timer);

  return 0;
}
