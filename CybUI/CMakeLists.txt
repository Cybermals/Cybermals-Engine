#Minimum CMake version and policy settings
cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0076 NEW)

#Project Name
project(CybUI)

#Add library
add_library(CybUI SHARED)

#Add include dirs
if(WIN32)
    target_include_directories(
        CybUI
        PUBLIC
        include
        ../deps/windows/i386/SDL2/include/SDL2
        ../deps/windows/i386/SDL2_image/include
        ../deps/windows/i386/SDL2_ttf/include
        ../deps/windows/i386/libexpat/include
    )
endif(WIN32)

if(UNIX)
    target_include_directories(
        CybUI
        PUBLIC
        include
        ../deps/linux/amd64/SDL2/include/SDL2
        ../deps/linux/amd64/SDL2_image/include
        ../deps/linux/amd64/SDL2_ttf/include
        ../deps/linux/amd64/libexpat/include
    )
endif(UNIX)

#Add link dirs
if(WIN32)
    target_link_directories(
        CybUI
        PUBLIC
        ../deps/windows/i386/SDL2_image/bin
        ../deps/windows/i386/SDL2_image/lib
        ../deps/windows/i386/SDL2_ttf/bin
        ../deps/windows/i386/SDL2_ttf/lib
        ../deps/windows/i386/libexpat/bin
    )
endif(WIN32)

if(UNIX)
    target_link_directories(
        CybUI
        PUBLIC
        ../deps/linux/amd64/SDL2_image/bin
        ../deps/linux/amd64/SDL2_image/lib
        ../deps/linux/amd64/SDL2_ttf/bin
        ../deps/linux/amd64/SDL2_ttf/lib
        ../deps/linux/amd64/libexpat/bin
    )
endif(UNIX)

#Add source code
target_sources(
    CybUI
    PRIVATE
    src/CybButton.c
    src/CybGrid.c
    src/CybLabel.c
    src/CybListBox.c
    src/CybProgressBar.c
    src/CybTextBox.c
    src/CybTimer.c
    src/CybUI.c
    src/CybUIEvents.c
    src/CybUILoader.c
    src/CybWidgetList.c
)

#Libraries to link against
set(LIBS
    SDL2_image
    SDL2_ttf
    expat
    CybObjects
)

if(WIN32)
    target_link_libraries(
        CybUI
        ${LIBS}
    )
endif(WIN32)

if(UNIX)
    target_link_libraries(
        CybUI
        ${LIBS}
    )
endif(UNIX)

#Add compile flags
if(MSVC10)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /TP")
endif(MSVC10)

target_compile_options(
    CybUI
    PUBLIC
    -DDLL_EXPORTS
)

#Copy deps
if(WIN32)
    file(
        GLOB DEPS
        ../deps/windows/i386/SDL2_image/bin/*
        ../deps/windows/i386/SDL2_ttf/bin/*
        ../deps/windows/i386/libexpat/bin/*.dll
        ../deps/windows/i386/libexpat/bin/COPYING-libexpat
    )
    file(COPY ${DEPS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif(WIN32)

if(UNIX)
    file(
        GLOB DEPS
        ../deps/linux/amd64/SDL2/lib/*.a
        ../deps/linux/amd64/SDL2_image/bin/*.txt
        ../deps/linux/amd64/SDL2_image/lib/*.so*
        ../deps/linux/amd64/SDL2_ttf/bin/*.txt
        ../deps/linux/amd64/SDL2_ttf/lib/*.so*
        ../deps/linux/amd64/libexpat/bin/*.so*
        ../deps/linux/amd64/libexpat/bin/COPYING-libexpat
    )
    file(COPY ${DEPS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif(UNIX)

#Copy data files
# file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

#Install
install(TARGETS CybUI DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(FILES ${DEPS} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
# install(DIRECTORY data DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

if(WIN32)
    install(DIRECTORY ../deps/windows/i386/SDL2_ttf/include DESTINATION ${CMAKE_INSTALL_PREFIX})
endif(WIN32)

if(UNIX)
    install(DIRECTORY ../deps/linux/amd64/SDL2_ttf/include DESTINATION ${CMAKE_INSTALL_PREFIX})
endif(UNIX)

file(GLOB CYBUI_HEADERS include/*)
install(FILES ${CYBUI_HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/CybUI)