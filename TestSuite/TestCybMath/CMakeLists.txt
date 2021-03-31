#Minimum CMake version and policy settings
cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0076 NEW)

#Project Name
project(TestCybMath)

#Add executable
add_executable(TestCybMath)

#Add include dirs
if(WIN32)
    target_include_directories(
        TestCybMath
        PUBLIC
        include
        ../../CybCommon
        ../../CybMath/include
    )
endif(WIN32)

if(UNIX)
    target_include_directories(
        TestCybMath
        PUBLIC
        include
        ../../CybCommon
        ../../CybMath/include
    )
endif(UNIX)

#Add link dirs
if(WIN32)
    target_link_directories(
        TestCybMath
        PUBLIC
    )
endif(WIN32)

if(UNIX)
    target_link_directories(
        TestCybMath
        PUBLIC
    )
endif(UNIX)

#Add source code
target_sources(
    TestCybMath
    PRIVATE
    src/main.c
)

#Libraries to link against
set(LIBS
    CybMath
)

if(WIN32)
    target_link_libraries(
        TestCybMath
        ${LIBS}
    )
endif(WIN32)

if(UNIX)
    target_link_libraries(
        TestCybMath
        ${LIBS}
    )
endif(UNIX)

#Add compile flags
if(MSVC10)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /TP")
endif(MSVC10)

if(UNIX)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath=.")
endif(UNIX)

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
install(TARGETS TestCybMath DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
# install(FILES ${DEPS} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
# install(DIRECTORY data DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)