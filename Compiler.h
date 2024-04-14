#pragma once 

#include <iostream>
#include <stack>
#include <map>
#include <vector>
#include <regex>
#include <assert.h>
#include "native/trie.h"
#include "native/short_str.h"
#include "object/object.h"
#include "object/fun_obj.h"
#include "object/int_obj.h"
#include "mips/utils.h"

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
