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
    ${CMAKE_SOURCE_DIR}/exif
    ${CMAKE_BINARY_DIR}
    )

#--------------------------------------------------------------------------
# Set QT_VERSION using qmake.exe
find_program(QMAKE_EXE NAMES qmake)
execute_process(COMMAND ${QMAKE_EXE} -query QT_VERSION OUTPUT_VARIABLE QT_VERSION)
message("QT VERSION ${QT_VERSION}")

if(WIN32)
    set(winlibs
        imm32
        winmm
        ws2_32
        Mincore
        )
endif()

set(CMAKE_AUTOMOC ON)
if(QT_VERSION LESS 6.0)
    find_package(Qt5 COMPONENTS Core Gui Widgets )
    set(QT_LIBRARIES
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        ${winlibs}
        )
# Qt Version 6
else()
    find_package(Qt6 COMPONENTS Core Gui Widgets)
    set(QT_LIBRARIES
        Qt::Core
        Qt::Gui
        Qt::Widgets
        ${winlibs}
        )
endif()    

#--------------------------------------------------------------------------
set(sources
    ./main.cpp
    ./mainWindow.cpp
    ./mainWindow.h
    ./processCommandLine.cpp
    ./getOrientation.cpp
    ./exif/exif.cpp
    )

#--------------------------------------------------------------------------
add_executable(${exec} ${sources})

#--------------------------------------------------------------------------
target_link_libraries(${exec} ${QT_LIBRARIES})

#--------------------------------------------------------------------------
install(TARGETS ${exec}
    RUNTIME DESTINATION bin
)

