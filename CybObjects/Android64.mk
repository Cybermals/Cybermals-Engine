LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := CybObjects
LOCAL_C_INCLUDES := \
    include \
    ../CybCommon \
    ../deps/android/arm64-v8a/SDL2/include
LOCAL_SRC_FILES := \
    src/CybList.c \
    src/CybObject.c \
    src/CybObjects.c \
    src/CybQueue.c \
    src/CybVector.c
LOCAL_LDFLAGS += \
    -LC:/android-sdk/ndk/19.2.5345600/toolchains/llvm/prebuilt/windows/lib/gcc/aarch64-linux-android/4.9.x \
    -LC:/android-sdk/ndk/19.2.5345600/toolchains/llvm/prebuilt/windows/aarch64-linux-android/lib64 \
    -L../deps/android/arm64-v8a/SDL2/bin
LOCAL_LDLIBS += -lSDL2
include $(BUILD_SHARED_LIBRARY)