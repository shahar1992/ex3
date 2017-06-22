cmake_minimum_required(VERSION 3.7)
project(ex3)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

set(SOURCE_FILES
        Room.h
        Room.c
        Escaper.c
        Escaper.h
        Order.c
        Order.h
        Company.h
        Company.c
        EscapeTechnion.h
        EscapeTechnion.c
        mtm_ex3.h
        parser.c
        ./list_mtm1/list_mtm1.c
        ./list_mtm1/list_mtm1.h
        main.c)

add_executable(ex3 ${SOURCE_FILES})
target_link_libraries(ex3 ${CMAKE_SOURCE_DIR}//libmtm/win64/libmtm.a)