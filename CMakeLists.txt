cmake_minimum_required(VERSION 3.7)
project(ex3)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -pedantic-errors -g -Werror -DNDEBUG")

set(SOURCE_FILES
        list_test.c
        list_mtm1/list_mtm1.c
        list_mtm1/list_mtm1.h
        tests/list_mtm/test_utilities.h
        Escaper.h
        Escaper.c
        EscapeTechnion.c
        EscapeTechnion.h
        Order.h
        Order.c
        Company.c
        Company.h
        Room.h
        Room.c
        list.h
        set.h
        mtm_ex3.h)

add_executable(ex3 ${SOURCE_FILES})
target_link_libraries(ex3 ${CMAKE_SOURCE_DIR}//libmtm/win64/libmtm.a)