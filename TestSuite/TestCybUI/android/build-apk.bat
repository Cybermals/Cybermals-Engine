@echo off

rem Delete temp files from previous build
echo Cleaning...
del /Q obj\org\libsdl\app\*.*
del /Q obj\ml\cybermals\TestCybUI\*.*
del /Q tmp\*.*
del tmp\tmp.apk

rem Build the apk
echo Building...
aapt package -f -m -F tmp/tmp.apk --auto-add-overlay -S res -J src -G android-aapt.pro -A assets -M AndroidManifest.xml -I C:\android-sdk\platforms\android-28\android.jar
javac -classpath C:/android-sdk/platforms/android-28/android.jar src/org/libsdl/app/*.java src/ml/cybermals/TestCybUI/*.java -d obj
jar cvf tmp/classes.jar -C obj/ .
call dx --dex --output tmp/classes.dex tmp/classes.jar
cd tmp
mkdir lib\armeabi-v7a
mkdir lib\x86_64
aapt add tmp.apk classes.dex
aapt add tmp.apk lib/armeabi-v7a/libhidapi.so
aapt add tmp.apk lib/armeabi-v7a/libSDL2.so
aapt add tmp.apk lib/armeabi-v7a/libSDL2_image.so
aapt add tmp.apk lib/armeabi-v7a/libSDL2_ttf.so
aapt add tmp.apk lib/armeabi-v7a/libexpat.so
aapt add tmp.apk lib/armeabi-v7a/libCybObjects.so
aapt add tmp.apk lib/armeabi-v7a/libCybUI.so
aapt add tmp.apk lib/armeabi-v7a/libmain.so
zipalign 4 tmp.apk TestCybUI.apk
cd ..

rem Sign the apk
echo Signing...
call apksigner sign --ks my-release-key.keystore tmp/TestCybUI.apk
echo done