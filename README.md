<center>北京师范大学</center>
<center>2023-2024学年春季学期  “编译原理”实验报告</center>
<center><b><font size=6 bold=true>实验二    第二代编译器</font><b></center>

## 一、实验要求

​	二代编译器将 C 语言顺序语句序列翻译为等价的汇编程序，所输出的汇编程序符合 MIPS 汇编语言格式要求。二代编译器能够处理的文法如下：

1. **关键字**：int, return, main
2. **标识符**：符合 C 语言要求的标识符
3. **常量**：十进制整型，如 1、223、10 等
4. **操作符**：=、+、-、*、/、%、<、<=、>、>=、==、!=、&（按位与）、|（按位或）、^（按位异或）
5. **分隔符**： ；、{、}
6. **语句**：变量声明（单变量且无初始化）、表达式语句、赋值语句、return、main 函数定义（不带参数 + 带参数）

​	

## 二、报告正文

### 1. 代码设计

​	我们重构过一次代码。我先来谈谈重构前的内容。

​	这是因为在本次实验的初期，我们试图写出一个能连续交很多次作业的代码（或者不用改太多的代码）出来。为此，我们定义了很多类和很多对象。由于 C 语言的语句包括赋值、变量定义、函数定义等，而其转到汇编语言之后又涉及到寄存器的分配，所以我们构想了许许多多的类，希望能通过**类定义**来完成各种行为。

​	在重构之前，我们的代码包括以下文件：`Compile.h`, `C_Compiler.cpp` ,`FunObj.cpp`, `ImmObj.cpp`, `IntObj.cpp`, `Objects.cpp`, `StackReg.cpp`, `Trie.cpp`。`Compile.h` 文件是一系列的基础，定义了全部的类和函数。类包括数据类型类 `DataType`、寄存器地址类 `ResPos`、对象类 `Object` 及其子类 `ImmObject`, `IntObject`, `FunctionObject`等。我们来聊聊 `Object` 类。 

~~~
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
};
~~~

​	该类会被许多描述对象的类所继承。

​	在 `FunObj` 中，我们试图实现函数类的定义和跳转 `j` 指令；在 `ImmObj.cpp` 中，我们完成了立即数类的一系列函数；在 `IntObj.cpp` 中，定义了整数类的一系列函数，以此类推。以 `ImmObj.cpp` 为例：

~~~
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

...
~~~

​	在该类中，我们定义了有关立即数的一系列操作，如加、减、乘、除、左右移、寄存器存入等。我们确实实现了这些东西。

​	在写到一半的时候，我们发现，上述的内容只会将整个项目引向屎山。*// to do：为什么会变成屎山？子类→friend类*

​	于是我们开始了艰苦的重构生涯。**其中首当其冲的就是这些子类**。



### 2. /native

​	/native 文件夹包括文件 `short_str.h` 和 `trie.h`。`short_str.h` 定义了字符串形式 `Str16` ；`trie.h` 定义了一个名为 `trie` 的类，用于存储、查找满足 `Str16` 定义的字符串。

​	您可以看到 `short_str.h` 有一大段不和谐的、被注释掉的代码；另外 `Str16` 也仅仅是 `using Str16 = std::string;`，这看起来很逆天，但是我们可以解释：我们**重构**过一次代码。在重构之前，我们试图自己写许许多多的类出来，其中就包括字符串类 `Str16` ，在写的时候自行定义一些运算，如字符串的“+、-、*、/”之类。然而后来我们发现这样写代码只会成为一坨大屎山。我们就开始重构了。然而，在重构的过程中，我们发现改类名很麻烦，于是就使用了一个 `using` 语句，把 `string` 类型的名字改成我们之前定义的字符串的类名。

​	我们重点来解释一下 `trie.h` 的代码。该类实现了一个简单的字典树（Trie）数据结构，用于存储和查找字符串对象。其名称来自于“检索”（Retrieve）这个词的发音。Trie 的主要特点是将字符串按照字符序列组织成树形结构，其中每个结点代表一个字符，从根结点到叶子结点路径上的字符序列组成了一个完整的字符串。

~~~
#ifndef TRIE
#define TRIE

#include "../obj/object.h"
#include <stack>
#include <unordered_map>
#include <assert.h>

namespace native {

class trie {
    private:
    struct node {
        // std::unordered_map<char, int> next;
        std::stack<obj::object*> objects;
        std::unordered_map<char, int> next;
    };
    std::vector<node> nodes;
    public:
    trie() {
        nodes.push_back(node());
    }
    void insert(obj::object* object) {
        std::string name = object->name;
        int beg = 0;
        int len = name.size();
        for (int i = 0; i < len; i ++) {
            std::unordered_map<char, int>::iterator it = nodes[beg].next.find(name[i]);
            if (it == nodes[beg].next.end()) nodes[beg].next[name[i]] = nodes.size(), nodes.push_back(node());
            beg = nodes[beg].next[name[i]];
        }
        if (nodes[beg].objects.empty() || nodes[beg].objects.top()->valid_space < object->valid_space) {
            nodes[beg].objects.push(object);
            return;
        }
        assert(false);
    }
    obj::object* find(std::string name) {
        int len = name.size();
        int beg = 0;
        for (int i = 0; i < len; i ++) {
            std::unordered_map<char, int>::iterator it = nodes[beg].next.find(name[i]);
            if (it == nodes[beg].next.end()) assert(false);
            beg = (*it).second;
        }   
        assert(!nodes[beg].objects.empty());
        return nodes[beg].objects.top();
    }
};

}  // namespace native

