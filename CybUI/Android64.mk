LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := CybUI
LOCAL_C_INCLUDES := \
    include \
    ../CybCommon \
    ../CybObjects/include \
    ../deps/android/arm64-v8a/SDL2/include \
    ../deps/android/arm64-v8a/SDL2/include/SDL2 \
    ../deps/android/arm64-v8a/SDL2_image/include \
    ../deps/android/arm64-v8a/SDL2_ttf/include \
    ../deps/android/arm64-v8a/libexpat/include
LOCAL_SRC_FILES := \
    src/CybButton.c \
    src/CybGrid.c \
    src/CybLabel.c \
    src/CybListBox.c \
    src/CybProgressBar.c \
    src/CybTextBox.c \
    src/CybTimer.c \
    src/CybUI.c \
    src/CybUIEvents.c \
    src/CybUILoader.c \
    src/CybWidgetList.c
LOCAL_LDFLAGS += \
    -LC:/android-sdk/ndk/19.2.5345600/toolchains/llvm/prebuilt/windows/lib/gcc/aarch64-linux-android/4.9.x \
    -LC:/android-sdk/ndk/19.2.5345600/toolchains/llvm/prebuilt/windows/aarch64-linux-android/lib64 \
    -L../deps/android/arm64-v8a/SDL2/bin \
    -L../deps/android/arm64-v8a/SDL2_image/bin \
    -L../deps/android/arm64-v8a/SDL2_ttf/bin \
    -L../deps/android/arm64-v8a/libexpat/bin \
    -L../CybObjects/libs/arm64-v8a
LOCAL_LDLIBS += -lSDL2 -lSDL2_image -lSDL2_ttf -lexpat -lCybObjects
include $(BUILD_SHARED_LIBRARY)