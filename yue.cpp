#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <map>
#include <assert.h>
#include <string.h>
#include <unordered_map>
#include <regex>

#ifdef LOCAL
#include "debug.h"
#else
#define debug(...) do{} while (false)
#endif

#define IDMASK 0x0000ffff
#define TYPEMASK 0x00ff0000
#define VALMASK 0xff000000

using namespace std;

using PSS = pair<string, string>;
using PII = pair<int, int>;

stack<string> VariableName;

/*
function: 2
int: 1
special: 0
intargs: 3
*/

// Class Definition

/*
int id: the id of the element
int type: the type of the element, 0: special, 1: int, 2: function, 3: intargs
int validSpace: the valid space of the element
*/
class ElementID {
    private:
        int id;
        int type;
        int validSpace;
    public:
        ElementID(int i = 0, int t = 0, int v = 0): id(i), type(t), validSpace(v) {}
        int getId() { return id; }
        int getType() { return type; }
        int getValidSpace() { return validSpace; }
        tuple<int, int, int> Get() { return make_tuple(id, type, validSpace); }
};

class Trie {
    private:
        struct Node {
            unordered_map<char, int> next;
            stack<ElementID> value;
        };
        vector<Node> nodes;
    public:
        void Insert(string s, ElementID v) {
            int beg = 0;
            VariableName.push(s);
            for (int i = 0; i < s.size(); i ++) {
                unordered_map<char, int>::iterator it = nodes[beg].next.find(s[i]);
                if (it != nodes[beg].next.end()) beg = it -> second;
                else {
                    nodes.push_back(Node{});
                    nodes[beg].next[s[i]] = nodes.size() - 1;
                    beg = nodes.size() - 1;
                }
            }
            nodes[beg].value.push(v);
        }
        void Remove(string& s) {
            int beg = 0;
            for (int i = 0; i < s.size(); i ++) {
                unordered_map<char, int>& next = nodes[beg].next;
                unordered_map<char, int>::iterator it = next.find(s[i]);
                if (it == next.end()) return;
                beg = it->second;
            }
            if (!nodes[beg].value.empty())
                nodes[beg].value.pop();
            return;
        }

        Trie() {
            nodes.push_back(Node());
        }

        ElementID Find(string& s) {
            int beg = 0;
            for (int i = 0; i < s.size(); i ++) {
                unordered_map<char, int>& next = nodes[beg].next;
                unordered_map<char, int>::iterator it = next.find(s[i]);
                if (it == next.end()) {
                    assert(0); // Compile Error if the variable is not exist
                }
                beg = it->second;
            }
            
            if (nodes[beg].value.empty()) assert(0); // Compile Error if the variable is not exist
            return nodes[beg].value.top();
        }
} GlobalTrie, SpecialTrie, CommonTrie;

class Element {
    protected:
        int stackPos;
        int regPos;
    public:
        Element(int pos = 0, int reg = 0): stackPos(pos), regPos(reg) {}
        // virtual int getStackPos();
        // virtual void setStackPos(int pos);
};

