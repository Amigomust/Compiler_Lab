#include "Compiler.h"
#include <fstream>
#include <unordered_map>

std::string rmBegEndSpace(std::string s) {
    int len = s.size();
    int beg = 0;
    while (beg < len && s[beg] == ' ') beg ++;
    int end = len - 1;
    while (end >= 0 && s[end] == ' ') end --;
    return s.substr(beg, end - beg + 1);
}

void compileTheExpression(std::string s, int valid) {
    
}

void compileTheCode(std::string s, int valid) {
    // s = RemoveBegSpaceAndEndSpace(s);
    if (s.size() == 0) return;
    std::vector<int> Brackets(3);
    std::unordered_map<char, int> BracketsMap = {{'{', 0}, {'(', 1}, {'[', 2}, {'}', 0}, {')', 1}, {']', 2}};
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
            compileTheExpression(s.substr(l, i - l), valid);
        }
    }
}

std::string regular(std::string s) {
    int len = s.size();
    std::string res;
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

int main(int argc, char* argv[]) {
    assert(argc > 1);
    std::fstream fs;
    fs.open(argv[1], std::ios::in);
    assert(fs.is_open());
    std::string code;
    std::string temp;
    while (getline(fs, temp)) {
        code += temp;
    }
    fs.close();
    compileTheCode(regular(code), 0);
    return 0;
}