# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# 定义编译选项
CXXFLAGS = -std=c++17 -Wall
# 存储$(wildcard obj/*.o)的变量
SRCS_OBJ_FILES = fun_obj.cpp int_obj.cpp memory.cpp object.cpp
SRCS_NATIVE_FILES = 
SRCS_MIPS_FILES = 

all: obj compiler csubsub

#=============================================================================
# Special targets provided by cmake.

obj: $(SRCS_OBJ_FILES) $(SRCS_NATIVE_FILES) $(SRCS_MIPS_FILES)
	$(CXX) $(CXXFLAGS) -c fun_obj.cpp int_obj.cpp memory.cpp object.cpp

compiler: compiler.h compiler.cpp
	$(CXX) $(CXXFLAGS) -c compiler.cpp

csubsub: compiler.cpp.o csubsub.cpp fun_obj.cpp.o int_obj.cpp.o memory.cpp.o object.cpp.o
	$(CXX) $(CXXFLAGS) -o csubsub compiler.cpp.o csubsub.cpp fun_obj.cpp.o int_obj.cpp.o memory.cpp.o object.cpp.o
