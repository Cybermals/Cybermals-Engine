@echo off

rem Build objects library
set NDK_PROJECT_PATH=.
ndk-build NDK_APPLICATION_MK=./Application.mk