#Minimum CMake version and policy settings
cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0076 NEW)

#Project Name
project(Cybermals-Engine)

#Include sub-projects
add_subdirectory(CybMath)    #required
add_subdirectory(CybObjects) #required

set(Build_CybRender ON CACHE BOOL "Build the rendering library.")
set(Build_CybUI ON CACHE BOOL "Build the UI subsystem.")
set(Build_TestSuite ON CACHE BOOL "Build the test suite.")
set(Build_Tools ON CACHE BOOL "Build the tool programs.")

if(Build_CybRender)
    add_subdirectory(CybRender)
endif(Build_CybRender)

if(Build_CybUI)
    add_subdirectory(CybUI)
endif(Build_CybUI)

if(Build_TestSuite)
    add_subdirectory(TestSuite)
endif(Build_TestSuite)

if(Build_Tools)
    add_subdirectory(Tools)
endif(Build_Tools)