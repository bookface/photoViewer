#--------------------------------------------------------------------------
set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel.")
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "Normally the source dir")

#--------------------------------------------------------------------------
project(createDatabase)
cmake_minimum_required(VERSION ${CMAKE_VERSION})
set(CMAKE_DEBUG_POSTFIX  "d")
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)

#--------------------------------------------------------------------------
set(exec ${PROJECT_NAME})

#--------------------------------------------------------------------------
include_directories(
    sqlite
    )

#--------------------------------------------------------------------------
if(WIN32)
    set(libs imm32 winmm ws2_32 Mincore)
else()
    set(libs -lstdc++)
endif()

#--------------------------------------------------------------------------
set(sources
    ./createDatabase.cpp
    ./sqlite/sqlite3.c
    )

#--------------------------------------------------------------------------
add_executable(${exec} ${sources})

#--------------------------------------------------------------------------
target_link_libraries(${exec} ${libs})

#--------------------------------------------------------------------------
install(TARGETS ${exec}
    RUNTIME DESTINATION bin
)

