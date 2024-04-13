# 定义编译器
CXX = g++

# 定义编译选项
CXXFLAGS = -std=c++17 -Wall
# 存储$(wildcard obj/*.o)的变量
SRCS_OBJ_FILES = $(wildcard obj/*)
SRCS_NATIVE_FILES = $(wildcard native/*)
SRCS_MIPS_FILES = $(wildcard mips/*)

all: csubsub

# echo:
# 	@echo $(wildcard obj/*)
# 	@echo $(wildcard native/*)
# 	@echo $(wildcard mips/*)

# obj: $(SRCS_OBJ_FILES) $(SRCS_NATIVE_FILES) $(SRCS_MIPS_FILES)
# 	$(CXX) $(CXXFLAGS) -c obj/*.cpp 

# compiler: compiler.h compiler.cpp
# 	$(CXX) $(CXXFLAGS) -c compiler.cpp

csubsub: compiler.cpp csubsub.cpp fun_obj.cpp int_obj.cpp memory.cpp object.cpp
	$(CXX) $(CXXFLAGS) -o cminusminus compiler.cpp csubsub.cpp fun_obj.cpp int_obj.cpp memory.cpp object.cpp
