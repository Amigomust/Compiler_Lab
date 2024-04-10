#ifndef COMPILER
#define COMPILER
#include <iostream>
#include <stack>
#include <map>
#include <vector>
#include <assert.h>

class DataType {
    private:
    std::string typeName;
    public:
    DataType(std::string name);
};

class StackPos {
    private:
    static int GlobalStackPos;
    int sPos; // StackPos
    int flag; // flag = 1: Arg flag = 0: Common
    public:
    StackPos();
    StackPos(size_t size);
    std::string getStackPos();
};

class Object;

class RegPos {
    private:
    static int RegNum;
    static RegPos* regs;
    int rPos;
    Object* nowUsing;
    public:
    RegPos(Object* nowUsing = nullptr);
    std::string getRegPos();
    static RegPos* allocReg(Object* yue);
    static void freeReg(RegPos* reg);
    static void checkTheRegs();
};

class Object {
protected:
    StackPos sPos;
    RegPos* rPos;
    size_t obSize;
public:
    std::string name;
    DataType type;
    size_t validSpace;
    Object();
    Object(size_t size, std::string name, size_t validSpace);
    virtual std::string toReg();
    virtual void freeReg(int save);
    friend class RegPos;
    virtual Object* add(Object* obj) = 0;
    virtual Object* sub(Object* obj) = 0;
    virtual Object* mul(Object* obj) = 0;
    virtual Object* div(Object* obj) = 0;
    virtual Object* mod(Object* obj) = 0;
    virtual Object* and_(Object* obj) = 0;
    virtual Object* or_(Object* obj) = 0;
    virtual Object* xor_(Object* obj) = 0;
    virtual Object* lessEqual(Object* obj) = 0;
    virtual Object* lessThan(Object* obj) = 0;
    virtual Object* greaterEqual(Object* obj) = 0;
    virtual Object* greaterThan(Object* obj) = 0;
    virtual Object* notEqual(Object* obj) = 0;
    virtual Object* equal(Object* obj) = 0;
};

class IntObject:public Object {
    public:
    IntObject(std::string name, size_t valid);
    Object* add(Object* obj);
    Object* sub(Object* obj);
    Object* mul(Object* obj);
    Object* div(Object* obj);
    Object* mod(Object* obj);
    Object* and_(Object* obj);
    Object* or_(Object* obj);
    Object* xor_(Object* obj);
    Object* lessEqual(Object* obj);
    Object* lessThan(Object* obj);
    Object* greaterEqual(Object* obj);
    Object* greaterThan(Object* obj);
    Object* notEqual(Object* obj);
    Object* equal(Object* obj);
    
    std::string toReg();
};

class ImmObject:public Object {
    protected:
    int value;
    public:
    ImmObject(int val);
    std::string toReg();
    // int getVal();
    Object* add(Object* obj);
    Object* sub(Object* obj);
    Object* mul(Object* obj);
    Object* div(Object* obj);
    Object* mod(Object* obj);
    Object* and_(Object* obj);
    Object* or_(Object* obj);
    Object* xor_(Object* obj);
    Object* lessEqual(Object* obj);
    Object* lessThan(Object* obj);
    Object* greaterEqual(Object* obj);
    Object* greaterThan(Object* obj);
    Object* notEqual(Object* obj);
    Object* equal(Object* obj);
};
class FunctionObject:public Object {
    protected:
    std::vector<Object*> args;
    public:
    FunctionObject(std::string name, size_t valid, std::vector<Object*> args);
    template<typename T>
    T work();
};

class Trie {
    private:
    struct node {
        // std::unordered_map<char, int> next;
        std::stack<Object*> objects;
        std::map<char, int> next;
    };
    std::vector<node> nodes;
    public:
    void insert(Object* object);
    Object* find(std::string name);
};

void compileTheExpression(std::string s, int valid);
void compileTheCode(std::string s, int valid);
std::string regular(std::string code);

#endif