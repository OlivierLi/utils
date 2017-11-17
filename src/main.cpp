#include <iostream>
#include "is_in.hpp"
#include "type_print.hpp"
#include "constexpr_array.hpp"
#include "variadic-tuple.hpp"
#include "string_templating.hpp"

int main(int, char**){
    constexpr const char* valid = "{}{}{}{}";
    constexpr const char* invalid = "{}}{}{}";
    constexpr const char* empty = "";

    static_assert(len(valid) == 8, "Len not working");
    static_assert(len(invalid) == 7, "Len not working");
    static_assert(len(nullptr) == 0, "Len not working");
    static_assert(len(empty) == 0, "Len not working");

    static_assert(is_valid(valid), "Template is valid");
    static_assert(!is_valid(invalid), "Template is valid");

    static_assert(char_to_num('0') == 0, "Bad char/num conversion");
    static_assert(char_to_num('9') == 9, "Bad char/num conversion");

    return 0;
}
