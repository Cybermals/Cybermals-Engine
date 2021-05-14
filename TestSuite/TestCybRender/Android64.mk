LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := main
LOCAL_C_INCLUDES := \
    ../../CybCommon \
    ../../deps/android/arm64-v8a/SDL2/include \
    ../../deps/android/arm64-v8a/SDL2/include/SDL2 \
    ../../deps/android/arm64-v8a/SDL2_ttf/include \
    ../../deps/android/arm64-v8a/sqlite3/include \
    ../../CybObjects/include \
    ../../CybMath/include \
    ../../CybUI/include \
    ../../CybRender/include
LOCAL_SRC_FILES := \
    src/main.c
LOCAL_LDFLAGS += \
    -LC:/android-sdk/ndk/19.2.5345600/toolchains/llvm/prebuilt/windows/lib/gcc/aarch64-linux-android/4.9.x \
    -LC:/android-sdk/ndk/19.2.5345600/toolchains/llvm/prebuilt/windows/aarch64-linux-android/lib64 \
    -L../../deps/android/arm64-v8a/SDL2/bin \
    -L../../CybObjects/libs/arm64-v8a \
    -L../../CybMath/libs/arm64-v8a \
    -L../../CybUI/libs/arm64-v8a \
    -L../../CybRender/libs/arm64-v8a
LOCAL_LDLIBS += -lSDL2 -lCybObjects -lCybMath -lCybUI -lCybRender -lGLESv3
include $(BUILD_SHARED_LIBRARY)