// flag = 1: immediate value, flag = 0: stack value
/*
name: ElementNam
value:
*/
class IntElement: Element{
    private:
        string name;
        int value;
        int flag;
    public:
        IntElement(string n = "default", int v = 998244353, int stackPos = -1, int f = 0, int reg = -1) {
            name = n;
            value = v;
            stackPos = stackPos;
            f = flag;
            regPos = reg;
        }
        IntElement(const IntElement& a) {
            name = a.name;
            value = a.value;
            stackPos = a.stackPos;
            flag = a.flag;
            stackPos = a.stackPos;
            regPos = a.regPos;
        }
        static size_t getNeedMemory() { return 4; }
        IntElement operator+(const IntElement& a) {
            if (a.flag && flag) return IntElement(name, value + a.value, 0, 1, -1);
            if (a.flag) printf("%s + %d\n", name.c_str(), a.value);
            else if (flag) printf("%d + %s\n", value, a.name.c_str());
            else printf("%s + %s\n", name.c_str(), a.name.c_str());
            return IntElement(a.regPos == 0 ? a.name : name, value, 0, 0, 0);
        }
        IntElement operator-(const IntElement& a) {
            if (a.flag && flag) return IntElement(name, value - a.value, 0, 1, -1);
            if (a.flag) printf("%s - %d\n", name.c_str(), a.value);
            else if (flag) printf("%d - %s\n", value, a.name.c_str());
            else printf("%s - %s\n", name.c_str(), a.name.c_str());
            return IntElement(a.regPos == 0 ? a.name : name, value, 0, 0, 0);
        }
        IntElement operator*(const IntElement& a) {
            if (a.flag && flag) return IntElement(name, value * a.value, 0, 1, -1);
            if (a.flag) printf("%s * %d\n", name.c_str(), a.value);
            else if (flag) printf("%d * %s\n", value, a.name.c_str());
            else printf("%s * %s\n", name.c_str(), a.name.c_str());
            return IntElement(a.regPos == 0 ? a.name : name, value, 0, 0, 0);
        }
        IntElement operator/(const IntElement& a) {
            if (a.flag && flag) return IntElement(name, value / a.value, 0, 1, -1);
            if (a.flag) printf("%s / %d\n", name.c_str(), a.value);
            else if (flag) printf("%d / %s\n", value, a.name.c_str());
            else printf("%s / %s\n", name.c_str(), a.name.c_str());
            return IntElement(a.regPos == 0 ? a.name : name, value, 0, 0, 0);
        }
        IntElement operator%(const IntElement& a) {
            if (a.flag && flag) return IntElement(name, value % a.value, 0, 1, -1);
            if (a.flag) printf("%s %% %d\n", name.c_str(), a.value);
            else if (flag) printf("%d %% %s\n", value, a.name.c_str());
            else printf("%s %% %s\n", name.c_str(), a.name.c_str());
            return IntElement(a.regPos == 0 ? a.name : name, value, 0, 0, 0);
        }
        IntElement operator<(const IntElement& a) {
            if (a.flag && flag) return IntElement(name, value < a.value, 0, 1, -1);
            if (a.flag) printf("%s < %d\n", name.c_str(), a.value);
            else if (flag) printf("%d < %s\n", value, a.name.c_str());
            else printf("%s < %s\n", name.c_str(), a.name.c_str());
            return IntElement(a.regPos == 0 ? a.name : name, value, 0, 0, 0);
        }
        IntElement operator<=(const IntElement& a) {
            if (a.flag && flag) return IntElement(name, value <= a.value, 0, 1, -1);
            if (a.flag) printf("%s <= %d\n", name.c_str(), a.value);
            else if (flag) printf("%d <= %s\n", value, a.name.c_str());
            else printf("%s <= %s\n", name.c_str(), a.name.c_str());
            return IntElement(a.regPos == 0 ? a.name : name, value, 0, 0, 0);
        }
        IntElement operator>(const IntElement& a) {
            if (a.flag && flag) return IntElement(name, value > a.value, 0, 1, -1);
            if (a.flag) printf("%s > %d\n", name.c_str(), a.value);
            else if (flag) printf("%d > %s\n", value, a.name.c_str());
            else printf("%s > %s\n", name.c_str(), a.name.c_str());
            return IntElement(a.regPos == 0 ? a.name : name, value, 0, 0, 0);
        }
        IntElement operator>=(const IntElement& a) {
            if (a.flag && flag) return IntElement(name, value >= a.value, 0, 1, -1);
            if (a.flag) printf("%s >= %d\n", name.c_str(), a.value);
            else if (flag) printf("%d >= %s\n", value, a.name.c_str());
            else printf("%s >= %s\n", name.c_str(), a.name.c_str());
            return IntElement(a.regPos == 0 ? a.name : name, value, 0, 0, 0);
        }
        IntElement operator==(const IntElement& a) {
            if (a.flag && flag) return IntElement(name, value == a.value, 0, 1, -1);
            if (a.flag) printf("%s == %d\n", name.c_str(), a.value);
            else if (flag) printf("%d == %s\n", value, a.name.c_str());
            else printf("%s == %s\n", name.c_str(), a.name.c_str());
            return IntElement(a.regPos == 0 ? a.name : name, value, 0, 0, 0);
        }
        IntElement operator!=(const IntElement& a) {
            if (a.flag && flag) return IntElement(name, value != a.value, 0, 1, -1);
            if (a.flag) printf("%s != %d\n", name.c_str(), a.value);
            else if (flag) printf("%d != %s\n", value, a.name.c_str());
            else printf("%s != %s\n", name.c_str(), a.name.c_str());
            return IntElement(a.regPos == 0 ? a.name : name, value, 0, 0, 0);
        }
        IntElement operator&(const IntElement& a) {
            if (a.flag && flag) return IntElement(name, value & a.value, 0, 1, -1);
            if (a.flag) printf("%s & %d\n", name.c_str(), a.value);
            else if (flag) printf("%d & %s\n", value, a.name.c_str());
            else printf("%s & %s\n", name.c_str(), a.name.c_str());
            return IntElement(a.regPos == 0 ? a.name : name, value, 0, 0, 0);
        }
        IntElement operator|(const IntElement& a) {
            if (a.flag && flag) return IntElement(name, value | a.value, 0, 1, -1);
            if (a.flag) printf("%s | %d\n", name.c_str(), a.value);
            else if (flag) printf("%d | %s\n", value, a.name.c_str());
            else printf("%s | %s\n", name.c_str(), a.name.c_str());
            return IntElement(a.regPos == 0 ? a.name : name, value, 0, 0, 0);
        }
        IntElement operator^(const IntElement& a) {
            if (a.flag && flag) return IntElement(name, value ^ a.value, 0, 1, -1);
            if (a.flag) printf("%s ^ %d\n", name.c_str(), a.value);
            else if (flag) printf("%d ^ %s\n", value, a.name.c_str());
            else printf("%s ^ %s\n", name.c_str(), a.name.c_str());
            return IntElement(a.regPos == 0 ? a.name : name, value, 0, 0, 0);
        }

