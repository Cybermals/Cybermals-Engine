#!/usr/bin/bash

#Build CybObjects
cd ../CybObjects
source ./build-apk.sh

#Build CybMath
cd ../CybMath
source ./build-apk.sh

#Build CybRender
cd ../CybRender
export NDK_PROJECT_PATH=.
ndk-build NDK_APPLICATION_MK=./Application.mk
