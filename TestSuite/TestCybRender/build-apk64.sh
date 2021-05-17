#!/usr/bin/bash

#Build CybUI
cd ../../CybUI
source ./build-apk64.sh

#Build CybRender
cd ../CybRender
source ./build-apk64.sh

#Build main library
cd ../TestSuite/TestCybRender
export NDK_PROJECT_PATH=.
ndk-build NDK_APPLICATION_MK=./Application64.mk

#Copy deps
cp ../../deps/android/arm64-v8a/SDL2/bin/*.* android/tmp/lib/arm64-v8a
cp ../../deps/android/arm64-v8a/SDL2_image/bin/*.* android/tmp/lib/arm64-v8a
cp ../../deps/android/arm64-v8a/SDL2_ttf/bin/*.* android/tmp/lib/arm64-v8a
cp ../../deps/android/arm64-v8a/libexpat/bin/*.* android/tmp/lib/arm64-v8a
cp ../../CybObjects/libs/arm64-v8a/*.* android/tmp/lib/arm64-v8a
cp ../../CybMath/libs/arm64-v8a/*.* android/tmp/lib/arm64-v8a
cp ../../CybRender/libs/arm64-v8a/*.* android/tmp/lib/arm64-v8a
cp ../../CybUI/libs/arm64-v8a/*.* android/tmp/lib/arm64-v8a
cp libs/arm64-v8a/*.* android/tmp/lib/arm64-v8a

#Copy data files
cp -r ../data android/assets

#Build APK
cd android
source ./build-apk.sh
cd ..

#Copy APK to output dir
mkdir ../../bin/android/arm64-v8a
cp android/tmp/*.apk ../../bin/android/arm64-v8a
rm ../../bin/android/arm64-v8a/tmp.apk
