cmake_minimum_required(VERSION 3.7)
project(ex3)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

set(SOURCE_FILES
        Order.c
        Order.h
        Escaper.c
        Escaper.h
        Company.h
        Company.c
        Room.c
        Room.h
        EscapeTechnion.h
        EscapeTechnion.c
        main.c)

add_executable(ex3 ${SOURCE_FILES})
target_link_libraries(ex3 ${CMAKE_SOURCE_DIR}//libmtm/win64/libmtm.a)