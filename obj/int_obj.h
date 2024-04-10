#pragma once

#include <string>

#include "native/short_str.h"

namespace obj {

struct IntObject {
  bool proper;
  int cval;

  native::Str8 load_reg();
  void free_reg(bool save = false);
};

void cmd_add(IntObject& lhs, IntObject& rhs);
void cmd_sub(IntObject& lhs, IntObject& rhs);
void cmd_mul(IntObject& lhs, IntObject& rhs);
void cmd_and(IntObject& lhs, IntObject& rhs);
void cmd_or(IntObject& lhs, IntObject& rhs);
void cmd_xor(IntObject& lhs, IntObject& rhs);

}  // namespace obj
