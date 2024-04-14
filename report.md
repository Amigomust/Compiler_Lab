<center>北京师范大学</center>
<center>2023-2024学年春季学期  “编译原理”实验报告</center>
<center><b><font size=6 bold=true>实验二    第二代编译器</font><b></center>

<hr>


## 一、实验要求

​	二代编译器将 C 语言顺序语句序列翻译为等价的汇编程序，所输出的汇编程序符合 MIPS 汇编语言格式要求。二代编译器能够处理的文法如下：

1. **关键字**：int, return, main
2. **标识符**：符合 C 语言要求的标识符
3. **常量**：十进制整型，如 1、223、10 等
4. **操作符**：=、+、-、*、/、%、<、<=、>、>=、==、!=、&（按位与）、|（按位或）、^（按位异或）
5. **分隔符**： ；、{、}
6. **语句**：变量声明（单变量且无初始化）、表达式语句、赋值语句、return、main 函数定义（不带参数 + 带参数）

​	

## 二、组内分工

|  姓    名  |        分   工        |
| :--------: | :-------------------: |
|   陈志锐   |      代码主编写       |
|   孙旭琪   |      代码副编写1      |
|    梁鲲    | 代码副编写2、样例编写 |
| **徐照琦** |  总设计、代码副编写3  |



## 三、报告正文

### 1. 代码设计（XZQ）

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

​	在写到一半的时候，我们发现，上述的内容只会将整个项目引向屎山。因为函数之间的关系极其混乱，函数内部的实现也极其混乱。举例来说，我们为每一种对象都做了很多几乎一致的函数，如 `add`、`sub` 之类。这就在一定程度上违反了软件过程的**高内聚、低耦合**的要求。这种错误的一种修正方式是：定义友元函数。由此可以在需要时让特定的非成员函数与类的私有成员或保护成员进行交互，从而提高程序的灵活性和可维护性。另外，实际上，我们根本就没必要定义立即数类，只需要通过字符串的匹配查看其是否全是数字即可。

​	于是我们开始了艰苦的重构生涯。首先，我们删除了立即数类，// *to do：添加了什么？*

​	然后是重构后的整数类：

~~~
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

}	
~~~

​	可以看到的是，对于同其它类有关的函数，我们都定义了友元。其中的 `lhs` 和 `rhs` 分别代表左操作数和右操作数。例如，在 `cmd_le` 中，就是要判断是否存在关系 `lhs` $\le$ `rhs`，这当然是要通过寄存器指令来完成的。如下所示：

~~~
void cmd_le(int_object& lhs, int_object& rhs) {
    if (lhs.proper) {
        mips::binary_right("slt", rhs.load_reg(), lhs.load_reg(), __PRETTY_FUNCTION__);
        mips::binary_imm("xori", lhs.load_reg(), 1, __PRETTY_FUNCTION__);
        rhs.free_reg();
        return;
    }
    if (rhs.proper) {
        mips::binary_right("slt", rhs.load_reg(), lhs.load_reg(), __PRETTY_FUNCTION__);
        mips::binary_imm("xori", rhs.load_reg(), 1, __PRETTY_FUNCTION__);
        lhs.free_reg();
        std::swap(lhs, rhs);
        return;
    }
    debug(lhs.cval, rhs.cval);
    lhs.cval = lhs.cval <= rhs.cval;
    debug(lhs.cval);
}
~~~

其中 `binary_right` 函数和 `binary_imm` 函数的实现如下：

~~~
inline void binary_right(const char* op, native::Str16 lhs, native::Str16 rhs, const char* from = "") {
  std::printf("%s %s, %s, %s\t\t#%s\n", op, rhs.c_str(), lhs.c_str(), rhs.c_str(), from);
}

inline void binary_imm(const char* op, native::Str16 lhs, const int imm, const char* from = "") {
  std::printf("%s %s, %s, %d\t\t#%s\n", op, lhs.c_str(), lhs.c_str(), imm, from);
}
~~~

显然 `binary_right` 完成了生成把值存入右寄存器的 MIPS 指令生成；`binary_imm` 则完成了立即数的操作。







### 2. 字符串操作

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

