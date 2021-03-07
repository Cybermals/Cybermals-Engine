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
        ../deps/windows/i386/libexpat/include
    )
endif(WIN32)

if(UNIX)
    target_include_directories(
        CybUI
        PUBLIC
        include
        ../deps/linux/amd64/libexpat/include
    )
endif(UNIX)

#Add link dirs
if(WIN32)
    target_link_directories(
        CybUI
        PUBLIC
        ../deps/windows/i386/libexpat/bin
    )
endif(WIN32)

if(UNIX)
    target_link_directories(
        CybUI
        PUBLIC
        ../deps/linux/amd64/libexpat/bin
    )
endif(UNIX)

#Add source code
target_sources(
    CybUI
    PRIVATE
    src/CybGrid.c
    src/CybTimer.c
    src/CybUI.c
    src/CybUIEvents.c
    src/CybUILoader.c
    src/CybWidgetList.c
)

#Libraries to link against
set(LIBS
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
        ../deps/windows/i386/libexpat/bin/*.dll
        ../deps/windows/i386/libexpat/bin/COPYING-libexpat
    )
    file(COPY ${DEPS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif(WIN32)

if(UNIX)
    file(
        GLOB DEPS
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
file(GLOB CYBUI_HEADERS include/*)
install(FILES ${CYBUI_HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/CybUI)