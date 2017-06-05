cmake_minimum_required(VERSION 3.7)
project(ex3)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES
        ./tests/company_test.c
        EscapeTechnion.h
        Company.h
        notes.c
        Room.h
        Room.c
        Company.c
        EscapeTechnion.c
        Order.h
        Order.c
        )

add_executable(ex3 ${SOURCE_FILES})
target_link_libraries(ex3 ${CMAKE_SOURCE_DIR}//libmtm/win64/libmtm.a)