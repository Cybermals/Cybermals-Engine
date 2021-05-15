@echo off

rem Build CybUI
cd ../../CybUI
call build-apk64

rem Build CybRender
cd ../CybRender
call build-apk64

rem Build main library
cd ../TestSuite/TestCybRender
set NDK_PROJECT_PATH=.
call ndk-build NDK_APPLICATION_MK=./Application64.mk

rem Copy deps
xcopy ..\..\deps\android\arm64-v8a\SDL2\bin android\tmp\lib\arm64-v8a /E /S /Y
xcopy ..\..\deps\android\arm64-v8a\SDL2_image\bin android\tmp\lib\arm64-v8a /E /S /Y
xcopy ..\..\deps\android\arm64-v8a\SDL2_ttf\bin android\tmp\lib\arm64-v8a /E /S /Y
xcopy ..\..\deps\android\arm64-v8a\libexpat\bin android\tmp\lib\arm64-v8a /E /S /Y
xcopy ..\..\CybObjects\libs\arm64-v8a android\tmp\lib\arm64-v8a /E /S /Y
xcopy ..\..\CybMath\libs\arm64-v8a android\tmp\lib\arm64-v8a /E /S /Y
xcopy ..\..\CybUI\libs\arm64-v8a android\tmp\lib\arm64-v8a /E /S /Y
xcopy ..\..\CybRender\libs\arm64-v8a android\tmp\lib\arm64-v8a /E /S /Y
xcopy libs\arm64-v8a android\tmp\lib\arm64-v8a /E /S /Y

rem Copy data files
xcopy ..\data android\assets\data /E /S /Y

rem Build APK
cd android
call build-apk
cd ..

rem Copy APK to output dir
mkdir ..\..\bin\android\arm64-v8a
xcopy android\tmp\*.apk ..\..\bin\android\arm64-v8a /Y
del ..\..\bin\android\arm64-v8a\tmp.apk