#include <cstdint>
#include <iostream>

template <typename... Ts> struct tuple {};

template <typename N , typename T, typename... Ts>
struct tuple<N, T,  Ts...> : tuple<Ts...> {

  tuple(N, T t, Ts... ts) : tuple<Ts...>(ts...), tail(t){}

  T tail;
  static constexpr size_t id{N::value};
};

template <size_t N, typename... Ts>
constexpr bool is_right(){
    constexpr size_t id{tuple<Ts...>::id};
    return id==N;
} 

template <size_t N, typename... Ts, typename std::enable_if_t<is_right<N, Ts...>(), std::nullptr_t> = nullptr>
auto& get(tuple<Ts...>& t) {
  return t.tail;
}

template <size_t N, typename I, typename T, typename... Ts, typename std::enable_if_t<!is_right<N, I, T, Ts...>(), std::nullptr_t> = nullptr>
auto& get(tuple<I, T, Ts...>& t) {

  //TODO : Static assert to verify the the ID we are using is actually usable
  //stackoverflow.com/questions/29603364/type-trait-to-check-that-all-types-in-a-parameter-pack-are-copy-constructible

  tuple<Ts...>& base = t;
  return get<N>(base);
}
