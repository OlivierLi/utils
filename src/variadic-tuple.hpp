#include <cstdint>
#include <iostream>

namespace ID{
    constexpr size_t MessageType{35};
    constexpr size_t Side{54};
    constexpr size_t ExecInst{18};
}

template<size_t N>
using num = std::integral_constant<size_t, N>;

auto typeFromID(num<ID::MessageType>){
    char ret{'D'};
    return ret;
}

auto typeFromID(num<ID::Side>){
    uint64_t ret{19};
    return ret;
}

auto typeFromID(num<ID::ExecInst>){
    const char* ret{"M"};
    return ret;
}

template <size_t... IDs> struct tuple {};

template <size_t ID , size_t... IDs>
struct tuple<ID, IDs...> : tuple<IDs...> {

  tuple() : tuple<IDs...>(), tail(typeFromID(num<ID>{})){}

  decltype(typeFromID(num<ID>{})) tail;
  static constexpr size_t id{ID};
};

template <size_t ID, size_t... IDs>
constexpr bool is_right(){
    constexpr size_t id{tuple<IDs...>::id};
    return id==ID;
} 

template <size_t ID, size_t... IDs, typename std::enable_if_t<is_right<ID, IDs...>(), std::nullptr_t> = nullptr>
auto& get(tuple<IDs...>& t) {
  return t.tail;
}

template <size_t ID, size_t I, size_t... IDs, typename std::enable_if_t<!is_right<ID, I, IDs...>(), std::nullptr_t> = nullptr>
auto& get(tuple<I, IDs...>& t) {

  //TODO : Static assert to verify the the ID we are using is actually usable
  //stackoverflow.com/questions/29603364/type-trait-to-check-that-all-types-in-a-parameter-pack-are-copy-constructible

  tuple<IDs...>& base = t;
  return get<ID>(base);
}

