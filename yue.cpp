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

using ui = unsigned int;

using namespace std;
int valnum = 0;

/*
function: 2
int: 1
special: 0
*/
class Trie {
    private:
        struct Node {
            unordered_map<char, int> next;
            stack<ui> val;
        };
        vector<Node> nodes;
    public:
        void Insert(string s, int v) {
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
            nodes[beg].val.push(v);
        }
        Trie() {
            nodes.push_back(Node());
        }
        int Find(string& s, int valid) {
            int beg = 0;
            for (int i = 0; i < s.size(); i ++) {
                unordered_map<char, int>& next = nodes[beg].next;
                unordered_map<char, int>::iterator it = next.find(s[i]);
                if (it == next.end()) return -1;
                beg = it->second;
            }
            if (nodes[beg].val.empty() || (nodes[beg].val.top() & VALMASK) != valid) return -1;
            return nodes[beg].val.top() & (IDMASK | TYPEMASK);
        }
} GlobalTrie, SpecialTrie, CommonTrie;

class SpecialName {
    private:
        string name;
        SpecialName(string n) {
            name = n;
        }
};

class Element {
    protected:
        int stackPos;
        size_t needMemory;
    public:
        Element(int pos = 0, size_t need = 0) {
            stackPos = pos;
            needMemory = need;
        }
};

class IntElement: Element{
    private:
        int value;
        int flag;
    public:
        IntElement(int v, int stackPos, int f = 0) {
            value = v;
            stackPos = stackPos;
            f = flag;
            needMemory = 4;
            // TODO: complete the mips code
        }
        IntElement(IntElement& a) {
            value = a.value;
            stackPos = a.stackPos;
            flag = a.flag;
            needMemory = 4;
            // TODO: complete the mips code
        }
        ~IntElement() {
            // TODO: complete the mips code
        }
        IntElement operator+(const IntElement& a) const {
            // TODO: complete the mips code
            return IntElement(value + a.value, stackPos);
        }
        IntElement operator-(const IntElement& a) const {
            // TODO: complete the mips code
            return IntElement(value - a.value, stackPos);
        }
        IntElement operator*(const IntElement& a) const {
            // TODO: complete the mips code
            return IntElement(value * a.value, stackPos);
        }
        IntElement operator/(const IntElement& a) const {
            // TODO: complete the mips code
            return IntElement(value / a.value, stackPos);
        }
        IntElement operator%(const IntElement& a) const {
            // TODO: complete the mips code
            return IntElement(value % a.value, stackPos);
        }
        IntElement operator<(const IntElement& a) const {
            // TODO: complete the mips code
            return IntElement(value < a.value, stackPos);
        }
        IntElement operator<=(const IntElement& a) const {
            // TODO: complete the mips code
            return IntElement(value <= a.value, stackPos);
        }
        IntElement operator>(const IntElement& a) const {
            // TODO: complete the mips code
            return IntElement(value > a.value, stackPos);
        }
        IntElement operator>=(const IntElement& a) const {
            // TODO: complete the mips code
            return IntElement(value >= a.value, stackPos);
        }
        IntElement operator==(const IntElement& a) const {
            // TODO: complete the mips code
            return IntElement(value == a.value, stackPos);
        }
        IntElement operator!=(const IntElement& a) const {
            // TODO: complete the mips code
            return IntElement(value != a.value, stackPos);
        }
        IntElement operator&(const IntElement& a) const {
            // TODO: complete the mips code
            return IntElement(value & a.value, stackPos);
        }
        IntElement operator|(const IntElement& a) const {
            // TODO: complete the mips code
            return IntElement(value | a.value, stackPos);
        }
        IntElement operator^(const IntElement& a) const {
            // TODO: complete the mips code
            return IntElement(value ^ a.value, stackPos);
        }
};

class FunctionElement {
    private:
        string name;
        vector<string> args;
        vector<string> types;
        string codes;
        string return_type;
    public:
        FunctionElement(string n, vector<string> a, vector<string> v, string c, string rt) {
            name = n;
            args = a;
            types = v;
            codes = c;
            return_type = rt;
        }
        template<class T, class U>
        T Exec(vector<U>& temp) {
            // TODO: complete the mips code
        }
};

// TODO: complete the mips code
template<typename T>
void allocStack();

template<typename T>
void freeStack();

