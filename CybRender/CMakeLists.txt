#Minimum CMake version and policy settings
cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0076 NEW)

#Project Name
project(CybRender)

#Add library
add_library(CybRender SHARED)

#Add include dirs
if(WIN32)
    target_include_directories(
        CybRender
        PUBLIC
        include
        ../deps/windows/i386/SDL2/include/SDL2
        ../deps/windows/i386/SDL2_image/include
    )
endif(WIN32)

if(UNIX)
    target_include_directories(
        CybRender
        PUBLIC
        include
        ../deps/linux/amd64/SDL2/include/SDL2
        ../deps/linux/amd64/SDL2_image/include
    )
endif(UNIX)

#Add link dirs
if(WIN32)
    target_link_directories(
        CybRender
        PUBLIC
        ../deps/windows/i386/SDL2_image/bin
        ../deps/windows/i386/SDL2_image/lib
    )
endif(WIN32)

if(UNIX)
    target_link_directories(
        CybRender
        PUBLIC
        ../deps/linux/amd64/SDL2_image/bin
        ../deps/linux/amd64/SDL2_image/lib
    )
endif(UNIX)

#Add source code
target_sources(
    CybRender
    PRIVATE
    src/CybMesh.c
    src/CybRenderer.c
    src/CybShader.c
)

#Libraries to link against
set(LIBS
    SDL2
    SDL2_image
    CybObjects
    CybMath
)

if(WIN32)
    target_link_libraries(
        CybRender
        ${LIBS}
        opengl32
    )
endif(WIN32)

if(UNIX)
    target_link_libraries(
        CybRender
        ${LIBS}
        GL
    )
endif(UNIX)

#Add compile flags
if(MSVC10)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /TP")
endif(MSVC10)

target_compile_options(
    CybRender
    PUBLIC
    -DDLL_EXPORTS
)

#Copy deps
if(WIN32)
    file(
        GLOB DEPS
        ../deps/windows/i386/SDL2_image/bin/*
    )
    file(COPY ${DEPS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif(WIN32)

if(UNIX)
    file(
        GLOB DEPS
        ../deps/linux/amd64/SDL2_image/bin/*.txt
        ../deps/linux/amd64/SDL2_image/lib/*.so*
    )
    file(COPY ${DEPS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif(UNIX)

#Copy data files
# file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

#Install
install(TARGETS CybRender DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(FILES ${DEPS} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
# install(DIRECTORY data DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

file(GLOB CYBRENDER_HEADERS include/*)
install(FILES ${CYBRENDER_HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/CybRender)