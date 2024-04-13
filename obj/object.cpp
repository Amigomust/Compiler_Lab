#include "object.h"

namespace obj {

object::object() {
    this->name = "";
    this->type = "";
    this->valid_space = 0;
}

object::object(std::string name, std::string type, int valid_space) {
    this->name = name;
    this->type = type;
    this->valid_space = valid_space;
}

object::object(const object& other) : name(other.name), type(other.type), valid_space(other.valid_space) {}

value_object::value_object(std::string name, std::string type, int valid_space) {
    this->name = name;
    this->type = type;
    this->valid_space = valid_space;
    reg_pos = nullptr;
}

value_object::value_object() {
    this->name = "";
    this->type = "value";
    this->valid_space = 0;
    reg_pos = nullptr;
}

value_object::value_object(const value_object& other) : object(other.name, other.type, other.valid_space), stk_pos(other.stk_pos), reg_pos(nullptr) {
    if (other.reg_pos != nullptr) {
    reg_pos = memory::alloc_reg(this);
    reg_pos->nowUsing = this;
    reg_pos->get_pos() = other.reg_pos->get_pos();
    }
}

native::Str16 value_object::load_reg() {
    if (reg_pos == nullptr) {
        reg_pos = memory::alloc_reg(this);
        assert(reg_pos != nullptr);
        store2reg();
    }
    debug("fuck5555");
    
    return reg_pos->get_pos();
}

native::Str16 value_object::load_reg(int x) {
    if (reg_pos == nullptr) {
        reg_pos = memory::alloc_reg(this, x);
        assert(reg_pos != nullptr);
        store2reg();
    }
    
    return reg_pos->get_pos();
}

void value_object::store2reg() {
    debug("fuck!");
}

void value_object::free_reg() {
    assert(reg_pos);
    if (reg_pos) {
        memory::free_reg(reg_pos);
        reg_pos = nullptr;
    }
}

native::Str16 value_object::load_stk() {
    debug("fuck111");
    auto temp = stk_pos.get_pos(4);
    debug(temp);
    return temp;
}

}