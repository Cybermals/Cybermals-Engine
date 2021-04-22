LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := CybMath
LOCAL_C_INCLUDES := \
    include \
    ../CybCommon
LOCAL_SRC_FILES := \
    src/CybBox.c \
    src/CybMatrix.c \
    src/CybSphere.c \
    src/CybVec.c
LOCAL_LDFLAGS += \
    -LC:/android-sdk/ndk/19.2.5345600/toolchains/llvm/prebuilt/windows/lib/gcc/arm-linux-androideabi/4.9.x/armv7-a \
    -LC:/android-sdk/ndk/19.2.5345600/toolchains/llvm/prebuilt/windows/arm-linux-androideabi/lib/armv7-a
LOCAL_LDLIBS += -lm
include $(BUILD_SHARED_LIBRARY)