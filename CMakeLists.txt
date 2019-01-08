cmake_minimum_required(VERSION 3.10)
project(Symbol_Table C)

set(CMAKE_C_STANDARD 11)

add_executable(Symbol_Table src/test.c src/symtable.h src/symtable.c)