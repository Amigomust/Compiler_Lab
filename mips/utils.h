#ifndef UTILS
#define UTILS

#include <cstdio>

#include "../native/short_str.h"


namespace mips {

inline void binary_left(const char* op, native::Str16 lhs, native::Str16 rhs) {
  std::printf("%s %s, %s, %s\n", op, lhs.c_str(), lhs.c_str(), rhs.c_str());
}

inline void binary_right(const char* op, native::Str16 lhs, native::Str16 rhs) {
  std::printf("%s %s, %s, %s\n", op, rhs.c_str(), lhs.c_str(), rhs.c_str());
}

inline void binary_imm(const char* op, native::Str16 lhs, const int imm) {
  std::printf("%s %s, %s, %d\n", op, lhs.c_str(), lhs.c_str(), imm);
}

inline void binary_div_left(const char* op, const char* op2, native::Str16 lhs, native::Str16 rhs) {
  std::printf("%s %s, %s\n", op, lhs.c_str(), rhs.c_str());
  std::printf("%s %s\n", op2, lhs.c_str());
}

inline void binary_div_right(const char* op, const char* op2, native::Str16 lhs, native::Str16 rhs) {
  std::printf("%s %s, %s\n", op, lhs.c_str(), rhs.c_str());
  std::printf("%s %s\n", op2, rhs.c_str());
}

// save and load
inline void binary_access(const char* op, const native::Str16 reg, const native::Str16 stk) {
  std::printf("%s %s, %s\n", op, reg.c_str(), stk.c_str());
}

// free and allocate
inline void binary_manage(const char* op, const native::Str16 stk, const int size) {
  binary_imm(op, stk, size);
}

inline void system_print(const native::Str16 x) {
  binary_access("li", "$v0", "1");
  std::printf("syscall\n");
}

inline void return_value(const native::Str16 x) {
  std::printf("jr $31\n");
}


}
#endif