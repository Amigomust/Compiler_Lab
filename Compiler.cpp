#include "Compiler.h"

int StackPos::GlobalStackPos = 0;
int RegPos::RegNum = 0;

StackPos::StackPos(size_t size) {
    sPos = GlobalStackPos;
    GlobalStackPos += size;
    // TODO: mips
    flag = 0;
}

StackPos::StackPos():sPos(-1), flag(-1){}

RegPos::RegPos(Object* nowUsing = nullptr) {
    rPos = RegNum ++;
    this->nowUsing = nowUsing;
}

RegPos regs[32];
