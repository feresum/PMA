cmake_minimum_required(VERSION 3.1)

project(snails CXX)

add_library(snails_lib STATIC
    snails/ast.h
    snails/compile.h
    snails/compile.cpp
    snails/encoding.h
    snails/encoding.cpp
    snails/execute.h
    snails/execute.cpp
    snails/general.h
    snails/grid.h
    snails/grid.cpp
    snails/instructions.h
    snails/interpreter.h
    snails/interpreter.cpp
    snails/parse.h
    snails/parse.cpp
    snails/pattern.h
    snails/pattern.cpp
    snails/state.h
    snails/state.cpp
    snails/tokens.h
)

add_executable(snails main/main.cpp)
target_link_libraries(snails snails_lib)

add_executable(test_ test/test.cpp)
target_link_libraries(test_ snails_lib)
set_target_properties(test_ PROPERTIES OUTPUT_NAME test)

set_target_properties(snails_lib snails test_ PROPERTIES CXX_STANDARD 11)
