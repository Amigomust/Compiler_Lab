#ifndef OBJECT
#define OBJECT

#include <string>
#include "utils.h"
#include "short_str.h"
#include <vector>
#include <iostream>
#include <assert.h>

namespace obj {
  class value_object;
  class object;
}
namespace memory {
  extern std::vector<size_t> stk_size;
  extern std::vector<int> global_tp_pos;
  class stack_pos;
  class register_pos;
  register_pos* alloc_reg(obj::value_object* yue);
  register_pos* alloc_reg(obj::value_object* yue, int x);
  void free_reg(register_pos* reg);
  void check_regs();
  void alloc_stack(size_t x, bool is_global = false);
  void free_stack();
  void save_fp();
  void load_fp();
  void move_fp();
}

namespace memory {
class stack_pos {
  private:
    int sPos; // StackPos
    int flag; // flag = 1: Arg flag = 0: Common
  public:
    stack_pos();
    stack_pos(size_t size);
    native::Str16 get_pos(size_t size);
};

class register_pos {
  private:
    static int RegNum;
    static register_pos* regs;

    int rPos;
    
  public:
    obj::value_object* nowUsing;
    register_pos();
    native::Str16 get_pos();
    
    friend void free_reg(register_pos* reg);
    friend void check_regs();
    friend register_pos* alloc_reg(obj::value_object* yue);
    friend register_pos* alloc_reg(obj::value_object* yue, int x);
};

  
} // namespace memory

namespace obj {

class object {
public:
    std::string name;
    std::string type;
    int valid_space;
    object();
    object(std::string name, std::string type, int valid_space);
    object(const object& other);
};

// 一开始不需要分配reg，只分配stk
class value_object: public object {
    private:
    memory::stack_pos stk_pos;
    memory::register_pos* reg_pos;
    public:
    value_object(const value_object& other);
    value_object(std::string name, std::string type, int valid_space);
    value_object();
    native::Str16 load_reg();
    native::Str16 load_reg(int x);
    virtual void store2reg();
    void free_reg();
    void save_reg(native::Str16 stk);
    native::Str16 load_stk();
    friend class memory::register_pos;
    friend class memory::stack_pos;
};

} // namespace obj

#endif