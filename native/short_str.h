#pragma once

#include <array>

namespace native {

template <size_t N>
struct ShortStr {
  std::array<char, N> s;
  constexpr const char* c_str() const { return s.data(); }
};

using Str8 = ShortStr<8>;

}  // namespace native
