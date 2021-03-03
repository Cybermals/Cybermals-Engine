#Minimum CMake version and policy settings
cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0076 NEW)

#Project Name
project(CybObjects)

#Add library
add_library(CybObjects SHARED)

#Add include dirs
if(WIN32)
    target_include_directories(
        CybObjects
        PUBLIC
        include
        ../CybCommon
        ../deps/windows/i386/SDL2/include
    )
endif(WIN32)

if(UNIX)
    target_include_directories(
        CybObjects
        PUBLIC
        include
        ../CybCommon
        ../deps/linux/amd64/SDL2/include
    )
endif(UNIX)

#Add link dirs
if(WIN32)
    target_link_directories(
        CybObjects
        PUBLIC
        ../deps/windows/i386/SDL2/bin
        ../deps/windows/i386/SDL2/lib
    )
endif(WIN32)

if(UNIX)
    target_link_directories(
        CybObjects
        PUBLIC
        ../deps/linux/amd64/SDL2/bin
        ../deps/linux/amd64/SDL2/lib
    )
endif(UNIX)

#Add source code
target_sources(
    CybObjects
    PRIVATE
    src/CybList.c
    src/CybObject.c
    src/CybObjects.c
)

#Libraries to link against
set(LIBS
    SDL2
)

if(WIN32)
    target_link_libraries(
        CybObjects
        ${LIBS}
    )
endif(WIN32)

if(UNIX)
    target_link_libraries(
        CybObjects
        ${LIBS}
    )
endif(UNIX)

#Add compile flags
if(MSVC10)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /TP")
endif(MSVC10)

target_compile_options(
    CybObjects
    PUBLIC
    -DDLL_EXPORTS
)

#Copy deps
if(WIN32)
    file(
        GLOB DEPS
        ../deps/windows/i386/SDL2/bin/*
    )
    file(COPY ${DEPS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif(WIN32)

if(UNIX)
    file(
        GLOB DEPS
        ../deps/linux/amd64/SDL2/bin/*.txt
        ../deps/linux/amd64/SDL2/lib/*.so*
    )
    file(COPY ${DEPS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif(UNIX)

#Copy data files
# file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

#Install
install(TARGETS CybObjects DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(FILES ${DEPS} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
# install(DIRECTORY data DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

if(WIN32)
    install(DIRECTORY ../deps/windows/i386/SDL2/include DESTINATION ${CMAKE_INSTALL_PREFIX})
endif(WIN32)

if(UNIX)
    install(DIRECTORY ../deps/linux/amd64/SDL2/include DESTINATION ${CMAKE_INSTALL_PREFIX})
endif(UNIX)

file(GLOB CYBOBJECTS_HEADERS include/*)
install(FILES ${CYBOBJECTS_HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/CybObjects)