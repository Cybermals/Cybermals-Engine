#!/usr/bin/bash

#Add Android build tools to path
export PATH=$PATH:$HOME/android-sdk/build-tools/28.0.2

#Delete temp files from previous build
echo Cleaning...
rm obj/org/libsdl/app/*.*
rm obj/ml/cybermals/TestCybRender/*.*
rm tmp/*.*
rm tmp/tmp.apk

#Build the apk
echo Building...
aapt package -f -m -F tmp/tmp.apk --auto-add-overlay -S res -J src -G android-aapt.pro -A assets -M AndroidManifest.xml -I $HOME/android-sdk/platforms/android-28/android.jar
javac -classpath $HOME/android-sdk/platforms/android-28/android.jar src/org/libsdl/app/*.java src/ml/cybermals/TestCybRender/*.java -d obj
jar cvf tmp/classes.jar -C obj/ .
dx --dex --output tmp/classes.dex tmp/classes.jar
cd tmp
mkdir lib/armeabi-v7a
mkdir lib/arm64-v8a
mkdir lib/x86_64
aapt add tmp.apk classes.dex
aapt add tmp.apk lib/armeabi-v7a/libhidapi.so
aapt add tmp.apk lib/armeabi-v7a/libSDL2.so
aapt add tmp.apk lib/armeabi-v7a/libSDL2_image.so
aapt add tmp.apk lib/armeabi-v7a/libSDL2_ttf.so
aapt add tmp.apk lib/armeabi-v7a/libexpat.so
aapt add tmp.apk lib/armeabi-v7a/libCybObjects.so
aapt add tmp.apk lib/armeabi-v7a/libCybMath.so
aapt add tmp.apk lib/armeabi-v7a/libCybRender.so
aapt add tmp.apk lib/armeabi-v7a/libCybUI.so
aapt add tmp.apk lib/armeabi-v7a/libmain.so
aapt add tmp.apk lib/arm64-v8a/libhidapi.so
aapt add tmp.apk lib/arm64-v8a/libSDL2.so
aapt add tmp.apk lib/arm64-v8a/libSDL2_image.so
aapt add tmp.apk lib/arm64-v8a/libSDL2_ttf.so
aapt add tmp.apk lib/arm64-v8a/libexpat.so
aapt add tmp.apk lib/arm64-v8a/libCybObjects.so
aapt add tmp.apk lib/arm64-v8a/libCybMath.so
aapt add tmp.apk lib/arm64-v8a/libCybRender.so
aapt add tmp.apk lib/arm64-v8a/libCybUI.so
aapt add tmp.apk lib/arm64-v8a/libmain.so
zipalign 4 tmp.apk TestCybRender.apk
cd ..

#Sign the apk
echo Signing...
apksigner sign --ks my-release-key.keystore tmp/TestCybRender.apk
echo done
