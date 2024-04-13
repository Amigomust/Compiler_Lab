# 定义编译器
CXX = g++

# 定义编译选项
CXXFLAGS = -std=c++17 -Wall -DLOCAL
# 存储$(wildcard obj/*.o)的变量
SRCS_OBJ_FILES = $(wildcard obj/*.cpp)
SRCS_NATIVE_FILES = $(wildcard native/*.cpp)
SRCS_MIPS_FILES = $(wildcard mips/*.cpp)

all: compiler

# echo:
# 	@echo $(wildcard obj/*)
# 	@echo $(wildcard native/*)
# 	@echo $(wildcard mips/*)

compiler: compiler.h compiler.cpp csubsub.cpp
	$(CXX) $(CXXFLAGS) -o csubsub $(SRCS_OBJ_FILES) compiler.cpp csubsub.cpp
