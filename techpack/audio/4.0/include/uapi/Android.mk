# Use this by setting
#   LOCAL_HEADER_LIBRARIES := audio_kernel_headers

LOCAL_PATH := $(call my-dir)
MYLOCAL_PATH := $(LOCAL_PATH)

UAPI_OUT := $(PRODUCT_OUT)/obj/vendor/qcom/opensource/audio-kernel/include

AUDIO_KERNEL_HEADERS := $(call all-named-files-under,*.h,linux) $(call all-named-files-under,*.h,sound)

HEADER_INSTALL_DIR := kernel/msm-$(TARGET_KERNEL_VERSION)/scripts

BUILD_ROOT_RELATIVE := ../../../../../../../
