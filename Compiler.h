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
    virtual void regSaveToStack();
    virtual void work();
    virtual void stackToReg();
    virtual void freeRegNoSave();
    friend class RegPos;
};

class IntObject:public Object {
    public:
    IntObject(std::string name, size_t valid);
    void regSaveToStack();
    void freeRegNoSave();
    void stackToReg();
};

class ImmObject:public Object {
    protected:
    int value;
    public:
    ImmObject(int val);
    int getVal();
};

class FunctionObject:public Object {
    protected:
    std::vector<Object*> args;
    public:
    FunctionObject(std::string name, size_t valid, std::vector<Object*> args);
    void work();
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

#endif