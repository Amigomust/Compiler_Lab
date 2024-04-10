#include "Compiler.h"

IntObject::IntObject(std::string name, size_t valid) : Object(4, name, valid) {
    type = DataType("int");
}

std::string IntObject::toReg() {
    std::string stkPos = sPos.getStackPos();
    if (rPos == nullptr) rPos = RegPos::allocReg(this);
    std::string regPos = rPos->getRegPos();
    // TODO: 栈 -> 寄存器

    return regPos;
}


Object* IntObject::add(Object* obj) {
    IntObject* intObj = dynamic_cast<IntObject*>(obj);
    std::string aPos = toReg();
    if (intObj) {
        std::string bPos = intObj->toReg();
        printf("add %s, %s, %s\n", aPos.c_str(), aPos.c_str(), bPos.c_str());
        intObj->freeReg(0);
        return this;
    }
    ImmObject* immObj = dynamic_cast<ImmObject*>(obj);
    if (immObj) {
        std::string bPos = immObj->toReg();
        printf("add %s, %s, %s\n", aPos.c_str(), aPos.c_str(), bPos.c_str());
        immObj->freeReg(0);
        return this;
    }
}

Object* IntObject::sub(Object* obj) {
    IntObject* intObj = dynamic_cast<IntObject*>(obj);
    std::string aPos = toReg();
     if (intObj) {
        std::string bPos = intObj->toReg();
        printf("sub %s, %s, %s\n", aPos.c_str(), aPos.c_str(), bPos.c_str());
        intObj->freeReg(0);
        return this;
    }
    ImmObject* immObj = dynamic_cast<ImmObject*>(obj);
    if (immObj) {
        std::string bPos = immObj->toReg();
        printf("sub %s, %s, %s\n", aPos.c_str(), aPos.c_str(), bPos.c_str());
        immObj->freeReg(0);
        return this;
    }
}

Object* IntObject::div(Object* obj) {
    IntObject* intObj = dynamic_cast<IntObject*>(obj);
    std::string aPos = toReg();
     if (intObj) {
        std::string bPos = intObj->toReg();
        printf("div %s, %s\n", aPos.c_str(), bPos.c_str());
        printf("mflo %s\n", aPos.c_str());
        intObj->freeReg(0);
        return this;
    }
    ImmObject* immObj = dynamic_cast<ImmObject*>(obj);
    if (immObj) {
        std::string bPos = immObj->toReg();
        printf("div %s, %s\n", aPos.c_str(), bPos.c_str());
        printf("mflo %s\n", aPos.c_str());
        immObj->freeReg(0);
        return this;
    }
}

Object* IntObject::mul(Object* obj) {
    IntObject* intObj = dynamic_cast<IntObject*>(obj);
    std::string aPos = toReg();
     if (intObj) {
        std::string bPos = intObj->toReg();
        printf("mul %s, %s, %s\n", aPos.c_str(), aPos.c_str(), bPos.c_str());
        intObj->freeReg(0);
        return this;
    }
    ImmObject* immObj = dynamic_cast<ImmObject*>(obj);
    if (immObj) {
        std::string bPos = immObj->toReg();
        printf("mul %s, %s, %s\n", aPos.c_str(), aPos.c_str(), bPos.c_str());
        immObj->freeReg(0);
        return this;
    }
}

Object* IntObject::mod(Object* obj) {
    IntObject* intObj = dynamic_cast<IntObject*>(obj);
    std::string aPos = toReg();
     if (intObj) {
        std::string bPos = intObj->toReg();
        printf("div %s, %s\n", aPos.c_str(), bPos.c_str());
        printf("mfhi %s\n", aPos.c_str());
        intObj->freeReg(0);
        return this;
    }
    ImmObject* immObj = dynamic_cast<ImmObject*>(obj);
    if (immObj) {
        std::string bPos = immObj->toReg();
        printf("div %s, %s\n", aPos.c_str(), bPos.c_str());
        printf("mfhi %s\n", aPos.c_str());
        immObj->freeReg(0);
        return this;
    }
}

Object* IntObject::and_(Object* obj) {
    IntObject* intObj = dynamic_cast<IntObject*>(obj);
    std::string aPos = toReg();
    if (intObj) {
        std::string bPos = intObj->toReg();
        printf("and %s, %s, %s\n", aPos.c_str(), aPos.c_str(), bPos.c_str());
        intObj->freeReg(0);
        return this;
    }
    ImmObject* immObj = dynamic_cast<ImmObject*>(obj);
    if (immObj) {
        std::string bPos = immObj->toReg();
        printf("and %s, %s, %s\n", aPos.c_str(), aPos.c_str(), bPos.c_str());
        immObj->freeReg(0);
        return this;
    }
}

Object* IntObject::or_(Object* obj) {
    IntObject* intObj = dynamic_cast<IntObject*>(obj);
    std::string aPos = toReg();
    if (intObj) {
        std::string bPos = intObj->toReg();
        printf("or %s, %s, %s\n", aPos.c_str(), aPos.c_str(), bPos.c_str());
        intObj->freeReg(0);
        return this;
    }
    ImmObject* immObj = dynamic_cast<ImmObject*>(obj);
    if (immObj) {
        std::string bPos = immObj->toReg();
        printf("or %s, %s, %s\n", aPos.c_str(), aPos.c_str(), bPos.c_str());
        immObj->freeReg(0);
        return this;
    }
}

