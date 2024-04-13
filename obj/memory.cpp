#include "object.h"
#include "../mips/utils.h"

namespace memory {
std::vector<size_t> stk_size(1,128);
std::vector<int> global_tp_pos(1,0);

int register_pos::RegNum = 0;
register_pos* register_pos::regs = new register_pos[32];

stack_pos::stack_pos():sPos(-1), flag(-1){}
stack_pos::stack_pos(size_t size) {  
    if (global_tp_pos.back() + size > stk_size.back()) {
        alloc_stack(stk_size.back());
    }
    sPos = global_tp_pos.back() + size;
    global_tp_pos.back() += size;
}

native::Str16 stack_pos::get_pos(size_t size) {
    if (sPos == -1) {
        debug(global_tp_pos, stk_size.back());
        if (global_tp_pos.back() + size > stk_size.back()) {
            alloc_stack(stk_size.back());
        }
        debug("alloc stackpos");
        sPos = global_tp_pos.back() + size;
        global_tp_pos.back() += size;
    }
    debug(stk_size.back(), sPos);
    return std::to_string(stk_size.back() - sPos) + "($fp)";
}

register_pos::register_pos() {
    rPos = RegNum ++;
    this->nowUsing = nullptr;
}

native::Str16 register_pos::get_pos() {
    return "$" + std::to_string(rPos);
}

register_pos* alloc_reg(obj::value_object* yue) {
    for (int i = 8; i < 15; i ++) {
        if (register_pos::regs[i].nowUsing == nullptr) {
            register_pos::regs[i].nowUsing = yue;
            return register_pos::regs + i;
        }
    }
    return nullptr;
}

register_pos* alloc_reg(obj::value_object* yue, int x) {
    free_reg(register_pos::regs + x);
    register_pos::regs[x].nowUsing = yue;
    return register_pos::regs + x;
}

void free_reg(register_pos* reg) {
    if (reg->nowUsing != nullptr) {
        reg->nowUsing = nullptr;
    }
}

void alloc_stack(size_t size, bool is_define) {
    mips::binary_manage("addiu", "$sp", -int(size), __PRETTY_FUNCTION__);
    if (is_define) {
        stk_size.push_back(size);
        global_tp_pos.push_back(0);
        save_fp();
        move_fp();
        return;
    }
    stk_size.back() += size;
}

void free_stack() {
    load_fp();
    mips::binary_manage("addiu", "$sp", stk_size.back(), __PRETTY_FUNCTION__);
    stk_size.pop_back();
    global_tp_pos.pop_back();
}

void check_regs() {
    std::cout << register_pos::RegNum << '\n';
    for (int i = 0; i < register_pos::RegNum; i ++) {
        std::cout << ((register_pos::regs + i) -> nowUsing == nullptr) << ' ';
    }
    std::cout << std::endl;
}

void save_fp() {
    int save_pos = stk_size.back() - 4;
    global_tp_pos.back() += 4;
    mips::binary_access("sw", "$fp", std::to_string(save_pos) + "($sp)", __PRETTY_FUNCTION__);
}

void move_fp() {
    mips::binary_access("move", "$fp", "$sp", __PRETTY_FUNCTION__);
}

void load_fp() {
    int save_pos = stk_size.back() - 4;
    mips::binary_access("lw", "$fp", std::to_string(save_pos) + "($sp)", __PRETTY_FUNCTION__);
}

} // namespace memory
