#--------------------------------------------------------------------------
project(sqlitelib)
cmake_minimum_required(VERSION ${CMAKE_VERSION})

set(lib sqlitelib)

set(lib_SRC
    shell.c
    sqlite3.c
    )

add_library(${lib} STATIC ${lib_SRC})



