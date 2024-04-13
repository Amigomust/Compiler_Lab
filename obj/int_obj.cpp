#include "int_obj.h"

#include <utility>
#include <stdexcept>

#include "../mips/utils.h"

namespace obj {

int_object::int_object(std::string name, bool proper, int val, int valid_space): value_object(name, "value", valid_space){
    this->proper = proper;
    this->cval = val;
    if (proper == 1) load_stk();
}

int_object::int_object(const int_object& other) : value_object(other), proper(other.proper), cval(other.cval) {}

void int_object::store2reg() {
    if (proper) mips::binary_access("lw", load_reg(), load_stk(), __PRETTY_FUNCTION__);
    else mips::binary_access("li", load_reg(), std::to_string(cval), __PRETTY_FUNCTION__);
}

#define DEFINE_BINARY_OP(NAME, OP)                                      \
    void cmd_##NAME(int_object& lhs, int_object& rhs) {                 \
        if (lhs.proper) {                                               \
            mips::binary_left(#NAME, lhs.load_reg(), rhs.load_reg(), __PRETTY_FUNCTION__);   \
            rhs.free_reg();                                             \
            return;                                                     \
        }                                                               \
        if (rhs.proper) {                                               \
            mips::binary_right(#NAME, lhs.load_reg(), rhs.load_reg(), __PRETTY_FUNCTION__);  \
            lhs.free_reg();                                             \
            std::swap(lhs, rhs);                                        \
            return;                                                     \
        }                                                               \
        lhs.cval = lhs.cval OP rhs.cval;                                \
    }

DEFINE_BINARY_OP(add, +)
DEFINE_BINARY_OP(sub, -)
DEFINE_BINARY_OP(mul, *)
DEFINE_BINARY_OP(and, &)
DEFINE_BINARY_OP(or, |)
DEFINE_BINARY_OP(xor, ^)
DEFINE_BINARY_OP(slt, <)

void cmd_le(int_object& lhs, int_object& rhs) {
    if (lhs.proper) {
        mips::binary_right("slt", rhs.load_reg(), lhs.load_reg(), __PRETTY_FUNCTION__);
        mips::binary_imm("xori", lhs.load_reg(), 1, __PRETTY_FUNCTION__);
        rhs.free_reg();
        return;
    }
    if (rhs.proper) {
        mips::binary_right("slt", rhs.load_reg(), lhs.load_reg(), __PRETTY_FUNCTION__);
        mips::binary_imm("xori", rhs.load_reg(), 1, __PRETTY_FUNCTION__);
        lhs.free_reg();
        std::swap(lhs, rhs);
        return;
    }
    debug(lhs.cval, rhs.cval);
    lhs.cval = lhs.cval <= rhs.cval;
    debug(lhs.cval);
}

void cmd_sgt(int_object& lhs, int_object& rhs) {
    std::swap(lhs, rhs);
    cmd_slt(lhs, rhs);
}

void cmd_ge(int_object& lhs, int_object& rhs) {
    std::swap(lhs, rhs);
    cmd_le(lhs, rhs);
}

void cmd_eq(int_object& lhs, int_object& rhs) {
    if (lhs.proper) {
        mips::binary_left("xor", lhs.load_reg(), rhs.load_reg(), __PRETTY_FUNCTION__);
        mips::binary_imm("sltiu", lhs.load_reg(), 1, __PRETTY_FUNCTION__);
        rhs.free_reg();
        return;
    }
    if (rhs.proper) {
        mips::binary_right("xor", lhs.load_reg(), rhs.load_reg(), __PRETTY_FUNCTION__);
        mips::binary_imm("sltiu", rhs.load_reg(), 1, __PRETTY_FUNCTION__);
        lhs.free_reg();
        std::swap(lhs, rhs);
        return;
    }
    lhs.cval = lhs.cval == rhs.cval;
}

void cmd_ne(int_object& lhs, int_object& rhs) {
    if (lhs.proper) {
        mips::binary_left("xor", lhs.load_reg(), rhs.load_reg(), __PRETTY_FUNCTION__);
        mips::binary_right("sltu", "$0", lhs.load_reg(), __PRETTY_FUNCTION__);
        rhs.free_reg();
        return;
    }
    if (rhs.proper) {
        mips::binary_right("xor", lhs.load_reg(), rhs.load_reg(), __PRETTY_FUNCTION__);
        mips::binary_right("sltu", "$0", rhs.load_reg(), __PRETTY_FUNCTION__);
        lhs.free_reg();
        std::swap(lhs, rhs);
        return;
    }
    lhs.cval = lhs.cval != rhs.cval;
}

void cmd_div(int_object& lhs, int_object& rhs) {
    if (lhs.proper) {
        mips::binary_div_left("div", "mflo", lhs.load_reg(), rhs.load_reg(), __PRETTY_FUNCTION__);
        rhs.free_reg();
        return;
    }
    if (rhs.proper) {
        mips::binary_div_right("div", "mflo", lhs.load_reg(), rhs.load_reg(), __PRETTY_FUNCTION__);
        lhs.free_reg();
        std::swap(lhs, rhs);
        return;
    }
    if (rhs.cval == 0) {
        throw std::runtime_error("Division by zero");
    }
    lhs.cval = lhs.cval / rhs.cval;
}
void cmd_mod(int_object& lhs, int_object& rhs) {
    if (lhs.proper) {
        mips::binary_div_left("div", "mfhi", lhs.load_reg(), rhs.load_reg(), __PRETTY_FUNCTION__);
        rhs.free_reg();
        return;
    }
    if (rhs.proper) {
        mips::binary_div_right("div", "mfhi", lhs.load_reg(), rhs.load_reg(), __PRETTY_FUNCTION__);
        lhs.free_reg();
        std::swap(lhs, rhs);
        return;
    }
    if (rhs.cval == 0) {
        throw std::runtime_error("Division by zero");
    }
    lhs.cval = lhs.cval % rhs.cval;
}
void cmd_assign(int_object& lhs, int_object& rhs) {
    if (!lhs.proper) {
        throw std::runtime_error("Cannot assign to a constant");
    }
    debug("fuck2222");
    mips::binary_access("sw", rhs.load_reg(), lhs.load_stk(), __PRETTY_FUNCTION__);
    debug("fuck4444");
    rhs.free_reg();
    debug("fuck3333");
}

}  // namespace obj
