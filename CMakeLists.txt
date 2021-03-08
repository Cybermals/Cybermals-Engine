#Minimum CMake version and policy settings
cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0076 NEW)

#Project Name
project(Cybermals-Engine)

#Include sub-projects
add_subdirectory(CybObjects) #required

set(Build_CybUI ON CACHE BOOL "Build the UI subsystem.")
set(Build_TestSuite ON CACHE BOOL "Build the test suite.")

if(Build_CybUI)
    add_subdirectory(CybUI)
endif(Build_CybUI)

if(Build_TestSuite)
    add_subdirectory(TestSuite)
endif(Build_TestSuite)