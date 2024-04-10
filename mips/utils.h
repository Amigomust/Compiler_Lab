#pragma once

#include <cstdio>

#include "native/short_str.h"

namespace mips {

inline void binary_left(const char* op, native::Str8 lhs, native::Str8 rhs) {
  std::printf("%s %s, %s, %s\n", op, lhs, rhs, lhs);
}

inline void binary_right(const char* op, native::Str8 lhs, native::Str8 rhs) {
  std::printf("%s %s, %s, %s\n", op, lhs, rhs, rhs);
}

}  // namespace mips
