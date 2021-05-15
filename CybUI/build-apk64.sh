#!/usr/bin/bash

#Build CybObjects
cd ../CybObjects
source ./build-apk64.sh

#Build CybUI
cd ../CybUI
export NDK_PROJECT_PATH=.
ndk-build NDK_APPLICATION_MK=./Application64.mk
