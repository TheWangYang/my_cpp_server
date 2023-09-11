# 设置CMake的最低版本要求
cmake_minimum_required(VERSION 3.12)

# 为项目命名
project(mywebserver)

# 设置C++标准（可以根据需要更改）
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 添加可执行文件
add_executable(mywebserver simple_webserver.cpp)

# 可选：添加其他源文件和头文件
# target_sources(myapp PRIVATE
#     src/file1.cpp
#     src/file2.cpp
# )

# 可选：包含头文件搜索路径
# target_include_directories(myapp PRIVATE
#     include
# )

# 可选：链接外部库
# find_package(ws2_32 REQUIRED)
target_link_libraries(mywebserver ws2_32)

# 可选：设置编译选项
# target_compile_options(myapp PRIVATE
#     -Wall
#     -Wextra
# )

# 可选：添加自定义编译定义
# target_compile_definitions(myapp PRIVATE
#     MY_DEFINE
# )

# 可选：安装目标文件
# install(TARGETS myapp
#     DESTINATION bin
# )

# 可选：安装其他文件（例如配置文件）
# install(FILES myconfig.conf
#     DESTINATION etc
# )