#endif
~~~

​	首先，使用命名空间 `native` 来定义整个类，是为了防止其与其他代码中的同名实体发生冲突。于是使用 `trie` 类的方式就成为了 `native::trie`。

​	字典树 `trie` 的成员变量包含结点 `node`、结点存储指向对象的指针的栈 `object`、用于存储字符到下一个结点映射关系的无序映射表 `next`；同时包含结点表 `nodes`。其函数包括构造函数、插入函数 `insert`、查找操作 `find`。也很好理解，插入就是插入一个新的字符串（从根到叶子）；查找就是找一下是否有和已知字符串 `name` 匹配的字符串。

​	对于字典树 `trie` 的使用，您可以看到：我们在 `compiler.cpp` 中定义了一个 `global_trie`，然后举例来说，我们搞了一个识别 `return` 语句的函数 `define_return`：

~~~
void define_return(std::string s, int valid) {
    std::vector<std::string> fun = split(s);
    if (valid != -1) {
        memory::free_stack();
    }
    assert(fun.size() == 2);
    std::string type = fun[0];
    std::string name = fun[1];
    assert(type == "return");
    int n = name.size();
    for (int i = 0; i < n; i ++) {
        if (std::isalpha(name[i]) || name[i] == '_') {
            int l = i;
            while (i < n && (std::isalnum(name[i]) || name[i] == '_')) i ++;
            std::string temp = name.substr(l, i - l);
            obj::object* who = global_trie.find(temp);
            if (who->type == "value") {
                obj::int_object* res = (obj::int_object*)who;
                mips::return_value(res->load_reg(2));
                res->free_reg();
            } else if (who->type == "function") {
                obj::function_object* fun = (obj::function_object*)who;
                /// TODO: check the expression is correct or not
                obj::int_object* res = work(fun, name, i);
                mips::return_value(res->load_reg(2));
                res->free_reg();
            } else assert(0); // Compile Error if the variable is not exist
            break;
        } else if (std::isdigit(name[i])) {
            int res = name[i] - '0';
            int j = i + 1;
            while (j < n && std::isdigit(name[j])) {
                res = res * 10 + name[j ++] - '0';
            }
            obj::int_object ans("res", 0, res, valid + 1);
            mips::return_value(ans.load_reg(2));
            break;
        }
    }
}
~~~

​	稍微解释一下这个函数：`split` （这是我们自己写的一个函数）之后，输入字符串被分成了两部分，一部分是类型（type），一部分是值（name）。`assert` 语句保证了类型一定是 `return`。查看值，分以下情况：

1. 如果 name 是字母或下划线，说明本句的格式是 `return [变量名]` 或 `return [函数名](参数表)`，于是继续向后查找直至获得的字符不是字母或下划线，得到之后做截断，在 `global_trie` 中查找之。随后又分变量和函数讨论。
2. 如果 name 是数字，说明本句的格式是 `return [立即数]`。直接处理数字即可。

再举一个例子，我们定义了函数 `define_int`：

~~~
void define_int(std::string s, int valid) {
    std::vector<std::string> fun = split(s);
    assert(fun.size() == 2);
    std::string type = fun[0];
    std::string name = fun[1];
    obj::int_object* obj = new obj::int_object(name, true, 0, valid);
    global_trie.insert((obj::object*)obj);
}
~~~

​	解释一下这个函数：`split` 之后，输入字符串被分成了两部分，一部分是类型（type），一部分是值（name），那么显然会有 `fun[0] = 'int'`。于是调用 `int_object` 构造函数实现整数赋值，并将其插入字典树中。举例来说，如果我有个语句 `int a = 2`，则最后字典树会存入的是 *// to do : 存入的是什么？*



### 3. /obj

​	/obj 文件夹里的文件就是我们的一系列类的定义文件和函数的实现文件。一切的基础是 `object.h` 和 `object.cpp`。我们先来看一看`object.h` ：

~~~
#ifndef OBJECT
#define OBJECT

#include <string>
#include "../mips/utils.h"
#include "../native/short_str.h"
#include <vector>
#include <iostream>
#include <assert.h>

namespace memory {
class stack_pos {
  private:
    int sPos; // StackPos
    int flag; // flag = 1: Arg flag = 0: Common
  public:
    stack_pos();
    stack_pos(size_t size);
    native::Str16 get_pos(size_t size);
};

class register_pos {
  private:
    static int RegNum;
    static register_pos* regs;

    int rPos;
    
  public:
    obj::value_object* nowUsing;
    register_pos();
    native::Str16 get_pos();
    
