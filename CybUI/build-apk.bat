@echo off

rem Build objects lib
cd ../CybObjects
call build-apk
cd ../CybUI

rem Build UI lib
set NDK_PROJECT_PATH=.
ndk-build NDK_APPLICATION_MK=./Application.mk