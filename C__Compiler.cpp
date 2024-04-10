#include "Compiler.h"
#include <fstream>

void compileTheCode(std::string s, int valid) {

}

std::string regular(std::string code) {
    
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