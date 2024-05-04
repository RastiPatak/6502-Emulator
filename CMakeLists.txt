cmake_minimum_required(VERSION 3.28)
project(6502_Emulator)

set(CMAKE_CXX_STANDARD 20)

add_executable(6502_Emulator
        Uncem_6502/Main.cpp
        Uncem_6502/Config.cpp
        Uncem_6502/Config.hpp)