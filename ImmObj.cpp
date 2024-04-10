#include "Compiler.h"

ImmObject::ImmObject(int val): value(val) {}

std::string ImmObject::toReg() {
    std::string stkPos = sPos.getStackPos();
    if (rPos == nullptr) rPos = RegPos::allocReg(this);
    std::string regPos = rPos->getRegPos();
    // TODO: 立即数存入Reg
    
    return regPos;
}

Object* ImmObject::add(Object* obj) {
    IntObject* intObj = dynamic_cast<IntObject*>(obj);
    if (intObj) {
        return intObj->add(this);
    }
    ImmObject* immObj = dynamic_cast<ImmObject*>(obj);
    if (immObj) {
        return nullptr;
    }
}

Object* ImmObject::sub(Object* obj) {
    IntObject* intObj = dynamic_cast<IntObject*>(obj);
    if (intObj) {
        return nullptr;
    }
    ImmObject* immObj = dynamic_cast<ImmObject*>(obj);
    if (immObj) {
        return nullptr;
    }
}

Object* ImmObject::div(Object* obj) {
    IntObject* intObj = dynamic_cast<IntObject*>(obj);
    if (intObj) {
        return nullptr;
    }
    ImmObject* immObj = dynamic_cast<ImmObject*>(obj);
    if (immObj) {
        return nullptr;
    }
}

Object* ImmObject::mul(Object* obj) {
    IntObject* intObj = dynamic_cast<IntObject*>(obj);
    if (intObj) {
        return intObj->mul(this);
    }
    ImmObject* immObj = dynamic_cast<ImmObject*>(obj);
    if (immObj) {
        return nullptr;
    }
}

Object* ImmObject::mod(Object* obj) {
    IntObject* intObj = dynamic_cast<IntObject*>(obj);
    if (intObj) {
        return nullptr;
    }
    ImmObject* immObj = dynamic_cast<ImmObject*>(obj);
    if (immObj) {
        return nullptr;
    }
}

Object* ImmObject::and_(Object* obj) {
    IntObject* intObj = dynamic_cast<IntObject*>(obj);
    if (intObj) {
        return intObj->and_(this);
    }
    ImmObject* immObj = dynamic_cast<ImmObject*>(obj);
    if (immObj) {
        return nullptr;
    }
}

Object* ImmObject::or_(Object* obj) {
    IntObject* intObj = dynamic_cast<IntObject*>(obj);
    if (intObj) {
        return intObj->or_(this);
    }
    ImmObject* immObj = dynamic_cast<ImmObject*>(obj);
    if (immObj) {
        return nullptr;
    }
}

Object* ImmObject::xor_(Object* obj) {
    IntObject* intObj = dynamic_cast<IntObject*>(obj);
    if (intObj) {
        return intObj->xor_(this);
    }
    ImmObject* immObj = dynamic_cast<ImmObject*>(obj);
    if (immObj) {
        return nullptr;
    }
}

Object* ImmObject::lessEqual(Object* obj) {
    IntObject* intObj = dynamic_cast<IntObject*>(obj);
    if (intObj) {
        return intObj->greaterThan(this);
    }
    ImmObject* immObj = dynamic_cast<ImmObject*>(obj);
    if (immObj) {
        return nullptr;
    }
}
Object* ImmObject::lessThan(Object* obj) {
    IntObject* intObj = dynamic_cast<IntObject*>(obj);
    if (intObj) {
        return intObj->greaterEqual(this);
    }
    ImmObject* immObj = dynamic_cast<ImmObject*>(obj);
    if (immObj) {
        return nullptr;
    }
}
Object* ImmObject::greaterEqual(Object* obj) {
    IntObject* intObj = dynamic_cast<IntObject*>(obj);
    if (intObj) {
        return intObj->lessThan(this);
    }
    ImmObject* immObj = dynamic_cast<ImmObject*>(obj);
    if (immObj) {
        return nullptr;
    }
}
Object* ImmObject::greaterThan(Object* obj) {
    IntObject* intObj = dynamic_cast<IntObject*>(obj);
    if (intObj) {
        return intObj->lessEqual(this);
    }
    ImmObject* immObj = dynamic_cast<ImmObject*>(obj);
    if (immObj) {
        return nullptr;
    }
}
Object* ImmObject::notEqual(Object* obj) {
    IntObject* intObj = dynamic_cast<IntObject*>(obj);
    if (intObj) {
        return intObj->notEqual(this);
    }
    ImmObject* immObj = dynamic_cast<ImmObject*>(obj);
    if (immObj) {

        return nullptr;
    }
}

// 赋值语句
Object* ImmObject::equal(Object* obj) {
    assert(false);
    IntObject* intObj = dynamic_cast<IntObject*>(obj);
    if (intObj) {
        return nullptr;
    }
    ImmObject* immObj = dynamic_cast<ImmObject*>(obj);
    if (immObj) {
        return nullptr;
    }
}