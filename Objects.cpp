#include "Compiler.h"

DataType::DataType(std::string name = "NULL"): typeName(name) {};

Object::Object() {}

Object::Object(size_t size, std::string name, size_t valid) {
    obSize = size;
    sPos = StackPos(size);
    this->name = name;
    validSpace = valid;
}

void Object::freeReg(int save) {
    if (rPos) {
        std::string stkPos = sPos.getStackPos();
        std::string regpos = rPos->getRegPos();
        if (save) {}// TODO:
        else {
            RegPos::freeReg(rPos);
            rPos = nullptr;
        }
    }
}
std::string Object::toReg() {}



