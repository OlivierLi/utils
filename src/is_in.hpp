#include <algorithm>
#include <array>
#include <iostream>
#include <set>

// Used to determine whether or not the class has a find member function
template <typename T>
class has_find {
 private:
  // If the find function is available the first statement in decltype is valid
  // and the chosen type will be std::true_type
  template <typename C>
  static auto test(void*)
      -> decltype(std::declval<C>().find({}), std::true_type{});

  // This is the fallback function, if the decltype does not work at the
  // previous step
  template <typename>
  static std::false_type test(...);

 public:
  // Return type of test is determined using SFINAE
  // Having the return type be std::true_type means we have a find function
  // Represent that in a bool value
  using return_type = decltype(test<T>(nullptr));
  static constexpr bool value =
      std::is_same<return_type, std::true_type>::value;
};

// This overload is made invalid if the find function is not present
template <typename K,
          typename C,
          std::enable_if_t<has_find<C>::value, std::nullptr_t> = nullptr>
bool is_in(K key, const C& container) {
  return container.find(key) != container.end();
}

// This overload is made invalid if the find function is present
template <typename K,
          typename C,
          std::enable_if_t<!has_find<C>::value, std::nullptr_t> = nullptr>
bool is_in(K key, const C& container) {
  return std::find(std::begin(container), std::end(container), key) !=
         container.end();
}

void test_has_in() {
  std::array<size_t, 2> data_a{{0, 1}};
  static_assert(!has_find<decltype(data_a)>::value, "Array does not have find");
  std::cout << std::boolalpha << is_in(5, data_a) << std::endl;
  std::cout << std::boolalpha << is_in(1, data_a) << std::endl;

  std::set<size_t> data_s{0, 1};
  static_assert(has_find<decltype(data_s)>::value, "Set has find");
  std::cout << std::boolalpha << is_in(5, data_s) << std::endl;
  std::cout << std::boolalpha << is_in(1, data_s) << std::endl;
}
