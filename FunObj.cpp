#include "Compiler.h"


FunctionObject::FunctionObject(std::string name, size_t valid, std::vector<Object*> args) : Object(0, name, valid) {
    type = DataType("function");
    this->args = args;
}

template<typename T>
T FunctionObject::work() {
    // TODO: jump指令
    return T()
}