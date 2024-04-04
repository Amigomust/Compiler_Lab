#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <map>
#include <assert.h>
#include <string.h>
#include <unordered_map>
#include <regex>

#include "debug.h"

#define IDMASK 0x0000ffff
#define TYPEMASK 0x00ff0000
#define VALMASK 0xff000000

using namespace std;

using PSS = pair<string, string>;

/*
function: 2
int: 1
special: 0
intargs: 3
*/

// Class Definition
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
            for (int i = 0; i < s.size(); i ++) {
                unordered_map<char, int>& next = nodes[beg].next;
                unordered_map<char, int>::iterator it = next.find(s[i]);
                if (it != next.end()) beg = it -> second;
                else {
                    nodes.push_back(Node{});
                    next[s[i]] = nodes.size();
                    beg = nodes.size();
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

        ElementID Find(string& s, int valid) {
            int beg = 0;
            for (int i = 0; i < s.size(); i ++) {
                unordered_map<char, int>& next = nodes[beg].next;
                unordered_map<char, int>::iterator it = next.find(s[i]);
                if (it == next.end()) return -1;
                beg = it->second;
            }
            if (nodes[beg].value.empty() || (nodes[beg].value.top().getValidSpace()) != valid) return -1;
            return nodes[beg].value.top();
        }
} GlobalTrie, SpecialTrie, CommonTrie;

class Element {
    protected:
        int stackPos;
    public:
        Element(int pos = 0) {
            stackPos = pos;
        }
};

// flag = 1: immediate value, flag = 0: stack value
class IntElement: Element{
    private:
        string name;
        int value;
        int flag;
    public:
        IntElement(string n, int v, int stackPos, int f = 0) {
            name = n;
            value = v;
            stackPos = stackPos;
            f = flag;
        }
        static size_t getNeedMemory() { return 4; }
        friend void Add(const IntElement& a, const IntElement& b, IntElement& c) {
            // TODO: complete the mips code
            if (a.flag && b.flag) c = IntElement(c.name, a.value + b.value, 0, 1);
            else if (a.flag) {
                // c = b + immediate value
            } else if (b.flag) {
                // c = a + immediate value
            } else {
                // c = a + b
            }
        }
        friend IntElement Sub(const IntElement& a, const IntElement& b, IntElement& c) {
            // TODO: complete the mips code
            return IntElement(c.name, 0, c.stackPos);
        }
        friend IntElement Mul(const IntElement& a, const IntElement& b, IntElement& c) {
            // TODO: complete the mips code
            return IntElement(c.name, a.value * b.value, c.stackPos);
        }
        friend IntElement Div(const IntElement& a, const IntElement& b, IntElement& c) {
            // TODO: complete the mips code
            return IntElement(c.name, 0, c.stackPos);
        }
        friend IntElement Mod(const IntElement& a, const IntElement& b, IntElement& c) {
            // TODO: complete the mips code
            return IntElement(c.name, 0, c.stackPos);
        }
        friend IntElement Less(const IntElement& a, const IntElement& b, IntElement& c) {
            // TODO: complete the mips code
            return IntElement(c.name, 0, c.stackPos);
        }
        friend IntElement LessEqual(const IntElement& a, const IntElement& b, IntElement& c) {
            // TODO: complete the mips code
            return IntElement(c.name, 0, c.stackPos);
        }
        friend IntElement Greater(const IntElement& a, const IntElement& b, IntElement& c) {
            // TODO: complete the mips code
            return IntElement(c.name, 0, c.stackPos);
        }
        friend IntElement GreaterEqual(const IntElement& a, const IntElement& b, IntElement& c) {
            // TODO: complete the mips code
            return IntElement(c.name, 0, c.stackPos);
        }
        friend IntElement Equal(const IntElement& a, const IntElement& b, IntElement& c) {
            // TODO: complete the mips code
            return IntElement(c.name, 0, c.stackPos);
        }
        friend IntElement NotEqual(const IntElement& a, const IntElement& b, IntElement& c) {
            // TODO: complete the mips code
            return IntElement(c.name, 0, c.stackPos);
        }
        friend IntElement And(const IntElement& a, const IntElement& b, IntElement& c) {
            // TODO: complete the mips code
            return IntElement(c.name, 0, c.stackPos);
        }
        friend IntElement Or(const IntElement& a, const IntElement& b, IntElement& c) {
            // TODO: complete the mips code
            return IntElement(c.name, 0, c.stackPos);
        }
        friend IntElement Xor(const IntElement& a, const IntElement& b, IntElement& c) {
            // TODO: complete the mips code
            return IntElement(c.name, 0, c.stackPos);
        }
        friend void Assign(const IntElement a, const IntElement b) {
            return;
        }
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

stack<string> VariableName;
int id = 0;
int valid = 0;
int globalStackPos = 0;
const IntElement zero = {"zero", 0, 0, 1};

// Function 
// TODO: complete the mips code

template<class T>
void allocStack() {
    printf("addiu $sp, $sp, -%d\n", T.getNeedMemory());
    globalStackPos += T.getNeedMemory();
}

template<class T>
void freeStack() {
    printf("addiu $sp, $sp, %d\n", T.getNeedMemory());
    globalStackPos -= T.getNeedMemory();
}

string RemoveBegSpaceAndEndSpace(string s);

void end();

void CompileTheCode(string s);

void CompileTheExpression(string s);

string Regular(string s);

FunctionElement CreateFunctionElement(string name, vector<string>& arglist, string codes, string rt = "int");

void CreateAssignElement(string s);

IntElement CreateIntElement(string name, int flag = 0, int value = 0, int needToInsert = 1);

ArgIntElement CreateArgIntElement(int idx, string name, int value = 0);

void RemoveFunctionElement();
/*
表达式求值
*/
void CreateAssignElement(string s) {
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

    auto cal = [&num, &op]() {
        string c = op.top(); op.pop();
        if (c == "(") return;
        IntElement a = num.top(); num.pop();
        IntElement b = num.top(); num.pop();
        IntElement temp = CreateIntElement("", 0, 0, 0);
        Assign(temp, zero);
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
        else if (c == "=") temp = a = b;
        else assert(0);
        
        num.push(temp);
        
    };
    vector<string> opName = {"+", "-", "*", "/", "%", 
    "<", ">", "<=", ">=", "==", 
    "&", "|", "^"};

	// for (int i = 0; i < n; i++)
	// 	if ('0' <= s[i] && s[i] <= '9') {
	// 		int k = i + 1;
	// 		int res = s[i] - '0';
	// 		while (k < n && '0' <= s[k] && s[k] <= '9')
	// 			res = res * 10 + (s[k ++] - '0');
	// 		num.push(res);
	// 		i = k - 1;
	// 	} 
	// 	else if (s[i] == ')') {
	// 		while (op.size() && op.top() != '(') cal();
	// 		op.pop();
	// 	}
	// 	else {
	// 		while (op.size() && Sin[op.top()] > Sout[s[i]]) cal();
	// 		op.push(s[i]);
	// 	}
    
	while (op.size()) cal();
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
    while (1) {
        ElementID Eid = GlobalTrie.Find(VariableName.top(), valid);
        if (Eid.getValidSpace() == valid) {
            GlobalTrie.Remove(VariableName.top());
            VariableName.pop();
            id --;
        } else break;
    }
    valid --;
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
        if (needToInsert) {
            ElementID Eid = {id ++, 1, valid};
            GlobalTrie.Insert(name, Eid);
        }
        allocStack<IntElement>();
        return {name, value, globalStackPos, 0};
    }
}


void end() {
    printf("move $a0, $v0\n");
    printf("li $v0, 1\n");
    printf("syscall\n");
    printf("li $v0, 10\n");
    printf("syscall");
}

void beg() {
    printf(".text\n");
    printf(".globl main\n");
    printf("main:\n");
    printf("move $fp, $sp\n");
    printf("addiu $sp, $sp, -0x100\n");
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
    debug(res);
    assert(res.back() == ';'); // Compile Error if the last char is not ';'
    return res;
}

void CompileTheCode(string s) {
    // beg();
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
        
        if (i != l) {
            debug(s.substr(l, i - l));
            CompileTheExpression(s.substr(l, i - l));
        }
    }
    // end();
}

void CompileTheExpression(string s) {
    regex FunctionExpression("(int|void)\\s*([a-z_A-Z][a-z_A-Z0-9]*)\\s*\\((.*)\\)\\s*\\{(.*)\\}");
    regex DefineExpression("(int)\\s*([a-z_A-Z][a-z_A-Z0-9]*)");
    regex AssignExpression("([a-z_A-Z][a-z_A-Z0-9]*)\\s*=\\s*(.*)");
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
    } else if (regex_match(s, DefineExpression)) {
        string type = sm[1];
        string name = sm[2];
        if (type == "int") CreateIntElement(name);
        else assert(0); // Compile Error if the type is not Exist
    } else if (regex_match(s, AssignExpression)) {
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
    CompileTheCode(Regular("int main() {int a = 1; int b = 2; int c = a + b;}"));
    RemoveFunctionElement();
}
