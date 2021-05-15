LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := CybRender
LOCAL_C_INCLUDES := \
    include \
    ../CybCommon \
    ../deps/android/arm64-v8a/SDL2/include \
    ../deps/android/arm64-v8a/SDL2/include/SDL2 \
    ../deps/android/arm64-v8a/SDL2_image/include \
    ../deps/android/arm64-v8a/sqlite3/include \
    ../CybObjects/include \
    ../CybMath/include
LOCAL_SRC_FILES := \
    src/CybAnimation.c \
    src/CybAnimChannel.c \
    src/CybArmature.c \
    src/CybAssetLoader.c \
    src/CybCamera.c \
    src/CybLight.c \
    src/CybMaterial.c \
    src/CybMesh.c \
    src/CybRenderer.c \
    src/CybShader.c \
    src/CybTexture.c \
    src/CybTimer.c
LOCAL_LDFLAGS += \
    -LC:/android-sdk/ndk/19.2.5345600/toolchains/llvm/prebuilt/windows/lib/gcc/aarch64-linux-android/4.9.x \
    -LC:/android-sdk/ndk/19.2.5345600/toolchains/llvm/prebuilt/windows/aarch64-linux-android/lib64 \
    -L../deps/android/arm64-v8a/SDL2/bin \
    -L../deps/android/arm64-v8a/SDL2_image/bin \
    -L../deps/android/arm64-v8a/sqlite3/lib \
    -L../CybObjects/libs/arm64-v8a \
    -L../CybMath/libs/arm64-v8a
LOCAL_LDLIBS += -lSDL2 -lSDL2_image -lsqlite3 -lCybObjects -lCybMath -lGLESv3
include $(BUILD_SHARED_LIBRARY)