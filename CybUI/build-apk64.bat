@echo off

rem Build CybObjects
cd ../CybObjects
call build-apk64

rem Build CybUI
cd ../CybUI
set NDK_PROJECT_PATH=.
ndk-build NDK_APPLICATION_MK=./Application64.mk