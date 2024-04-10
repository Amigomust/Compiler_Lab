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
        
        intObj->freeReg(0);
        return this;
    }
    ImmObject* immObj = dynamic_cast<ImmObject*>(obj);
    if (immObj) {
        return nullptr;
    }
}

Object* IntObject::sub(Object* obj) {
    IntObject* intObj = dynamic_cast<IntObject*>(obj);
    if (intObj) {
        return nullptr;
    }
    ImmObject* immObj = dynamic_cast<ImmObject*>(obj);
    if (immObj) {
        return nullptr;
    }
}

Object* IntObject::div(Object* obj) {
    IntObject* intObj = dynamic_cast<IntObject*>(obj);
    if (intObj) {
        return nullptr;
    }
    ImmObject* immObj = dynamic_cast<ImmObject*>(obj);
    if (immObj) {
        return nullptr;
    }
}

Object* IntObject::mul(Object* obj) {
    IntObject* intObj = dynamic_cast<IntObject*>(obj);
    if (intObj) {
        return nullptr;
    }
    ImmObject* immObj = dynamic_cast<ImmObject*>(obj);
    if (immObj) {
        return nullptr;
    }
}

Object* IntObject::mod(Object* obj) {
    IntObject* intObj = dynamic_cast<IntObject*>(obj);
    if (intObj) {
        return nullptr;
    }
    ImmObject* immObj = dynamic_cast<ImmObject*>(obj);
    if (immObj) {
        return nullptr;
    }
}

Object* IntObject::and_(Object* obj) {
    IntObject* intObj = dynamic_cast<IntObject*>(obj);
    if (intObj) {
        return nullptr;
    }
    ImmObject* immObj = dynamic_cast<ImmObject*>(obj);
    if (immObj) {
        return nullptr;
    }
}

Object* IntObject::or_(Object* obj) {
    IntObject* intObj = dynamic_cast<IntObject*>(obj);
    if (intObj) {
        return nullptr;
    }
    ImmObject* immObj = dynamic_cast<ImmObject*>(obj);
    if (immObj) {
        return nullptr;
    }
}

Object* IntObject::xor_(Object* obj) {
    IntObject* intObj = dynamic_cast<IntObject*>(obj);
    if (intObj) {
        return nullptr;
    }
    ImmObject* immObj = dynamic_cast<ImmObject*>(obj);
    if (immObj) {
        return nullptr;
    }
}

Object* IntObject::lessEqual(Object* obj) {
    IntObject* intObj = dynamic_cast<IntObject*>(obj);
    if (intObj) {
        return nullptr;
    }
    ImmObject* immObj = dynamic_cast<ImmObject*>(obj);
    if (immObj) {
        return nullptr;
    }
}
Object* IntObject::lessThan(Object* obj) {
    IntObject* intObj = dynamic_cast<IntObject*>(obj);
    if (intObj) {
        return nullptr;
    }
    ImmObject* immObj = dynamic_cast<ImmObject*>(obj);
    if (immObj) {
        return nullptr;
    }
}
Object* IntObject::greaterEqual(Object* obj) {
    IntObject* intObj = dynamic_cast<IntObject*>(obj);
    if (intObj) {
        return nullptr;
    }
    ImmObject* immObj = dynamic_cast<ImmObject*>(obj);
    if (immObj) {
        return nullptr;
    }
}
Object* IntObject::greaterThan(Object* obj) {
    IntObject* intObj = dynamic_cast<IntObject*>(obj);
    if (intObj) {
        return nullptr;
    }
    ImmObject* immObj = dynamic_cast<ImmObject*>(obj);
    if (immObj) {
        return nullptr;
    }
}
Object* IntObject::notEqual(Object* obj) {
    IntObject* intObj = dynamic_cast<IntObject*>(obj);
    if (intObj) {
        return nullptr;
    }
    ImmObject* immObj = dynamic_cast<ImmObject*>(obj);
    if (immObj) {
        return nullptr;
    }
}
Object* IntObject::equal(Object* obj) {
    IntObject* intObj = dynamic_cast<IntObject*>(obj);
    if (intObj) {
        return nullptr;
    }
    ImmObject* immObj = dynamic_cast<ImmObject*>(obj);
    if (immObj) {
        return nullptr;
    }
}