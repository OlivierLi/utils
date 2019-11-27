#include <limits.h>
#include <cassert>
#include <limits>
#include <type_traits>

template <typename T>
constexpr size_t num_bits() {
  return sizeof(T) * CHAR_BIT;
}

template <size_t i, typename T>
constexpr size_t get_mask() {
  static_assert(std::is_unsigned<T>::value, "This works with unsigned types!");

  // Start off the mask as being full of 1s
  T mask = std::numeric_limits<T>::max();

  // Find out how many bits are in our type
  constexpr size_t count{num_bits<T>()};

  /* Calculate the number of bits to set
     During the first step we want to mask half the bits.
     We halve the number of bits we want to set at every
     iteration. This is achieved by raising to the next
     power of two with the shift
  */
  size_t block_size{count - (count / (1 << (i + 1)))};

  // Create a block of set bits
  size_t bits = (1UL << block_size) - 1;

  // Clear the fist bits in the mask
  mask &= bits;

  return mask;
}

constexpr size_t log2(size_t n) {
  return ((n < 2) ? 1 : 1 + log2(n / 2));
}

// We have the sequence, fill an array with it
template <typename T, std::size_t N, T... Is>
constexpr auto array_from_sequence(std::integer_sequence<T, Is...>) {
  return std::array<T, N>{
      {(get_mask<Is, T>())...}};  // We are working with 1 indexed arrays
}

// Function to get the types needed to create the sequence
template <typename T>
constexpr auto create_array() {
  // We compute the number of masks we will use
  // The number is log2 - 1 because we don't need a mask full of 1s
  constexpr size_t num_masks = log2(num_bits<T>()) - 1;
  return array_from_sequence<T, num_masks>(
      std::make_integer_sequence<T, num_masks>{});
}

template <typename T>
size_t nlzs(T x) {
  size_t leading_zeros{0};

  // The mask starts of half full
  size_t zeroes_in_mask{num_bits<T>() / 2};

  for (auto mask : create_array<T>()) {
    if (x <= mask) {
      leading_zeros += zeroes_in_mask;
      x <<= zeroes_in_mask;
    }
    zeroes_in_mask /= 2;
  }

  return leading_zeros;
}

void test_nlz() {
  size_t value{0};
  for (size_t i = 0; i < CHAR_BIT * sizeof(size_t); ++i) {
    auto temp = value & 1 << i;
    assert(static_cast<size_t>(__builtin_clzl(temp)) == nlzs(temp));
  }
}
