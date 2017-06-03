#include <iostream>
#include "is_in.hpp"
#include "type_print.hpp"
#include "constexpr_array.hpp"
#include "variadic-tuple.hpp"

int main(int, char**){

  using tuple_t = tuple<ID::MessageType, ID::Side, ID::ExecInst>;
  tuple_t t1;

  std::cout << "Elem with ID " << ID::MessageType<< " is: " << get<ID::MessageType>(t1) << "\n";
  std::cout << "Elem with ID " << ID::Side<< " is: " << get<ID::Side>(t1) << "\n";
  std::cout << "Elem with ID " << ID::ExecInst<< " is: " << get<ID::ExecInst>(t1) << "\n";

  get<ID::MessageType>(t1) = '0';
  std::cout << "Elem with ID " << ID::MessageType<< " is: " << get<ID::MessageType>(t1) << "\n";

  return 0;
}
