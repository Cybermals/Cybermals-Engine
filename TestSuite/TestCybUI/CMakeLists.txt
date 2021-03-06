#Minimum CMake version and policy settings
cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0076 NEW)

#Project Name
project(TestCybUI)

#Add executable
add_executable(TestCybUI)

#Add include dirs
# if(WIN32)
    # target_include_directories(
        # TestCybUI
        # PUBLIC
    # )
# endif(WIN32)

# if(UNIX)
    # target_include_directories(
        # TestCybUI
        # PUBLIC
    # )
# endif(UNIX)

#Add link dirs
# if(WIN32)
    # target_link_directories(
        # TestCybUI
        # PUBLIC
    # )
# endif(WIN32)

# if(UNIX)
    # target_link_directories(
        # TestCybUI
        # PUBLIC
    # )
# endif(UNIX)

#Add source code
target_sources(
    TestCybUI
    PRIVATE
    src/main.c
)

#Libraries to link against
set(LIBS
    SDL2main
    CybUI
)

if(WIN32)
    if(MINGW)
        set(LIBS
            mingw32
            ${LIBS}
        )
    endif(MINGW)
    
    target_link_libraries(
        TestCybUI
        ${LIBS}
    )
endif(WIN32)

if(UNIX)
    target_link_libraries(
        TestCybUI
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
# if(WIN32)
    # file(
        # GLOB DEPS
    # )
    # file(COPY ${DEPS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# endif(WIN32)

# if(UNIX)
    # file(
        # GLOB DEPS
    # )
    # file(COPY ${DEPS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# endif(UNIX)

#Copy data files
file(COPY ../data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

#Install
install(TARGETS TestCybUI DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
# install(FILES ${DEPS} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(DIRECTORY ../data DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)