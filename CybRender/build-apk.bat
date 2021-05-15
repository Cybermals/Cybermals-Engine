@echo off

rem Build CybObjects
cd ../CybObjects
call build-apk

rem Build CybMath
cd ../CybMath
call build-apk

rem Build CybRender
cd ../CybRender
set NDK_PROJECT_PATH=.
ndk-build NDK_APPLICATION_MK=./Application.mk