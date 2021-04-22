@echo off

rem Build CybUI
cd ../../CybUI
call build-apk

rem Build CybRender
cd ../CybRender
call build-apk

rem Build main library
cd ../TestSuite/TestCybRender
set NDK_PROJECT_PATH=.
call ndk-build NDK_APPLICATION_MK=./Application.mk

rem Copy deps
xcopy ..\..\deps\android\armeabi-v7a\SDL2\bin android\tmp\lib\armeabi-v7a /E /S /Y
xcopy ..\..\deps\android\armeabi-v7a\SDL2_image\bin android\tmp\lib\armeabi-v7a /E /S /Y
xcopy ..\..\deps\android\armeabi-v7a\SDL2_ttf\bin android\tmp\lib\armeabi-v7a /E /S /Y
xcopy ..\..\deps\android\armeabi-v7a\libexpat\bin android\tmp\lib\armeabi-v7a /E /S /Y
xcopy ..\..\CybObjects\libs\armeabi-v7a android\tmp\lib\armeabi-v7a /E /S /Y
xcopy ..\..\CybMath\libs\armeabi-v7a android\tmp\lib\armeabi-v7a /E /S /Y
xcopy ..\..\CybUI\libs\armeabi-v7a android\tmp\lib\armeabi-v7a /E /S /Y
xcopy ..\..\CybRender\libs\armeabi-v7a android\tmp\lib\armeabi-v7a /E /S /Y
xcopy libs\armeabi-v7a android\tmp\lib\armeabi-v7a /E /S /Y

rem Copy data files
xcopy ..\data android\assets\data /E /S /Y

rem Build APK
cd android
call build-apk
cd ..

rem Copy APK to output dir
mkdir ..\..\bin\android\armeabi-v7a
xcopy android\tmp\*.apk ..\..\bin\android\armeabi-v7a /Y
del ..\..\bin\android\armeabi-v7a\tmp.apk