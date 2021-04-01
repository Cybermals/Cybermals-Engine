#Minimum CMake version and policy settings
cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0076 NEW)

#Project Name
project(TestSuite)

#Include sub-projects
add_subdirectory(TestCybMath)
add_subdirectory(TestCybObjects)

if(Build_CybRender)
    add_subdirectory(TestCybRender)
endif(Build_CybRender)

if(Build_CybUI)
    add_subdirectory(TestCybUI)
endif(Build_CybUI)