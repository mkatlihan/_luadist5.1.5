############################################################
# $Id$
LOCAL_PATH := $(call my-dir)

############################################################
include $(CLEAR_VARS)

# liblua_static module
$(warning Android make file: $(LOCAL_PATH)/Android.mk)
$(warning Android sysroot: $(SYSROOT))

LOCAL_MODULE := liblua_static
LOCAL_SRC_FILES :=  

LOCAL_CFLAGS := -DLUA_ANSI

LOCAL_CFLAGS += -fopenmp -fexceptions
LOCAL_LDFLAGS += -fopenmp
LOCAL_LDFLAGS += -latomic
LOCAL_LDFLAGS += -Wl,--allow-multiple-definition

LOCAL_CPP_INCLUDES = $(LOCAL_PATH)/etc
LOCAL_C_INCLUDES = $(LOCAL_PATH)/src

LOCAL_SRC_FILES += $(LOCAL_PATH)//src/lapi.c
LOCAL_SRC_FILES += $(LOCAL_PATH)//src/lcode.c
LOCAL_SRC_FILES += $(LOCAL_PATH)//src/ldebug.c
LOCAL_SRC_FILES += $(LOCAL_PATH)//src/ldo.c
LOCAL_SRC_FILES += $(LOCAL_PATH)//src/ldump.c
LOCAL_SRC_FILES += $(LOCAL_PATH)//src/lfunc.c
LOCAL_SRC_FILES += $(LOCAL_PATH)//src/lgc.c
LOCAL_SRC_FILES += $(LOCAL_PATH)//src/llex.c
LOCAL_SRC_FILES += $(LOCAL_PATH)//src/lmem.c
LOCAL_SRC_FILES += $(LOCAL_PATH)//src/lobject.c
LOCAL_SRC_FILES += $(LOCAL_PATH)//src/lopcodes.c
LOCAL_SRC_FILES += $(LOCAL_PATH)//src/lparser.c
LOCAL_SRC_FILES += $(LOCAL_PATH)//src/lstate.c
LOCAL_SRC_FILES += $(LOCAL_PATH)//src/lstring.c
LOCAL_SRC_FILES += $(LOCAL_PATH)//src/ltable.c
LOCAL_SRC_FILES += $(LOCAL_PATH)//src/ltm.c
LOCAL_SRC_FILES += $(LOCAL_PATH)//src/lundump.c
LOCAL_SRC_FILES += $(LOCAL_PATH)//src/lvm.c
LOCAL_SRC_FILES += $(LOCAL_PATH)//src/lzio.c
LOCAL_SRC_FILES += $(LOCAL_PATH)//src/lauxlib.c
LOCAL_SRC_FILES += $(LOCAL_PATH)//src/lbaselib.c
LOCAL_SRC_FILES += $(LOCAL_PATH)//src/ldblib.c
LOCAL_SRC_FILES += $(LOCAL_PATH)//src/liolib.c
LOCAL_SRC_FILES += $(LOCAL_PATH)//src/lmathlib.c
LOCAL_SRC_FILES += $(LOCAL_PATH)//src/loslib.c
LOCAL_SRC_FILES += $(LOCAL_PATH)//src/ltablib.c
LOCAL_SRC_FILES += $(LOCAL_PATH)//src/lstrlib.c
LOCAL_SRC_FILES += $(LOCAL_PATH)//src/linit.c
#LUA_USE_RELATIVE_LOADLIB 
#LOCAL_SRC_FILES += $(LOCAL_PATH)//src/loadlib_rel.c
LOCAL_SRC_FILES += $(LOCAL_PATH)//src/loadlib.c

include $(BUILD_STATIC_LIBRARY)