    friend void free_reg(register_pos* reg);
    friend void check_regs();
    friend register_pos* alloc_reg(obj::value_object* yue);
    friend register_pos* alloc_reg(obj::value_object* yue, int x);
};

  
} // namespace memory

namespace obj {

class object {
public:
    std::string name;
    std::string type;
    int valid_space;
    object();
    object(std::string name, std::string type, int valid_space);
    object(const object& other);
};

// 一开始不需要分配reg，只分配stk
class value_object: public object {
    private:
    memory::stack_pos stk_pos;
    memory::register_pos* reg_pos;
    public:
    value_object(const value_object& other);
    value_object(std::string name, std::string type, int valid_space);
    value_object();
    native::Str16 load_reg();
    native::Str16 load_reg(int x);
    virtual void store2reg();
    void free_reg();
    native::Str16 load_stk();
    friend class memory::register_pos;
    friend class memory::stack_pos;
};

} // namespace obj

#endif
~~~

​	上述代码的目的定义对象相关的类和函数，以及与内存和寄存器分配相关的功能。定义了两个命名空间 `obj` 和 `memory`。`memory` 中定义了栈位置类和寄存器位置类。`obj` 中定义了两个类 `value_object` 和 `object`，`object` 类中有用的内容包括对象类型、名称、有效空间大小；`value_object` 继承自 `object` 类，除了 `object` 的成员之外还包含栈位置对象、寄存器位置对象、存入寄存器的函数、加载寄存器的函数、加载栈的函数等。需要注意的是，在 `value_object` 之中，如果我们定义了一个变量，一开始并不需要分配寄存器（可以没有 `reg_pos`），只分配栈即可（要有 `stk_pos`）。

​	说完了一切类的基础，我们来看一个具体类的实现。在 `intobj.h` 中，我们是这么写的：

~~~
#ifndef INT_OBJECT
#define INT_OBJECT

#include <string>
#include "object.h"

namespace obj {

class int_object: public value_object{
public:
    bool proper;
    int cval;
public:
    // native::Str16 load_reg();
    int_object(std::string name, bool proper, int cval, int valid_space);
    int_object(const int_object& other);
    void store2reg();
    friend void cmd_add(int_object& lhs, int_object& rhs);
    friend void cmd_sub(int_object& lhs, int_object& rhs);
    friend void cmd_mul(int_object& lhs, int_object& rhs);
    friend void cmd_and(int_object& lhs, int_object& rhs);
    friend void cmd_or(int_object& lhs, int_object& rhs);
    friend void cmd_xor(int_object& lhs, int_object& rhs);
    friend void cmd_le(int_object& lhs, int_object& rhs);
    friend void cmd_slt(int_object& lhs, int_object& rhs);
    friend void cmd_sgt(int_object& lhs, int_object& rhs);
    friend void cmd_ge(int_object& lhs, int_object& rhs);
    friend void cmd_eq(int_object& lhs, int_object& rhs);
    friend void cmd_ne(int_object& lhs, int_object& rhs);
    friend void cmd_div(int_object& lhs, int_object& rhs);
    friend void cmd_mod(int_object& lhs, int_object& rhs);
    friend void cmd_assign(int_object& lhs, int_object& rhs);
};

void cmd_add(int_object& lhs, int_object& rhs);
void cmd_sub(int_object& lhs, int_object& rhs);
void cmd_mul(int_object& lhs, int_object& rhs);
void cmd_and(int_object& lhs, int_object& rhs);
void cmd_or(int_object& lhs, int_object& rhs);
void cmd_xor(int_object& lhs, int_object& rhs);
void cmd_le(int_object& lhs, int_object& rhs);
void cmd_slt(int_object& lhs, int_object& rhs);
void cmd_sgt(int_object& lhs, int_object& rhs);
void cmd_ge(int_object& lhs, int_object& rhs);
void cmd_eq(int_object& lhs, int_object& rhs);
void cmd_ne(int_object& lhs, int_object& rhs);
void cmd_div(int_object& lhs, int_object& rhs);
void cmd_mod(int_object& lhs, int_object& rhs);
void cmd_assign(int_object& lhs, int_object& rhs);

}  // namespace obj
#endif
~~~









### 4. /mips

​	/mips 文件夹包括文件 `utls.h`，目的是定义一些在 `mips` 命名空间中的内联函数。



### 5. 主文件





### 6. 样例编写

​	在样例编写的部分，我来讲一个故事。

​	首先，由于无法获得测试平台上的样例。在测试阶段，我写了如下的测试样例。这个测试样例能把我们**实现的所有功能**带全。

~~~
int main() {
    int a;
    int b;
    int c;
    int d;
    int e;
    int f;
    int g;
    int r;

	a = 24;
    b = 15;
    c = 6;
    d = 3;
    e = 2;
    f = 8;
    g = 5;

  
    r = (a - b) * (d + e) + c % d + ((f > g) * (a + c)) / e + (d & e) + (a > b) + (g <= f);

    println_Int(r);  
    return 0;
}
~~~

上述样例的答案是：64。

​	戏剧化的是，上述代码跑出来的结果一直都不对（经常出现 63）。所以我又写了以下的测试样例。

~~~

~~~