Object* IntObject::xor_(Object* obj) {
    IntObject* intObj = dynamic_cast<IntObject*>(obj);
    std::string aPos = toReg();
    if (intObj) {
        std::string bPos = intObj->toReg();
        printf("xor %s, %s, %s\n", aPos.c_str(), aPos.c_str(), bPos.c_str());
        intObj->freeReg(0);
        return this;
    }
    ImmObject* immObj = dynamic_cast<ImmObject*>(obj);
    if (immObj) {
        std::string bPos = immObj->toReg();
        printf("xor %s, %s, %s\n", aPos.c_str(), aPos.c_str(), bPos.c_str());
        immObj->freeReg(0);
        return this;
    }
}

Object* IntObject::lessEqual(Object* obj) {
    IntObject* intObj = dynamic_cast<IntObject*>(obj);
    std::string aPos = toReg();
    if (intObj) {
        std::string bPos = intObj->toReg();
        printf("slt %s, %s, %s\n", aPos.c_str(), bPos.c_str(), aPos.c_str());
        printf("xori %s, %s, 1\n", aPos.c_str());
        intObj->freeReg(0);
        return this;
    }
    ImmObject* immObj = dynamic_cast<ImmObject*>(obj);
    if (immObj) {
        std::string bPos = immObj->toReg();
        printf("slt %s, %s, %s\n", aPos.c_str(), bPos.c_str(), aPos.c_str());
        printf("xori %s, %s, 1\n", aPos.c_str());
        immObj->freeReg(0);
        return this;
    }
}
Object* IntObject::lessThan(Object* obj) {
    IntObject* intObj = dynamic_cast<IntObject*>(obj);
    std::string aPos = toReg();
    if (intObj) {
        std::string bPos = intObj->toReg();
        printf("slt %s, %s, %s\n", aPos.c_str(), aPos.c_str(), bPos.c_str());
        intObj->freeReg(0);
        return this;
    }
    ImmObject* immObj = dynamic_cast<ImmObject*>(obj);
    if (immObj) {
        std::string bPos = immObj->toReg();
        printf("slt %s, %s, %s\n", aPos.c_str(), aPos.c_str(), bPos.c_str());
        immObj->freeReg(0);
        return this;
    }
}
Object* IntObject::greaterEqual(Object* obj) {
    IntObject* intObj = dynamic_cast<IntObject*>(obj);
    std::string aPos = toReg();
    if (intObj) {
        std::string bPos = intObj->toReg();
        printf("slt %s, %s, %s\n", aPos.c_str(), aPos.c_str(), bPos.c_str());
        printf("xori %s, %s, 1\n", aPos.c_str());
        intObj->freeReg(0);
        return this;
    }
    ImmObject* immObj = dynamic_cast<ImmObject*>(obj);
    if (immObj) {
        std::string bPos = immObj->toReg();
        printf("slt %s, %s, %s\n", aPos.c_str(), aPos.c_str(), bPos.c_str());
        printf("xori %s, %s, 1\n", aPos.c_str());
        immObj->freeReg(0);
        return this;
    }
}
Object* IntObject::greaterThan(Object* obj) {
    IntObject* intObj = dynamic_cast<IntObject*>(obj);
    std::string aPos = toReg();
    if (intObj) {
        std::string bPos = intObj->toReg();
        printf("slt %s, %s, %s\n", aPos.c_str(), bPos.c_str(), aPos.c_str());
        intObj->freeReg(0);
        return this;
    }
    ImmObject* immObj = dynamic_cast<ImmObject*>(obj);
    if (immObj) {
        std::string bPos = immObj->toReg();
        printf("slt %s, %s, %s\n", aPos.c_str(), bPos.c_str(), aPos.c_str());
        immObj->freeReg(0);
        return this;
    }
}
Object* IntObject::notEqual(Object* obj) {
    IntObject* intObj = dynamic_cast<IntObject*>(obj);
    std::string aPos = toReg();
    if (intObj) {
        std::string bPos = intObj->toReg();
        printf("beq %s, %s, 2\n", aPos.c_str(), bPos.c_str());
        printf("li %s, 0\n", aPos.c_str());
        printf("addi %s, 1\n", aPos.c_str());
        printf("li %s, 0\n", aPos.c_str());
        intObj->freeReg(0);
        return this;
    }
    ImmObject* immObj = dynamic_cast<ImmObject*>(obj);
    if (immObj) {
        std::string bPos = immObj->toReg();
        printf("beq %s, %s, 2\n", aPos.c_str(), bPos.c_str());
        printf("li %s, 0\n", aPos.c_str());
        printf("addi %s, 1\n", aPos.c_str());
        printf("li %s, 0\n", aPos.c_str());
        immObj->freeReg(0);
        return this;
    }
}
Object* IntObject::equal(Object* obj) {
    IntObject* intObj = dynamic_cast<IntObject*>(obj);
    std::string aPos = toReg();
    if (intObj) {
        std::string bPos = intObj->toReg();
        printf("bne %s, %s, 2\n", aPos.c_str(), bPos.c_str());
        printf("li %s, 0\n", aPos.c_str());
        printf("addi %s, 1\n", aPos.c_str());
        printf("li %s, 0\n", aPos.c_str());
        intObj->freeReg(0);
        return this;
    }
    ImmObject* immObj = dynamic_cast<ImmObject*>(obj);
    if (immObj) {
        std::string bPos = immObj->toReg();
        printf("bne %s, %s, 2\n", aPos.c_str(), bPos.c_str());
        printf("li %s, 0\n", aPos.c_str());
        printf("addi %s, 1\n", aPos.c_str());
        printf("li %s, 0\n", aPos.c_str());
        immObj->freeReg(0);
        return this;
    }
}