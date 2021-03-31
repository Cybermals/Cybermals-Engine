#Minimum CMake version and policy settings
cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0076 NEW)

#Project Name
project(TestSuite)

#Include sub-projects
add_subdirectory(TestCybObjects)

if(Build_CybMath)
    add_subdirectory(TestCybMath)
endif(Build_CybMath)

if(Build_CybUI)
    add_subdirectory(TestCybUI)
endif(Build_CybUI)