#Minimum CMake version and policy settings
cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0076 NEW)

#Project Name
project(Library)

#Add library
add_library(Library SHARED)

#Add include dirs
if(WIN32)
    target_include_directories(
        Library
        PUBLIC
        include
    )
endif(WIN32)

if(UNIX)
    target_include_directories(
        Library
        PUBLIC
        include
    )
endif(UNIX)

#Add link dirs
if(WIN32)
    target_link_directories(
        Library
        PUBLIC
    )
endif(WIN32)

if(UNIX)
    target_link_directories(
        Library
        PUBLIC
    )
endif(UNIX)

#Add source code
target_sources(
    Library
    PRIVATE
    src/main.c
)

#Libraries to link against
set(LIBS
)

if(WIN32)
    target_link_libraries(
        Library
        ${LIBS}
    )
endif(WIN32)

if(UNIX)
    target_link_libraries(
        Library
        ${LIBS}
    )
endif(UNIX)

#Add compile flags
if(MSVC10)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /TP")
endif(MSVC10)

target_compile_options(
    Library
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
file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

#Install
install(TARGETS Library DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(FILES ${DEPS} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(DIRECTORY data DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX})