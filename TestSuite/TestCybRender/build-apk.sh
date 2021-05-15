#!/usr/bin/bash

#Build CybUI
cd ../../CybUI
source ./build-apk.sh

#Build CybRender
cd ../CybRender
source ./build-apk.sh

#Build main library
cd ../TestSuite/TestCybRender
export NDK_PROJECT_PATH=.
ndk-build NDK_APPLICATION_MK=./Application.mk

#Copy deps
cp ../../deps/android/armeabi-v7a/SDL2/bin/*.* android/tmp/lib/armeabi-v7a
cp ../../deps/android/armeabi-v7a/SDL2_image/bin/*.* android/tmp/lib/armeabi-v7a
cp ../../deps/android/armeabi-v7a/SDL2_ttf/bin/*.* android/tmp/lib/armeabi-v7a
cp ../../deps/android/armeabi-v7a/libexpat/bin/*.* android/tmp/lib/armeabi-v7a
cp ../../CybObjects/libs/armeabi-v7a/*.* android/tmp/lib/armeabi-v7a
cp ../../CybMath/libs/armeabi-v7a/*.* android/tmp/lib/armeabi-v7a
cp ../../CybRender/libs/armeabi-v7a/*.* android/tmp/lib/armeabi-v7a
cp ../../CybUI/libs/armeabi-v7a/*.* android/tmp/lib/armeabi-v7a

#Copy data files
cp -r ../data android/assets

#Build APK
cd android
source ./build-apk.sh
cd ..

#Copy APK to output dir
mkdir ../../bin/android/armeabi-v7a
cp android/tmp/*.apk ../../bin/android/armeabi-v7a
rm ../../bin/android/armeabi-v7a/tmp.apk
