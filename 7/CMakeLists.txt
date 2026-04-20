cmake_minimum_required(VERSION 3.29)
project(syspro_cpp)

set(CMAKE_CXX_STANDARD 17)

add_executable(syspro_cpp
        src/main.cpp
        src/emulator.cpp
        src/Jmpz.cpp
        src/Mov.cpp
        src/Add.cpp
        src/Sub.cpp
        src/Mul.cpp
        src/Div.cpp
        src/Load.cpp
        src/Store.cpp
        src/Jmp.cpp)

target_include_directories(syspro_cpp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