/*
表达式求值
*/
template<typename T>
T calExpre(string& s) {
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
    
    stack<T> num; stack<string> op;
    int tempValNum = 0;
    int n = s.size();

    auto cal = [&num, &op, &tempValNum]() {
        string c = op.top(); op.pop();
        if (c == "(") return;
        T a = num.top(); num.pop();
        T b = num.top(); num.pop();
        T temp;
        tempValNum ++;
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
        else assert(0);
        
        num.push(temp);
        
    };
    vector<string> opName = {"+", "-", "*", "/", "%", 
    "<", ">", "<=", ">=", "==", 
    "&", "|", "^"};

	for (int i = 0; i < n; i++)
		if ('0' <= s[i] && s[i] <= '9') {
			int k = i + 1;
			int res = s[i] - '0';
			while (k < n && '0' <= s[k] && s[k] <= '9')
				res = res * 10 + (s[k ++] - '0');
			num.push(res);
			i = k - 1;
		} 
		else if (s[i] == ')') {
			while (op.size() && op.top() != '(') cal();
			op.pop();
		}
		else {
			while (op.size() && Sin[op.top()] > Sout[s[i]]) cal();
			op.push(s[i]);
		}
    
	while (op.size()) cal();

	return num.top();
}

string rmBegSpaceAndEndSpace(string s) {
    int len = s.size();
    int beg = 0;
    while (beg < len && s[beg] == ' ') beg ++;
    int end = len - 1;
    while (end >= 0 && s[end] == ' ') end --;
    return s.substr(beg, end - beg + 1);
}

FunctionElement createFunctionElement(string name, vector<string>& arglist, string codes, string rt = "int") {
    vector<string> args;
    vector<string> types;
    for (int i = 0; i < arglist.size(); i ++) {
        string arg = arglist[i];
        assert(arg.size() != 0);
        int pos = arg.find(' ');
        string type = rmBegSpaceAndEndSpace(arg.substr(0, pos));
        string temp = rmBegSpaceAndEndSpace(arg.substr(pos + 1));
        
        args.push_back(name);
        types.push_back(type);
    }
    return FunctionElement(name, args, types, codes, rt);
}

void Expre(string s = "int main(int a, int b, int c) {};") {
    regex functionReg("(int|void)\\s\\s*([a-z_A-Z][a-zA-Z0-9_]*)\\s*\\((.*)\\)\\s*(\\{(.*)\\};)");
    regex varReg("(int)\\s\\s*([a-zA-Z_][a-z_A-Z0-9]*)\\s*;");
    regex assignReg("([a-zA-Z_][a-z_A-Z0-9]*)\\s*(=)\\s*(.*);");
    if (regex_match(s, functionReg)) {
        smatch sm;
        regex_match(s, sm, functionReg);
        string type = rmBegSpaceAndEndSpace(sm[1]);
        string name = rmBegSpaceAndEndSpace(sm[2]);
        string args = sm[3];
        string codes = sm[4];
        regex commaReg(",");
        sregex_token_iterator iter(args.begin(), args.end(), commaReg, -1);
        sregex_token_iterator end;
        vector<string> argList(iter, end);
        FunctionElement func = createFunctionElement(name, argList, codes, type);
    } else if (regex_match(s, varReg)) {

    } else if (regex_match(s, assignReg)) {

    } else {
        assert(0);
    }
}

void end() {
    printf("move $a0, $v0\n");
    printf("li $v0, 1\n");
    printf("syscall\n");
    printf("li $v0, 10\n");
    printf("syscall");
}

string Regular(string& s) {
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
        } else if (s[i] == '}' && i + 1 < len && s[i + 1] != ';') res += "};";
        else res += s[i];
    }
    assert(res.back() == ';'); // Compile Error if the last char is not ';'
    return res;
}


int main(int argc, char* argv[]) {
    // assert(argc > 1);
    // fstream fs;
    // fs.open(argv[1], ios::in);
    // assert(fs.is_open());
    // printf(".text\n");
    // printf(".globl main\n");
    // printf("main:\n");
    // printf("move $fp, $sp\n");
    // printf("addiu $sp, $sp, -0x100\n");
    // string code;
    // string temp;
    // while (getline(fs, temp)) {
    //     code += temp;
    // }
    // fs.close();
    // end();
    // string new_code = Regular(code);
    // return 0;
    Expre();
}