        friend IntElement Equal(IntElement& a, IntElement& b) {
            if (a.flag) assert(0); // Compile Error if the a is immediate value
            if (b.flag) printf("%s = %d\n", a.name.c_str(), b.value);
            else printf("%s = %s\n", a.name.c_str(), b.name.c_str());
            return IntElement(b.regPos == 0 ? b.name : a.name, b.value, 0, 0, 0);
        }
        int getRegPos() { return regPos; }
        void setRegPos(int pos) { regPos = pos; }
        string getName() {return name;}
};

class ArgIntElement: IntElement {
    private:
        int argidx;
    public:
        ArgIntElement(int idx, string n, int v = 0, int stackPos = 0, int f = 1): IntElement(n, v, stackPos, f){
            argidx = idx;
        }
};

class FunctionElement {
    private:
        string name;
        vector<PSS> args;
        string codes;
        string return_type;
    public:
        FunctionElement(string n, vector<PSS> a, string c, string rt) {
            name = n;
            args = a;
            codes = c;
            return_type = rt;
        }
        template<class T, class U>
        T Exec(vector<U>& temp) {
            // TODO: complete the mips code
        }
};


int id = 0;
int valid = 0;
int globalStackPos = 0;
const IntElement zero = {"zero", 0, 0, 1};
int used[2] = {0, 0};
unordered_map<int, IntElement> IntElementMap; 

// Function 
// TODO: complete the mips code

template<class T>
void allocStack() {
    printf("addiu $sp, $sp, -%d\n", T::getNeedMemory());
    globalStackPos += T::getNeedMemory();
}

template<class T>
void freeStack() {
    printf("addiu $sp, $sp, %d\n", T::getNeedMemory());
    globalStackPos -= T::getNeedMemory();
}

