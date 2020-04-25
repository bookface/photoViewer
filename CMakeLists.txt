#--------------------------------------------------------------------------
set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel.")
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "Normally the source dir")

#--------------------------------------------------------------------------
project(project)
cmake_minimum_required(VERSION 3.0)
set(CMAKE_DEBUG_POSTFIX  "d")
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)

#--------------------------------------------------------------------------
set(exec photoViewer)

#--------------------------------------------------------------------------
include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    )

#--------------------------------------------------------------------------
find_package(Qt5 COMPONENTS Core Gui Widgets )

execute_process(COMMAND ${QMAKE_EXECUTABLE} -query QT_VERSION OUTPUT_VARIABLE QT_VERSION)
message(STATUS "Qt Version: ${QT_VERSION}Location: ${Qt5_DIR}")

set(CMAKE_AUTOMOC ON)
set(QT_LIBRARIES
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    imm32
    winmm
    ws2_32
    Mincore
    )

#--------------------------------------------------------------------------
set(sources
    ./main.cpp
    ./mainWindow.cpp
    ./mainWindow.h
    )

#--------------------------------------------------------------------------
add_executable(${exec} ${sources})

#--------------------------------------------------------------------------
target_link_libraries(${exec} ${QT_LIBRARIES})

#--------------------------------------------------------------------------
install(TARGETS ${exec}
    RUNTIME DESTINATION bin
)

