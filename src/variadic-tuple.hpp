#include <cstdint>
#include <iostream>

template<size_t N>
using num = std::integral_constant<size_t, N>;

auto typeFromID(num<7>){
    double ret{17.0};
    return ret;
}

auto typeFromID(num<8>){
    uint64_t ret{19};
    return ret;
}

auto typeFromID(num<9>){
    const char* ret{"A"};
    return ret;
}

template <typename... Ts> struct tuple {};

template <typename ID , typename T, typename... Ts>
struct tuple<ID, T,  Ts...> : tuple<Ts...> {

  tuple() : tuple<Ts...>(), tail(typeFromID(ID{})){}

  decltype(typeFromID(ID{})) tail;
  static constexpr size_t id{ID::value};
};

template <size_t ID, typename... Ts>
constexpr bool is_right(){
    constexpr size_t id{tuple<Ts...>::id};
    return id==ID;
} 

template <size_t ID, typename... Ts, typename std::enable_if_t<is_right<ID, Ts...>(), std::nullptr_t> = nullptr>
auto& get(tuple<Ts...>& t) {
  return t.tail;
}

template <size_t ID, typename I, typename T, typename... Ts, typename std::enable_if_t<!is_right<ID, I, T, Ts...>(), std::nullptr_t> = nullptr>
auto& get(tuple<I, T, Ts...>& t) {

  //TODO : Static assert to verify the the ID we are using is actually usable
  //stackoverflow.com/questions/29603364/type-trait-to-check-that-all-types-in-a-parameter-pack-are-copy-constructible

  tuple<Ts...>& base = t;
  return get<ID>(base);
}