// template<class T>
// void allocReg(T& a) {
//     if (a.flag != 1 && a.getRegPos() == -1) {
//         if (used[0]) a.setRegPos(1);
//         else if (used[1]) a.setRegPos(0);
//         else assert(0); // Compile Error if the reg is not enough
//         printf("lw $t%d, %d($fp)\n", a.regPos, a.stackPos);
//     }
// }

template<class T>
void freeReg(T& a) {
    used[a.getRegPos()] = 0;
    a.setRegPos(-1);
}

string RemoveBegSpaceAndEndSpace(string s);

void end();

void CompileTheCode(string s);

void CompileTheExpression(string s);

string Regular(string s);

FunctionElement CreateFunctionElement(string name, vector<string>& arglist, string codes, string rt);

void CreateAssignElement(string s);

IntElement CreateIntElement(string name, int flag, int value, int needToInsert);

ArgIntElement CreateArgIntElement(int idx, string name, int value);

void RemoveFunctionElement();
/*
表达式求值
*/
void CreateAssignElement(string s) {
    
    s = RemoveBegSpaceAndEndSpace(s);
    debug("AssignElement: ", s);
    unordered_map<string, int> Sin = { 
        {"(", 1}, {")", 12}, 
        {"*", 11}, {"%", 11}, {"/", 11}, 
        {"+", 9}, {"-", 9}, 
        {"<=", 7}, {">=", 7}, {"<", 7}, {">", 7}, {"==", 7},
        {"&", 5}, {"^", 5}, {"|", 5},
        {"=", 3}};
    unordered_map<string, int> Sout = {
        {"(", 12}, {")", 1},
         {"*", 10}, {"%", 10}, {"/", 10}, 
        {"+", 8}, {"-", 8},
         {"<=", 6}, {">=", 6}, {"<", 6}, {">", 6}, {"==", 6},
         {"&", 4}, {"^", 4}, {"|", 4},
         {"=", 2}};
    
    stack<IntElement> num; stack<string> op;
    int n = s.size();
    auto cal = [&]() {
        string c = op.top(); op.pop();
        if (c == "(") return;
        IntElement b = num.top(); num.pop();
        IntElement a = num.top(); num.pop();
        // allocReg(a);
        // allocReg(b);
        IntElement temp = zero;
        debug(a.getName(), c, b.getName());
        if (c == "+") temp = a + b;
        else if (c == "-") temp = a - b;
        else if (c == "*") temp = a * b;
        else if (c == "/") temp = a / b;
        else if (c == "%") temp = a % b;
        else if (c == "&") temp = a & b;
        else if (c == "|") temp = a | b;
        else if (c == "^") temp = a ^ b;
        else if (c == "<") temp = a < b;
        else if (c == ">") temp = a > b;
        else if (c == "<=") temp = a <= b;
        else if (c == ">=") temp = a >= b;
        else if (c == "==") temp = a == b;
        else if (c == "=") temp = Equal(a, b);
        else assert(0);
        if (a.getRegPos()) freeReg(a);
        else if (b.getRegPos()) freeReg(b);
        num.push(temp);
    };
    // vector<string> opName = {"+", "-", "*", "/", "%", 
    // "<", ">", "<=", ">=", "==", 
    // "&", "|", "^"};
    string opName = "+-*/%<>=&|^";
	for (int i = 0; i < n; i++) {
        if (s[i] == ' ') continue;
        if (isdigit(s[i])) {
            int res = s[i] - '0';
            int j = i + 1;
            while (j < n && isdigit(s[j])) {
                res = res * 10 + s[j ++] - '0';
            }
            num.push(CreateIntElement(to_string(res), 1, res, 0));
            i = j - 1;
        } else if (isalpha(s[i]) || s[i] == '_') {
            int j = i + 1;
            string temp = "";
            temp += s[i];
            while (j < n && (isalpha(s[j]) || isdigit(s[j]) || s[j] == '_')) temp += s[j ++];
            ElementID Eid = GlobalTrie.Find(temp);
            if (Eid.getType() == 1) {
                num.push(IntElementMap[Eid.getId()]);
            } else {
                assert(0); // Compile Error if the variable is not exist
            }
            i = j - 1;
        } else if (opName.find(s[i]) != opName.npos) {
            int j = i + 1;
            string temp = "";
            temp += s[i];
            while (j < n && opName.find(s[j]) != opName.npos) temp += s[j ++];
            while (op.size() && Sin[temp] <= Sout[op.top()]) cal();
            op.push(temp);
            i = j - 1;
        } else if (s[i] == ')') {
            while (op.size() && op.top() != "(") cal();
            op.pop();
        } else assert(0); // Compile Error if the expression is not exist
    }
	while (op.size()) {
        cal();
    }
    IntElement res = num.top();
    num.pop();
    // freeReg(res);
}

