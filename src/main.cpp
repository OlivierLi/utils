#include <iostream>
#include "is_in.hpp"
#include "type_print.hpp"
#include "constexpr_array.hpp"
#include "variadic-tuple.hpp"


int main(int, char**){
  //---------------------------------------------------------------------------
    
  //TODO : To avoid having to list everything twice : Implement something like std::make_tuple?
  using tuple_t = tuple<num<7>, double, num<8>, uint64_t, num<9>, const char*>;
  tuple_t t1(num<7>(), 12.2, num<8>(), 42, num<9>(), "big");

  std::cout << "Elem with ID 7 is: " << get<7>(t1) << "\n";
  std::cout << "Elem with ID 8 is: " << get<8>(t1) << "\n";
  std::cout << "Elem with ID 9 is: " << get<9>(t1) << "\n";

  get<7>(t1) = 103;
  std::cout << "Elem with ID 7 is: " << get<7>(t1) << "\n";

  return 0;
}
