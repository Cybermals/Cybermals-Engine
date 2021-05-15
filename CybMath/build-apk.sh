#!/usr/bin/bash

#Add Android NDK to path
export PATH=$PATH:$HOME/android-sdk/ndk/19.2.5345600

#Build CybMath
export NDK_PROJECT_PATH=.
ndk-build NDK_APPLICATION_MK=./Application.mk
