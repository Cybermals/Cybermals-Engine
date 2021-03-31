#Minimum CMake version and policy settings
cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0076 NEW)

#Project Name
project(CybMath)

#Add library
add_library(CybMath SHARED)

#Add include dirs
if(WIN32)
    target_include_directories(
        CybMath
        PUBLIC
        include
        ../CybCommon
    )
endif(WIN32)

if(UNIX)
    target_include_directories(
        CybMath
        PUBLIC
        include
        ../CybCommon
    )
endif(UNIX)

#Add link dirs
if(WIN32)
    target_link_directories(
        CybMath
        PUBLIC
    )
endif(WIN32)

if(UNIX)
    target_link_directories(
        CybMath
        PUBLIC
    )
endif(UNIX)

#Add source code
target_sources(
    CybMath
    PRIVATE
    src/CybBox.c
    src/CybMatrix.c
    src/CybVector.c
)

#Libraries to link against
set(LIBS
)

if(WIN32)
    target_link_libraries(
        CybMath
        ${LIBS}
    )
endif(WIN32)

if(UNIX)
    target_link_libraries(
        CybMath
        ${LIBS}
        m
    )
endif(UNIX)

#Add compile flags
if(MSVC10)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /TP")
endif(MSVC10)

target_compile_options(
    CybMath
    PUBLIC
    -DDLL_EXPORTS
)

#Copy deps
if(WIN32)
    file(
        GLOB DEPS
    )
    file(COPY ${DEPS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif(WIN32)

if(UNIX)
    file(
        GLOB DEPS
    )
    file(COPY ${DEPS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif(UNIX)

#Copy data files
# file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

#Install
install(TARGETS CybMath DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
# install(FILES ${DEPS} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
# install(DIRECTORY data DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX})