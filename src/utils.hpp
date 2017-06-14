
template<size_t> 
constexpr bool has_int(){
    return false;
}

template<size_t Lhs, size_t Rhs, size_t... Values>
constexpr bool has_int(){
    return (Lhs == Rhs) || has_int<Lhs,Values...>() ;
}

void test_has_int(){
  static_assert(has_int<3, 1,2,3,4,5>(), "") ;
  static_assert(has_int<3, 3,3,3,3>(), "") ;
  static_assert(!has_int<3, 1,2,4,5>(), "") ;
  static_assert(!has_int<3>(), "") ;
}
