#ifndef SHORT_STR
#define SHORT_STR

#include <array>
#include <string>

#ifdef LOCAL
#include "../debug.h"
#else
#define debug(...) {}
#endif

namespace native {

// template <size_t N>
// struct ShortStr {
//   std::array<char, N> s;
//   constexpr const char* c_str() const { return s.data(); }
//   friend template<typename T> T to_string(int x);
// };

using Str16 = std::string;

// template<typename T>
// T to_string(int x) {
//   T res;
//   int i = 0;
//   while (x) {
//     res.s[i++] = x % 10 + '0';
//     x /= 10;
//   }
//   return res;
// }

}  // namespace native
#endif