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