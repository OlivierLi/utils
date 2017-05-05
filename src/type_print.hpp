//Taken from : http://stackoverflow.com/questions/35941045/can-i-obtain-c-type-names-in-a-constexpr-way/35943472#35943472
#include <array>
#include <iostream>

struct string_view
{
    char const* data;
    std::size_t size;
};

inline std::ostream& operator<<(std::ostream& o, string_view const& s)
{
    return o.write(s.data, s.size);
}

template<class T>
constexpr string_view get_name()
{
    char const* p = __PRETTY_FUNCTION__;
    while (*p++ != '=');
    for (; *p == ' '; ++p);
    char const* p2 = p;
    int count = 1;
    for (;;++p2)
    {
        switch (*p2)
        {
            case '[':
                ++count;
                break;
            case ']':
                --count;
                if (!count)
                    return {p, std::size_t(p2 - p)};
        }
    }
    return {};
}
