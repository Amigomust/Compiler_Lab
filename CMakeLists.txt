cmake_minimum_required(VERSION 3.10)  # 指定 CMake 的最低版本要求
project(cminusminus VERSION 1.1)  # 设置项目名称和版本号

# 添加可执行文件
add_executable(cminusminus 
    csubsub.cpp      # 主程序入口文件
    fun_obj.cpp     # 其他 CPP 文件
    int_obj.cpp
    memory.cpp
    object.cpp
    compiler.cpp
    # 添加更多的 CPP 文件
)

# 添加包含目录
target_include_directories(cminusminus PRIVATE include)  # 包含头文件目录

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)  # 设置 C++ 标准
set(CMAKE_CXX_STANDARD_REQUIRED True)  # 设置标准为必须

# 生成可执行文件
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)  # 设置可执行文件输出路径