string RemoveBegSpaceAndEndSpace(string s) {
    int len = s.size();
    int beg = 0;
    while (beg < len && s[beg] == ' ') beg ++;
    int end = len - 1;
    while (end >= 0 && s[end] == ' ') end --;
    return s.substr(beg, end - beg + 1);
}

void RemoveFunctionElement() {
    while (VariableName.size()) {
        ElementID Eid = GlobalTrie.Find(VariableName.top());
        // system("pause");
        if (Eid.getValidSpace() == valid) {
            GlobalTrie.Remove(VariableName.top());
            VariableName.pop();
            if (Eid.getType() == 1) {
                IntElementMap.erase(id);
                freeStack<IntElement>();
            }
            else if (Eid.getType() == 3) freeStack<IntElement>();
            id --;
        } else break;
    }
    valid --;
}

ArgIntElement CreateArgIntElement(int idx, string name, int value) {
    ElementID Eid = ElementID(id ++, 3, valid);
    GlobalTrie.Insert(name, Eid);
    return ArgIntElement(idx, name, value);
}

FunctionElement CreateFunctionElement(string name, vector<string>& arglist, string codes, string rt = "int") {
    printf("%s:\n", name.c_str());
    vector<PSS> args;
    for (int i = 0; i < arglist.size(); i ++) {
        string arg = arglist[i];
        assert(arg.size() != 0);
        int pos = arg.find(' ');
        string type = RemoveBegSpaceAndEndSpace(arg.substr(0, pos));
        string name = RemoveBegSpaceAndEndSpace(arg.substr(pos + 1));
        args.emplace_back(type, name);
    }
    ElementID functionId = ElementID(id ++, 2, valid);
    GlobalTrie.Insert(name, functionId);
    valid ++;
    for (int i = 0; i < args.size(); i ++) {
        auto [type, name] = args[i];
        if (type == "int") {
            CreateArgIntElement(i, name, 0);
        }
        else assert(0); // Compile Error if the type is not Exist
    }
    valid ++;
    CompileTheCode(codes);
    RemoveFunctionElement(); // Remove the function elements
    RemoveFunctionElement(); // Remove the arg elements
    return FunctionElement(name, args, codes, rt);
}

/*
name: variable name
valid: valid space
id: varialble id
flag: 1: immediate value, 0: stack value
value: immediate value
needToInsert: 1: need to insert into the trie, 0: no need to insert into the trie
*/
IntElement CreateIntElement(string name, int flag = 0, int value = 0, int needToInsert = 1) {
    if (flag) {
        return {name, value, globalStackPos, 1};
    } else {
        IntElement res(name, value, globalStackPos, 0);
        if (needToInsert) {
            IntElementMap[id] = res;
            ElementID Eid = {id ++, 1, valid};
            GlobalTrie.Insert(name, Eid);
        }
        allocStack<IntElement>();
        return res;
    }
}


void end() {
    // printf("move $a0, $v0\n");
    // printf("li $v0, 1\n");
    // printf("syscall\n");
    // printf("li $v0, 10\n");
    // printf("syscall");
}

