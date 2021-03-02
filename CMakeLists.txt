#Minimum CMake version and policy settings
cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0076 NEW)

#Project Name
project(Cybermals-Engine)

#Include sub-projects
add_subdirectory(CybObjects) #required

set(BuildTestSuite ON CACHE BOOL "Build the test suite.")

if(BuildTestSuite)
    add_subdirectory(TestSuite)
endif(BuildTestSuite)