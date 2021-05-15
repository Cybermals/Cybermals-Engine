LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := CybMath
LOCAL_C_INCLUDES := \
    include \
    ../CybCommon
LOCAL_SRC_FILES := \
    src/CybBox.c \
    src/CybMatrix.c \
    src/CybQuat.c \
    src/CybSphere.c \
    src/CybVec.c
LOCAL_LDFLAGS += \
    -LC:/android-sdk/ndk/19.2.5345600/toolchains/llvm/prebuilt/windows/lib/gcc/aarch64-linux-android/4.9.x \
    -LC:/android-sdk/ndk/19.2.5345600/toolchains/llvm/prebuilt/windows/aarch64-linux-android/lib64
LOCAL_LDLIBS += -lm
include $(BUILD_SHARED_LIBRARY)