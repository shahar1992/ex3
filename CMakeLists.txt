cmake_minimum_required(VERSION 3.7)
project(ex3)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

set(SOURCE_FILES
        Company.h
        Company.c
        Room.h
        Room.c
        Escaper.c
        Escaper.h
        Order.c
        Order.h
        EscapeTechnion.h
        EscapeTechnion.c
        tests/sys_tests.c)

add_executable(ex3 ${SOURCE_FILES})
target_link_libraries(ex3 ${CMAKE_SOURCE_DIR}//libmtm/win32/libmtm.a)