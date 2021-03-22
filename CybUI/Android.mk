LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := CybUI
LOCAL_C_INCLUDES := \
    include \
    ../CybCommon \
    ../CybObjects/include \
    ../deps/android/armeabi-v7a/SDL2/include \
    ../deps/android/armeabi-v7a/SDL2/include/SDL2 \
    ../deps/android/armeabi-v7a/SDL2_image/include \
    ../deps/android/armeabi-v7a/SDL2_ttf/include \
    ../deps/android/armeabi-v7a/libexpat/include
LOCAL_SRC_FILES := \
    src/CybButton.c \
    src/CybGrid.c \
    src/CybLabel.c \
    src/CybTextBox.c \
    src/CybTimer.c \
    src/CybUI.c \
    src/CybUIEvents.c \
    src/CybUILoader.c \
    src/CybWidgetList.c
LOCAL_LDFLAGS += \
    -LC:/android-sdk/ndk/19.2.5345600/toolchains/llvm/prebuilt/windows/lib/gcc/arm-linux-androideabi/4.9.x/armv7-a \
    -LC:/android-sdk/ndk/19.2.5345600/toolchains/llvm/prebuilt/windows/arm-linux-androideabi/lib/armv7-a \
    -L../deps/android/armeabi-v7a/SDL2/bin \
    -L../deps/android/armeabi-v7a/SDL2_image/bin \
    -L../deps/android/armeabi-v7a/SDL2_ttf/bin \
    -L../deps/android/armeabi-v7a/libexpat/bin \
    -L../CybObjects/libs/armeabi-v7a
LOCAL_LDLIBS += -lSDL2 -lSDL2_image -lSDL2_ttf -lexpat -lCybObjects
include $(BUILD_SHARED_LIBRARY)