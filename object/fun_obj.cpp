#include "fun_obj.h"
#include "int_obj.h"
#include "../mips/utils.h"

namespace obj {
    function_object::function_object(std::string name, int valid, std::vector<int_object*> args): object(name, "function", valid){
        this->name = name;
        this->valid_space = valid;
        this->args = args;
    }
    //TODO: 重构
    int_object* function_object::work(std::vector<int_object*>& obj) {
        mips::system_print((obj[0])->load_reg(4));
        obj[0]->free_reg();
        return new int_object("0", 0, 0, -1);
    }
}