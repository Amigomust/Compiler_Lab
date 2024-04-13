#include <fstream>
#include "compiler.h"

int main(int argc, char* argv[]) {
    assert(argc > 1);
    std::fstream fs;
    fs.open(argv[1], std::ios::in);
    assert(fs.is_open());
    std::string code(std::istreambuf_iterator<char>(fs), {});
    code.erase(std::remove_if(code.begin(), code.end(), [](char c) {
        return c == '\t' || c == '\n' || c == '\r';
    }), code.end());
    printf(".text\n.globl main\n");
    css::define_function("int println_int(int x){}", -1);
    css::compile_code(css::regular(code), 0);
    return 0;
}