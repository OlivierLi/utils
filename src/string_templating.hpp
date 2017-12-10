// Get the length of the string
constexpr size_t len(const char* str){
    if(!str || *str == '\0'){
        return 0;
    }
    else{
        return 1+len(++str);
    }
}

static constexpr char OPEN{'{'};
static constexpr char CLOSE{'}'};

// Validate that marker are closed correctly
constexpr bool is_valid(const char* str, size_t count=0){
    if(!str || *str == '\0'){
        return count == 0;
    }
    else{
        if(*str == OPEN){
            ++count;
        }
        else if(*str == CLOSE){
            --count;
        }
        return is_valid(++str, count);
    }
}

constexpr size_t size(const char* str){

    while(*str){
        ++str;
    }

    return 0;
}

constexpr std::uint8_t char_to_num(char c){
    if(c < 48 || c > 57){
       throw  std::exception{}; 
    }
    return static_cast<std::uint8_t>(c)-48;
}

void test_template(){
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
}
