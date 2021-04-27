#Minimum CMake version and policy settings
cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0076 NEW)

#Project Name
project(CybAssetMgr)

#Add executable
add_executable(CybAssetMgr)

#Add include dirs
if(WIN32)
    target_include_directories(
        CybAssetMgr
        PUBLIC
        ../../CybCommon
        ../../CybMath/include
        ../../CybRender/include
        ../../deps/windows/i386/sqlite3/include
        ../../deps/windows/i386/assimp/include
    )
endif(WIN32)

if(UNIX)
    target_include_directories(
        CybAssetMgr
        PUBLIC
        ../../CybCommon
        ../../CybMath/include
        ../../CybRender/include
        ../../deps/linux/amd64/sqlite3/include
        ../../deps/linux/amd64/assimp/include
    )
endif(UNIX)

#Add link dirs
if(WIN32)
    target_link_directories(
        CybAssetMgr
        PUBLIC
        ../../deps/windows/i386/sqlite3/lib
        ../../deps/windows/i386/assimp/bin
    )
endif(WIN32)

if(UNIX)
    target_link_directories(
        CybAssetMgr
        PUBLIC
        ../../deps/linux/amd64/sqlite3/lib
        ../../deps/linux/amd64/assimp/lib
    )
endif(UNIX)

#Add source code
target_sources(
    CybAssetMgr
    PRIVATE
    src/main.c
)

#Libraries to link against
set(LIBS
    sqlite3
    assimp
)

if(WIN32)
    target_link_libraries(
        CybAssetMgr
        ${LIBS}
    )
endif(WIN32)

if(UNIX)
    target_link_libraries(
        CybAssetMgr
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
        ../../deps/windows/i386/assimp/bin/*.dll
        ../../deps/windows/i386/assimp/bin/LICENSE*
    )
    file(COPY ${DEPS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif(WIN32)

if(UNIX)
    file(
        GLOB DEPS
        ../../deps/linux/amd64/assimp/lib/*.so*
        ../../deps/linux/amd64/assimp/bin/LICENSE*
    )
    file(COPY ${DEPS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif(UNIX)

#Copy data files
# file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

#Install
install(TARGETS CybAssetMgr DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(FILES ${DEPS} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
# install(DIRECTORY data DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)