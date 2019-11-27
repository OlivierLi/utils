#include <cstdint>
#include <iostream>
#include "utils.hpp"

namespace ID {
constexpr size_t MessageType{35};
constexpr size_t Side{54};
constexpr size_t ExecInst{18};
}  // namespace ID

template <size_t N>
using num = std::integral_constant<size_t, N>;

auto typeFromID(num<ID::MessageType>) {
  char ret{'D'};
  return ret;
}

auto typeFromID(num<ID::Side>) {
  uint64_t ret{19};
  return ret;
}

auto typeFromID(num<ID::ExecInst>) {
  const char* ret{"M"};
  return ret;
}

template <size_t... IDs>
struct tuple {};

template <size_t ID, size_t... IDs>
struct tuple<ID, IDs...> : tuple<IDs...> {
  tuple() : tuple<IDs...>(), tail(typeFromID(num<ID>{})) {}

  decltype(typeFromID(num<ID>{})) tail;
  static constexpr size_t id{ID};
};

template <size_t ID, size_t... IDs>
constexpr bool is_right() {
  constexpr size_t id{tuple<IDs...>::id};
  return id == ID;
}

template <
    size_t ID,
    size_t... IDs,
    typename std::enable_if_t<is_right<ID, IDs...>(), std::nullptr_t> = nullptr>
auto& get(tuple<IDs...>& t) {
  return t.tail;
}

// TODO : Rename ID to something better. It's the ID we are looking for. Could
// be better called TARGET, RHS or value?.
// TODO : Rename I to something better. It's the sliced off first ID. Could be
// better called ID
template <size_t ID,
          size_t I,
          size_t... IDs,
          typename std::enable_if_t<!is_right<ID, I, IDs...>(),
                                    std::nullptr_t> = nullptr>
auto& get(tuple<I, IDs...>& t) {
  static_assert(has_int<ID, I, IDs...>(), "ID not in tuple!");

  tuple<IDs...>& base = t;
  return get<ID>(base);
}
