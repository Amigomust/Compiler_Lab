#ifndef UTILS
#define UTILS

#include <cstdio>

#include "../native/short_str.h"


namespace mips {

inline void binary_left(const char* op, native::Str16 lhs, native::Str16 rhs, const char* from = "") {
  std::printf("%s %s, %s, %s\t\t#%s\n", op, lhs.c_str(), lhs.c_str(), rhs.c_str(), from);
}

inline void binary_right(const char* op, native::Str16 lhs, native::Str16 rhs, const char* from = "") {
  std::printf("%s %s, %s, %s\t\t#%s\n", op, rhs.c_str(), lhs.c_str(), rhs.c_str(), from);
}

inline void binary_imm(const char* op, native::Str16 lhs, const int imm, const char* from = "") {
  std::printf("%s %s, %s, %d\t\t#%s\n", op, lhs.c_str(), lhs.c_str(), imm, from);
}

inline void binary_div_left(const char* op, const char* op2, native::Str16 lhs, native::Str16 rhs, const char* from = "") {
  std::printf("%s %s, %s\t\t#%s\n", op, lhs.c_str(), rhs.c_str(), from);
  std::printf("%s %s\n", op2, lhs.c_str());
}

inline void binary_div_right(const char* op, const char* op2, native::Str16 lhs, native::Str16 rhs, const char* from = "") {
  std::printf("%s %s, %s\t\t#%s\n", op, lhs.c_str(), rhs.c_str(), from);
  std::printf("%s %s\n", op2, rhs.c_str());
}

// save and load
inline void binary_access(const char* op, const native::Str16 reg, const native::Str16 stk, const char* from = "") {
  std::printf("%s %s, %s\t\t#%s\n", op, reg.c_str(), stk.c_str(), from);
}

// free and allocate
inline void binary_manage(const char* op, const native::Str16 stk, const int size, const char* from = "") {
  binary_imm(op, stk, size, from);
}

inline void system_print(const native::Str16 x) {
  binary_access("li", "$v0", "1");
  std::printf("syscall\n");
  binary_access("li", "$v0", "4");
  binary_access("la", "$a0", "newline");
  std::printf("syscall\n");
}

inline void return_value(const native::Str16 x) {
  std::printf("jr $31\n");
}


}
#endif