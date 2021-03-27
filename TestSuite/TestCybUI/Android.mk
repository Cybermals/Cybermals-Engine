LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := main
LOCAL_C_INCLUDES := \
    ../../deps/android/armeabi-v7a/SDL2/include \
    ../../deps/android/armeabi-v7a/SDL2/include/SDL2 \
    ../../deps/android/armeabi-v7a/SDL2_ttf/include \
    ../../CybCommon \
    ../../CybObjects/include \
    ../../CybUI/include
LOCAL_SRC_FILES := \
    src/main.c
LOCAL_LDFLAGS += \
    -LC:/android-sdk/ndk/19.2.5345600/toolchains/llvm/prebuilt/windows/lib/gcc/arm-linux-androideabi/4.9.x/armv7-a \
    -LC:/android-sdk/ndk/19.2.5345600/toolchains/llvm/prebuilt/windows/arm-linux-androideabi/lib/armv7-a \
    -L../../deps/android/armeabi-v7a/SDL2/bin \
    -L../../CybObjects/libs/armeabi-v7a \
    -L../../CybUI/libs/armeabi-v7a
LOCAL_LDLIBS += -lSDL2 -lCybObjects -lCybUI
include $(BUILD_SHARED_LIBRARY)