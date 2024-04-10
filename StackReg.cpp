#include "Compiler.h"

int StackPos::GlobalStackPos = 0;
int RegPos::RegNum = 0;
RegPos* RegPos::regs = new RegPos[32];

StackPos::StackPos(size_t size) {
    sPos = GlobalStackPos;
    GlobalStackPos += size;
    // TODO: mips
    flag = 0;
}

StackPos::StackPos():sPos(-1), flag(-1){}

RegPos::RegPos(Object* nowUsing) {
    rPos = RegNum ++;
    this->nowUsing = nowUsing;
}

void RegPos::checkTheRegs() {
    std::cout << RegNum << '\n';
    for (int i = 0; i < RegNum; i ++) {
        std::cout << (regs + i) -> rPos << ' ';

        assert((regs + i) -> nowUsing == nullptr);
    }
    std::cout << std::endl;
}

RegPos* RegPos::allocReg(Object* yue) {
    for (int i = 8; i < 15; i ++) {
        if (regs[i].nowUsing == nullptr) {
            regs[i].nowUsing = yue;
            return regs + i;
        }
    }
    assert(false); // no more reg
}

void RegPos::freeReg(RegPos* reg) {
    if (reg->nowUsing) {
        reg->nowUsing = nullptr;
    }
}

std::string RegPos::getRegPos() {
    return "$" + std::to_string(rPos);
}