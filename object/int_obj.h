#ifndef INT_OBJECT
#define INT_OBJECT

#include <string>
#include "object.h"

namespace obj {

class int_object: public value_object{
public:
    bool proper;
    int cval;
public:
    // native::Str16 load_reg();
    int_object(std::string name, bool proper, int cval, int valid_space);
    int_object(const int_object& other);
    void store2reg();
    friend void cmd_add(int_object& lhs, int_object& rhs);
    friend void cmd_sub(int_object& lhs, int_object& rhs);
    friend void cmd_mul(int_object& lhs, int_object& rhs);
    friend void cmd_and(int_object& lhs, int_object& rhs);
    friend void cmd_or(int_object& lhs, int_object& rhs);
    friend void cmd_xor(int_object& lhs, int_object& rhs);
    friend void cmd_le(int_object& lhs, int_object& rhs);
    friend void cmd_slt(int_object& lhs, int_object& rhs);
    friend void cmd_sgt(int_object& lhs, int_object& rhs);
    friend void cmd_ge(int_object& lhs, int_object& rhs);
    friend void cmd_eq(int_object& lhs, int_object& rhs);
    friend void cmd_ne(int_object& lhs, int_object& rhs);
    friend void cmd_div(int_object& lhs, int_object& rhs);
    friend void cmd_mod(int_object& lhs, int_object& rhs);
    friend void cmd_assign(int_object& lhs, int_object& rhs);
};

void cmd_add(int_object& lhs, int_object& rhs);
void cmd_sub(int_object& lhs, int_object& rhs);
void cmd_mul(int_object& lhs, int_object& rhs);
void cmd_and(int_object& lhs, int_object& rhs);
void cmd_or(int_object& lhs, int_object& rhs);
void cmd_xor(int_object& lhs, int_object& rhs);
void cmd_le(int_object& lhs, int_object& rhs);
void cmd_slt(int_object& lhs, int_object& rhs);
void cmd_sgt(int_object& lhs, int_object& rhs);
void cmd_ge(int_object& lhs, int_object& rhs);
void cmd_eq(int_object& lhs, int_object& rhs);
void cmd_ne(int_object& lhs, int_object& rhs);
void cmd_div(int_object& lhs, int_object& rhs);
void cmd_mod(int_object& lhs, int_object& rhs);
void cmd_assign(int_object& lhs, int_object& rhs);

}  // namespace obj
#endif