void beg() {
    // printf(".text\n");
    // printf(".globl main\n");
    // printf("main:\n");
    // printf("move $fp, $sp\n");
    // printf("addiu $sp, $sp, -0x100\n");
}

string Regular(string s) {
    int len = s.size();
    string res;
    for (int i = 0; i < len; i ++) {
        if (s[i] == '\n') res += ' ';
        else if (s[i] == '\t') res += ' ';
        else if (s[i] == ' ') {
            int k = i + 1;
            while (k < len && s[k] == ' ') k++;
            i = k - 1;
            res += ' ';
        } else if (s[i] == '}' && (i + 1 == len || s[i + 1] != ';')) res += "};";
        else res += s[i];
    }
    assert(res.back() == ';'); // Compile Error if the last char is not ';'
    return res;
}

void CompileTheCode(string s) {
    // beg();
    // system("pause");
    s = RemoveBegSpaceAndEndSpace(s);
    if (s.size() == 0) return;
    vector<int> Brackets(3);
    unordered_map<char, int> BracketsMap = {{'{', 0}, {'(', 1}, {'[', 2}, {'}', 0}, {')', 1}, {']', 2}};
    for (int i = 0; i < s.size(); i ++) {
        int l = i;
        while (s[i] != ';' || (Brackets[0] || Brackets[1] || Brackets[2])) {
            if (s[i] == '{' || s[i] == '(' || s[i] == '[') Brackets[BracketsMap[s[i]]] ++;
            else if (s[i] == '}' || s[i] == ')' || s[i] == ']') Brackets[BracketsMap[s[i]]] --;
            assert(Brackets[0] >= 0 && Brackets[1] >= 0 && Brackets[2] >= 0); // Compile Error if the brackets are not matched
            i ++;
        }
        // system("pause");
        if (i != l) {
            CompileTheExpression(s.substr(l, i - l));
        }
    }
    // end();
}

void CompileTheExpression(string s) {
    s = RemoveBegSpaceAndEndSpace(s);
    // system("pause");
    regex FunctionExpression("\\s*(int|void)\\s*([a-z_A-Z][a-z_A-Z0-9]*)\\s*\\((.*)\\)\\s*\\{(.*)\\}");
    regex DefineExpression("\\s*(int)\\s*([a-z_A-Z][a-z_A-Z0-9]*)");
    regex AssignExpression("\\s*([a-z_A-Z][a-z_A-Z0-9]*)\\s*=\\s*(.*)");
    smatch sm;
    if (regex_match(s, sm, FunctionExpression)) { // Compile the Function
        string type = sm[1];
        string name = sm[2];
        string args = sm[3];
        vector<string> arglist;
        args = RemoveBegSpaceAndEndSpace(args);
        int argslen = args.size();
        for (int i = 0; i < argslen; i ++) {
            int l = i;
            while (i < argslen && args[i] != ',') i ++;
            assert(i != l);
            string arg = RemoveBegSpaceAndEndSpace(args.substr(l, i - l));
            if (arg.size()) arglist.push_back(arg);
        }
        string codes = sm[4];
        CreateFunctionElement(name, arglist, codes, type);
    } else if (regex_match(s, sm, DefineExpression)) {
        string type = sm[1];
        string name = sm[2];
        if (type == "int") CreateIntElement(name);
        else assert(0); // Compile Error if the type is not Exist
    } else if (regex_match(s, sm, AssignExpression)) {
        CreateAssignElement(s);
    }
        
}

int main(int argc, char* argv[]) {
    // assert(argc > 1);
    // fstream fs;
    // fs.open(argv[1], ios::in);
    // assert(fs.is_open());
    // string code;
    // string temp;
    // while (getline(fs, temp)) {
    //     code += temp;
    // }
    // fs.close();
    // CompileTheCode(Regular(code), 0);
    // return 0;
    CompileTheCode(Regular("int main() {int a; int b; a = 1; b = 2; int c; c = a + 1 + 2 * 3 + 100;}"));
    RemoveFunctionElement();
}
