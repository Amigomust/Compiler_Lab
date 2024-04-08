#include "Compiler.h"

DataType::DataType(std::string name = "NULL"): typeName(name) {};

Object::Object() {}

Object::Object(size_t size, std::string name, size_t valid) {
    obSize = size;
    sPos = StackPos(size);
    this->name = name;
    validSpace = valid;
}

IntObject::IntObject(std::string name, size_t valid) : Object(4, name, valid) {
    type = DataType("int");
}

void IntObject::regSaveToStack() {
    std::string regPos = rPos -> getRegPos();
    std::string stkPos = sPos.getStackPos();
    // TODO: Complete it
}

ImmObject::ImmObject(int val): value(val) {}

FunctionObject::FunctionObject(std::string name, size_t valid, std::vector<Object*> args) : Object(0, name, valid) {
    type = DataType("function");
    this->args = args;
}

void FunctionObject::work() {
    // TODO: jump指令
}