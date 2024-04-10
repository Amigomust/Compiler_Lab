#include "int_obj.h"

#include <utility>

#include "mips/utils.h"

namespace obj {

#define DEFINE_BINARY_OP(NAME, OP)                               \
  void cmd_##NAME(IntObject& lhs, IntObject& rhs) {              \
    if (lhs.proper) {                                            \
      mips::binary_left(#NAME, lhs.load_reg(), rhs.load_reg());  \
      rhs.free_reg();                                            \
      return;                                                    \
    }                                                            \
    if (rhs.proper) {                                            \
      mips::binary_right(#NAME, lhs.load_reg(), rhs.load_reg()); \
      lhs.free_reg();                                            \
      std::swap(lhs, rhs);                                       \
      return;                                                    \
    }                                                            \
    lhs.cval = lhs.cval OP rhs.cval;                             \
  }

DEFINE_BINARY_OP(add, +)
DEFINE_BINARY_OP(sub, +)
DEFINE_BINARY_OP(mul, +)
DEFINE_BINARY_OP(and, +)
DEFINE_BINARY_OP(or, +)
DEFINE_BINARY_OP(xor, +)

}  // namespace obj
