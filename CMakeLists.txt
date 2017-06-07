cmake_minimum_required(VERSION 3.7)
project(ex3)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

set(SOURCE_FILES
        tests/list_mtm/test_utilities.h
        tests/Order_tests.c
        Order.c
        Order.h
        Escaper.h
        Escaper.c
        Room.h
        Room.c
        )

add_executable(ex3 ${SOURCE_FILES})
target_link_libraries(ex3 ${CMAKE_SOURCE_DIR}//libmtm/win64/libmtm.a)