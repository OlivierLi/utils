#include <array>

template <typename T, size_t N>
constexpr T& get(const std::array<T, N>& arr, size_t i) {
  // Cast to const array to access the constexpr operator[]
  // Const cast the acquired ref to be able to modify it
  return const_cast<char&>(static_cast<decltype(arr) const&>(arr)[i]);
}

template <size_t N>
struct ArrayView {
  constexpr ArrayView() : arr{} {
    constexpr char start{'a'};
    for (size_t i = 0; i < N - 1; ++i) {
      get(arr, i) = start + i;
    }

    get(arr, N - 1) = '\0';
  }

  std::array<char, N> arr;
};

void test_array() {
  static constexpr auto view = ArrayView<4>();
  std::cout << view.arr.begin() << std::endl;
}
