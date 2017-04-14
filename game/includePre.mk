BUILD_DIR = build

ifeq ($(RELEASE), 1)
	BUILD_TYPE = Release
	DBG_FLAGS = 

	BUILD_ARGS = RELEASE=1
	RELEASE = 1
else
	BUILD_TYPE = Debug
	DBG_FLAGS = -g

	BUILD_ARGS = RELEASE=0
	RELEASE = 0
endif

OBJS_DIR = $(BUILD_DIR)/$(BUILD_TYPE)


CC = g++
CFLAGS = -std=c++14
AR = ar
RANLIB = ranlib
RM = rm
CP = cp
MKDIR = mkdir

NAMESPACE = nan2

ROOT_OUT_DIR = $(PROJ_ROOT)/out/$(BUILD_TYPE)
ROOT_INC_DIR = $(PROJ_ROOT)/include
ROOT_INC_DIR_LIBREL = $(ROOT_INC_DIR)/$(NAMESPACE)
SCRIPTS_DIR = $(PROJ_ROOT)/scripts


DEPEND_FILE = $(OBJS_DIR)/depend_file

SUB_DIRS = $(addprefix $(PROJ_ROOT)/lib/, $(SUB_DIRS_STRIPED))
LIB_DIRS = -L./ $(addprefix -L, $(SUB_DIRS))