​	解释一下这个函数：`split` 之后，输入字符串被分成了两部分，一部分是类型（type），一部分是值（name），那么显然会有 `fun[0] = 'int'`。于是调用 `int_object` 构造函数实现整数赋值，并将其插入字典树中。举例来说，如果我有个语句 `int a = 2`，则最后字典树会存入的是 `a`。



### 3. 对象实现（CZR）

​	我们定义了一系列类的定义文件和函数的实现文件。一切的基础是 `object.h` 和 `object.cpp`。我们先来看一看`object.h` ：

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

上述代码几乎涵盖了实验要求中的对 `int` 的所有操作。值得一提的是，对于一些操作符的使用（`+-*&^|`），我们是通过定义宏来实现的。例如 `cmd_add`：

~~~
#define DEFINE_BINARY_OP(NAME, OP)                                      \
    void cmd_##NAME(int_object& lhs, int_object& rhs) {                 \
        if (lhs.proper) {                                               \
            mips::binary_left(#NAME, lhs.load_reg(), rhs.load_reg(), __PRETTY_FUNCTION__);   \
            rhs.free_reg();                                             \
            return;                                                     \
        }                                                               \
        if (rhs.proper) {                                               \
            mips::binary_right(#NAME, lhs.load_reg(), rhs.load_reg(), __PRETTY_FUNCTION__);  \
            lhs.free_reg();                                             \
            std::swap(lhs, rhs);                                        \
            return;                                                     \
        }                                                               \
        lhs.cval = lhs.cval OP rhs.cval;                                \
    }

DEFINE_BINARY_OP(add, +)
~~~



​	除此之外，我们还实现了 `fun_obj.h`，用于生成函数对象。该文件在未来将会成为用户自定义函数语句实现的文件，暂时则只是实现 `main` 函数的文件。如下所示：

~~~
#ifndef FUN_OBJ
#define FUN_OBJ

#include "object.h"
#include "int_obj.h"
#include <vector>
namespace obj {

class function_object: public object{
    public:
    std::vector<int_object*> args;
    int_object* work(std::vector<int_object*>& obj);
    function_object(std::string name, int valid, std::vector<int_object*> args);
};

} // namespace obj

#endif
~~~





​	





### 4. MIPS 的内联函数（SXQ）

​	`utls.h` 文件定义了一些在 `mips` 命名空间中的内联函数。

~~~
#ifndef UTILS
#define UTILS

#include <cstdio>

#include "short_str.h"


namespace mips {

inline void binary_left(const char* op, native::Str16 lhs, native::Str16 rhs, const char* from = "") {
  std::printf("%s %s, %s, %s\t\t#%s\n", op, lhs.c_str(), lhs.c_str(), rhs.c_str(), from);
}

inline void binary_right(const char* op, native::Str16 lhs, native::Str16 rhs, const char* from = "") {
  std::printf("%s %s, %s, %s\t\t#%s\n", op, rhs.c_str(), lhs.c_str(), rhs.c_str(), from);
}

inline void binary_imm(const char* op, native::Str16 lhs, const int imm, const char* from = "") {
  std::printf("%s %s, %s, %d\t\t#%s\n", op, lhs.c_str(), lhs.c_str(), imm, from);
}

inline void binary_div_left(const char* op, const char* op2, native::Str16 lhs, native::Str16 rhs, const char* from = "") {
  std::printf("%s %s, %s\t\t#%s\n", op, lhs.c_str(), rhs.c_str(), from);
  std::printf("%s %s\n", op2, lhs.c_str());
}

inline void binary_div_right(const char* op, const char* op2, native::Str16 lhs, native::Str16 rhs, const char* from = "") {
  std::printf("%s %s, %s\t\t#%s\n", op, lhs.c_str(), rhs.c_str(), from);
  std::printf("%s %s\n", op2, rhs.c_str());
}

// save and load
inline void binary_access(const char* op, const native::Str16 reg, const native::Str16 stk, const char* from = "") {
  std::printf("%s %s, %s\t\t#%s\n", op, reg.c_str(), stk.c_str(), from);
}

// free and allocate
inline void binary_manage(const char* op, const native::Str16 stk, const int size, const char* from = "") {
  binary_imm(op, stk, size, from);
}

inline void system_print(const native::Str16 x) {
  binary_access("li", "$v0", "1");
  std::printf("syscall\n");
  binary_access("li", "$v0", "4");
  binary_access("la", "$a0", "newline");
  std::printf("syscall\n");
}

inline void return_value(const native::Str16 x) {
  std::printf("jr $31\n");
}


}
#endif
~~~

这个文件相对也比较简单。我们在命名空间 `mips` 中定义了一些用于生成 MIPS 汇编指令的工具函数。// *to do：举个例子*



### 5. 主代码与杂项（CZR）

​	在主代码、对象实现。MIPS 指令和 Trie 树之外，我们还需要做一些准备操作。为此，我们写出了 `memory.cpp` 这个用于内存管理的文件。// *to do：memory.cpp 的介绍*



​	主代码（不是 main 函数，而是主编译器）的定义出现在 `compile.h`里：

~~~
#pragma once 

#include <iostream>
#include <stack>
#include <map>
#include <vector>
#include <regex>
#include <assert.h>
#include "trie.h"
#include "short_str.h"
#include "object.h"
#include "fun_obj.h"
#include "int_obj.h"
#include "utils.h"

namespace css {

void define_function(std::string s, int valid);
void define_int(std::string s, int valid);
void compile_expression(std::string s, int valid);
void compile_code(std::string s, int valid);
std::vector<obj::int_object*> define_arg(std::string s, int valid);
std::vector<obj::int_object*> extract_args(std::string s);
std::string remove_begend_space(std::string s);
std::string regular(std::string code);
std::vector<std::string> split(std::string s);
obj::int_object* work(obj::function_object* obj, std::string& s, int& beg);

}

~~~

​	我们来讲述其中一个相对重要的函数 `compile_expression`，这个函数是用于定义编译表达式的。下面是它的代码：
~~~
void compile_expression(std::string s, int valid) {
    
    s = remove_begend_space(s);
    std::unordered_map<std::string, int> Sin = { 
        {"(", 1}, {")", 18}, 
        {"*", 17}, {"%", 17}, {"/", 17}, 
        {"+", 15}, {"-", 15}, 
        {"==", 13}, {"!=", 13},
        {"<=", 11}, {">=", 11}, {"<", 11}, {">", 11},
        {"&", 9}, 
        {"^", 7}, 
        {"|", 5},
        {"=", 2}};
    std::unordered_map<std::string, int> Sout = {
        {"(", 18}, {")", 1},
         {"*", 16}, {"%", 16}, {"/", 16}, 
        {"+", 14}, {"-", 14},
         {"==", 12}, {"!=", 12},
         {"<=", 10}, {">=", 10}, {"<", 10}, {">", 10},
         {"&", 8}, 
         {"^", 6}, 
         {"|", 4},
         {"=", 3}};
    
    std::stack<obj::int_object> num; std::stack<std::string> op;
    int n = s.size();
    auto cal = [&]() -> void {
        std::string c = op.top(); op.pop();
        if (c == "(") return;   
        obj::int_object b = num.top(); num.pop();
        obj::int_object a = num.top(); num.pop();
        debug(((a -> proper) ? (a -> name) : std::to_string(a -> cval)), c, ((b -> proper) ? (b -> name) : std::to_string(b -> cval)));
        if (c == "+") obj::cmd_add(a, b);
        else if (c == "-") obj::cmd_sub(a, b);
        else if (c == "*") obj::cmd_mul(a, b);
        else if (c == "/") obj::cmd_div(a, b);
        else if (c == "%") obj::cmd_mod(a, b);
        else if (c == "&") obj::cmd_and(a, b);
        else if (c == "|") obj::cmd_or(a, b);
        else if (c == "^") obj::cmd_xor(a, b);
        else if (c == "<") obj::cmd_slt(a, b);
        else if (c == ">") obj::cmd_sgt(a, b);
        else if (c == "<=") obj::cmd_le(a, b);
        else if (c == ">=") obj::cmd_ge(a, b);
        else if (c == "==") obj::cmd_eq(a, b);
        else if (c == "=") obj::cmd_assign(a, b);
        else if (c == "!=") obj::cmd_ne(a, b);
        else assert(0);

        if (c == "=") {
            num.push(a);
        } else {
            if (a.proper) {
                obj::int_object temp("res", 1, 0, -1);
                a.save_reg(temp.load_stk());
                num.push(temp);
            } else {
                num.push(a);
            }
        }
    };
    // vector<string> opName = {"+", "-", "*", "/", "%", 
    // "<", ">", "<=", ">=", "==", 
    // "&", "|", "^"};
    std::string opName = "+-*/%<>=&|^!";
	for (int i = 0; i < n; i++) {
        if (s[i] == ' ') continue;
        bool neg = false;
        if (isdigit(s[i]) || neg) {
            if (neg) {
                while (!isdigit(s[i])) i ++;
            }
            int res = s[i] - '0';
            int j = i + 1;
            while (j < n && isdigit(s[j])) {
                res = res * 10 + s[j ++] - '0';
            }
            if (neg) res = -res;
            num.push(obj::int_object(std::to_string(res), 0, res, valid + 1));
            i = j - 1;
        } else if (isalpha(s[i]) || s[i] == '_') {
            int j = i + 1;
            std::string temp = "";
            temp += s[i];
            while (j < n && (isalpha(s[j]) || isdigit(s[j]) || s[j] == '_')) temp += s[j ++];

            obj::object* who = global_trie.find(temp);
            if (who->type == "value") {
                num.push(*((obj::int_object*)who));
            } else if (who->type == "function") {
                obj::function_object* fun = (obj::function_object*)who;
                num.push(*work(fun, s, j));
                assert(s[j] == ')');
                j ++;
            } else assert(0); // Compile Error if the variable is not exist
            i = j - 1;
        } else if (opName.find(s[i]) != opName.npos) {
            int j = i + 1;
            std::string temp = "";
            temp += s[i];
            while (j < n && opName.find(s[j]) != opName.npos) temp += s[j ++];
            while (op.size() && Sin[op.top()] > Sout[temp]) cal();
            op.push(temp);
            i = j - 1;
        } else if (s[i] == ')') {
            while (op.size() && op.top() != "(") cal();
            op.pop();
        } else {
            std::string temp;
            temp += s[i];
            while (op.size() && Sin[op.top()] > Sout[temp]) cal();
			op.push(temp); // Compile Error if the expression is not exist
        }
    }
	while (op.size()) {
        cal();
    }
}
~~~

这个函数完成了对表达式的处理。首先，要清除表达式前后的一切空格（`remove_begend_space`），然后初始化运算符的优先级表。我们要着重聊一下这个优先级表。因为初期的时候这个表存在很大的问题：

1. 我们的优先级表和真实的 C 语言的优先级表有一些不同，真正的优先级表阐述的是：`== `和 `!=` 的优先级要小于 `<=`、`>=` 这些。然而上述代码的评测却没有任何问题。这显然应当是**评测样例不够充分**的缘故。
2. 在第一次的时候，这个表存在的很大问题是：压根没写 `!=`，于是我们补上了它；在最后一次提交之前，我们发现在这时的优先级表中，`&^|`三个操作符的优先级是一致的，这直接导致了最后一个评测样例没过。在修正之后，我们成功取得了 $10$ 分的成绩。也就是说。这个操作符表代表着我们 debug 历程的起始和结束，非常有纪念意义。

在操作符表介绍完毕之后，我们继续来看代码：在创建操作符栈和数栈之后，查看表达式。

1. 如果是数字或者负号：如果是负号，则跳过（因为本次实验并不需要实现负数）；是数字，提取数字，并将其作为整数对象压入操作数栈中。
2. 如果是字母或下划线：获取变量名；在全局字典中查找变量，如果是值类型，则将其压入操作数栈中；如果是函数类型，调用函数并将结果作为整数对象压入操作数栈中。
3. 如果是运算符：获取连续的运算符；根据当前运算符和栈顶运算符的优先级比较，执行相应的计算操作；将当前运算符压入运算符栈中。
4. 如果是右括号：执行栈中的计算。

​	接下来，我们来看实现了编译的重要函数 `compile_code`：

~~~
void compile_code(std::string s, int valid) {
    s = remove_begend_space(s);
    pause();
    if (s.size() == 0) return;
    std::vector<int> Brackets(3);
    std::unordered_map<char, int> BracketsMap = {{'{', 0}, {'(', 1}, {'[', 2}, {'}', 0}, {')', 1}, {']', 2}};
    for (int i = 0; i < (int)s.size(); i ++) {
        int l = i;
        while (s[i] != ';' || (Brackets[0] || Brackets[1] || Brackets[2])) {
            if (s[i] == '{' || s[i] == '(' || s[i] == '[') Brackets[BracketsMap[s[i]]] ++;
            else if (s[i] == '}' || s[i] == ')' || s[i] == ']') Brackets[BracketsMap[s[i]]] --;
            assert(Brackets[0] >= 0 && Brackets[1] >= 0 && Brackets[2] >= 0); // Compile Error if the brackets are not matched
            i ++;
        }
        pause();
        // system("pause");
        if (i != l) {
            std::regex function_expression("\\s*(int|void)\\s*([a-z_A-Z][a-z_A-Z0-9]*)\\s*\\((.*)\\)\\s*\\{(.*)\\}\\s*");
            std::regex define_expression("\\s*(int)\\s*([a-z_A-Z][a-z_A-Z0-9]*)\\s*");
            std::regex return_expression("\\s*return(.*)");
            std::string temp = s.substr(l, i - l);
            if (std::regex_match(temp, function_expression)) {
                pause();
                define_function(temp, valid);
            } else if (std::regex_match(temp, define_expression)){
                define_int(temp, valid);
            } else if (std::regex_match(temp, return_expression)) {
                define_return(temp, valid);
            } else { compile_expression(temp, valid);}
        }
    }
}
~~~

代码的实现当然也比较暴力。首先，清楚字符串两端的空格；然后通过初始化一个计数器向量 `Brackets`，用于跟踪括号的数量、初始化一个括号映射表 `BracketsMap`，用于记录各种括号的类型；随即要遍历字符串中的所有字符：

1. 记录当前位置为 `l`。

2. 当字符不是分号，或者括号数量不为0时，继续循环。如果当前字符是左括号，则增加相应括号计数器；如果当前字符是右括号，则减少相应括号计数器；如果任一括号计数器小于0，则断言失败，表示括号不匹配。

3. 更新索引 `i`，如果 `i != l`，则表示找到了一个代码片段。

- 提取子字符串 `temp`，即从位置 `l` 到位置 `i` 的部分。
- 使用正则表达式匹配 `temp`，以确定代码片段的类型。
  - 如果是函数定义，则调用 `define_function` 函数。
  - 如果是整数定义，则调用 `define_int` 函数。
  - 如果是返回语句，则调用 `define_return` 函数。
  - 否则，调用 `compile_expression` 函数编译表达式。

​	值得一说的是，在代码提交到 $8$ 分之后，就是通过修改正则表达式来取得 $9$ 分的。因为在样例中存在 `e06.c` 这样的一个奇葩文件（就不放上来了），使我们发现了正则表达式中的错误：没有后续空格，即：`\\s*(int)\\s*([a-z_A-Z][a-z_A-Z0-9]*)*`，而不是`\\s*(int)\\s*([a-z_A-Z][a-z_A-Z0-9]*)\\s*`。修改之后，当然也是又过了一个点。



### 6. 样例编写（LK）

​	在样例编写的部分，我来讲一个故事。

​	首先，在测试阶段，我写了如下的测试样例。这个测试样例几乎能带全我们**实现的所有功能**。

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
    r = (a - b) * (d + e) + c % d + ((f > g) * (a + c)) / e + (d & e) + (a > b) + (g <= f);
    println_Int(r);  
    r = (a - b) * (d + e) + c % d + ((f > g) * (a + c)) / e + (d & e) + (a > b) + (g <= f);
    println_Int(r);  
    r = (a - b) * (d + e) + c % d + ((f > g) * (a + c)) / e + (d & e) + (a > b) + (g <= f);
    println_Int(r);  
    r = (a - b) * (d + e) + c % d + ((f > g) * (a + c)) / e + (d & e) + (a > b) + (g <= f);
    println_Int(r);  
    return 0;
}
~~~

上述样例的答案是：64

​	戏剧化的是，上述代码跑出来的结果一直都不对（存在寄存器污染问题和代码构想的问题）。

​	在这个样例之后，我们经过无尽的 debug，终于是通过了测试。这时的代码在希冀平台上的得分为 $7$.

​	此时的代码存在的问题如下：**`CMakeLists.txt` 的书写不正确、`!=` 忘记实现、正则表达式的实现出错、`&^|`操作符的优先级存在问题。**
