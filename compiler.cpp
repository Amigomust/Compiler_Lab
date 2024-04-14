#include "compiler.h"

namespace css {

#ifdef LOCAL
void pause() {
    int x;
    std::cin >> x;
}
#else
#define pause(...) do {} while (false)
#endif

native::trie global_trie;

void define_return(std::string s, int valid) {
    std::vector<std::string> fun = split(s);
    assert(fun.size() == 2);
    std::string type = fun[0];
    std::string name = fun[1];
    assert(type == "return");
    int n = name.size();
    native::Str16 ans;
    for (int i = 0; i < n; i ++) {
        if (std::isalpha(name[i]) || name[i] == '_') {
            int l = i;
            while (i < n && (std::isalnum(name[i]) || name[i] == '_')) i ++;
            std::string temp = name.substr(l, i - l);
            obj::object* who = global_trie.find(temp);
            if (who->type == "value") {
                obj::int_object* res = (obj::int_object*)who;
                ans = res->load_reg(2);
                res->free_reg();
            } else if (who->type == "function") {
                obj::function_object* fun = (obj::function_object*)who;
                /// TODO: check the expression is correct or not
                obj::int_object* res = work(fun, name, i);
                ans = res->load_reg(2);
                res->free_reg();
            } else assert(0); // Compile Error if the variable is not exist
            break;
        } else if (std::isdigit(name[i])) {
            int res = name[i] - '0';
            int j = i + 1;
            while (j < n && std::isdigit(name[j])) {
                res = res * 10 + name[j ++] - '0';
            }
            obj::int_object yue("res", 0, res, valid + 1);
            ans = yue.load_reg(2);
            break;
        }
    }
    if (valid != -1) {
        memory::free_stack();
    }
    mips::return_value(ans);
}

void define_function(std::string s, int valid) {
    std::vector<std::string> fun = split(s);
    assert(fun.size() == 4);
    std::string type = fun[0];
    std::string name = fun[1];
    if (valid != -1) {
        printf("%s:\n", name.c_str());
        memory::alloc_stack(2048, true);
    }
    std::vector<obj::int_object*> args;
    if (true) args = define_arg(fun[2], valid + 1);
    std::string code = fun[3];
    pause();
    obj::function_object* obj = new obj::function_object(name, valid, args);
    global_trie.insert(obj);
    compile_code(code, valid + 2);
}

void define_int(std::string s, int valid) {
    std::vector<std::string> fun = split(s);
    assert(fun.size() == 2);
    std::string type = fun[0];
    std::string name = fun[1];
    obj::int_object* obj = new obj::int_object(name, true, 0, valid);
    global_trie.insert((obj::object*)obj);
}

std::vector<obj::int_object*> define_arg(std::string s, int valid) {
    return std::vector<obj::int_object*>(1, nullptr);
}

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

obj::int_object* work(obj::function_object * obj, std::string & s, int & beg) {
    while (s[beg] != '(') beg ++;
    beg ++;
    std::vector<std::string> temp;

    // TODO: 封装成一个函数
    while (s[beg] != ')') {
        if (s[beg] == ',' || s[beg] == ' ') {
            beg ++;
            continue;   
        }
        int l = beg;
        while (s[beg] != ',' && s[beg] != ')' && s[beg] != ' ') beg ++;
        temp.push_back(s.substr(l, beg - l));
    }
    

    std::vector<obj::int_object*> args(temp.size(), nullptr);
    assert(temp.size() == obj->args.size());
    for (int i = 0; i < (int)temp.size(); i ++) {
        std::string yue = temp[i];
        int m = yue.size();
        for (int j = 0; j < m; j ++) {
            if (isdigit(yue[j])) {
                int res = yue[j] - '0';
                int k = j + 1;
                while (k < m && isdigit(yue[k])) {
                    res = res * 10 + yue[k ++] - '0';
                }
                args[i] = new obj::int_object(std::to_string(res), 0, res, -1);
                j = k - 1;
            } else if (isalpha(yue[j]) || yue[j] == '_') {
                int k = j + 1;
                std::string temp = "";
                temp += yue[j];
                while (k < m && (isalnum(yue[k]) || yue[k] == '_')) temp += yue[k ++];
                obj::object* who = global_trie.find(temp);
                if (who->type == "value") {
                    obj::int_object* val = (obj::int_object*)who;
                    args[i] = val;
                } else assert(0); // Compile Error if the variable is not exist
                j = k - 1;
            }
        }
    }
    
    return obj->work(args);
}


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

std::string remove_begend_space(std::string s) {
    int len = s.size();
    int beg = 0;
    while (beg < len && s[beg] == ' ') beg ++;
    int end = len - 1;
    while (end >= 0 && s[end] == ' ') end --;
    return s.substr(beg, end - beg + 1);
}

std::string regular(std::string code) {
    int len = code.size();
    std::string res;
    for (int i = 0; i < len; i ++) {
        if (code[i] == '\n') res += ' ';
        else if (code[i] == '\t') res += ' ';
        else if (code[i] == ' ') {
            int k = i + 1;
            while (k < len && code[k] == ' ') k++;
            i = k - 1;
            res += ' ';
        } else if (code[i] == '}' && (i + 1 == len || code[i + 1] != ';')) res += "};";
        else res += code[i];
    }
    assert(res.back() == ';'); // Compile Error if the last char is not ';'
    return res;
}

std::vector<std::string> split(std::string s) {
    int n = s.size();
    std::vector<std::string> yue;
    for (int i = 0; i < n; i ++) {
        if (isalnum(s[i]) || s[i] == '_') {
            std::string temp;
            int j = i + 1;
            temp = s[i];
            while (j < n && (isalnum(s[j]) || s[j] == '_'))
                temp += s[j ++];
            i = j - 1;
            yue.push_back(temp);
        } else if (s[i] == '(' || s[i] == '{') {
            std::vector<int> brackets(3);
            std::unordered_map<char, int> bracket_map = {
                {'{', 0}, {'(', 1}, 
                {'[', 2}, {'}', 0}, 
                {')', 1}, {']', 2}};
            int l = i;
            do {
                if (s[i] == '{' || s[i] == '(' || s[i] == '[') brackets[bracket_map[s[i]]] ++;
                else if (s[i] == '}' || s[i] == ')' || s[i] == ']') brackets[bracket_map[s[i]]] --;
                i++;
            } while (i < n && (brackets[0] || brackets[1] || brackets[2]));
            i --;
            yue.push_back(s.substr(l + 1, i - l - 1));
        } 
    }
    return yue;
}
}