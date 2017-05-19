PROJ_ROOT = .
LIB_NAME = Main
LIB_NAMESPACE = main
NOT_LIB = 1

include $(PROJ_ROOT)/includePre.mk

SRC_PATH = src
INC_PATH = include
TARGET_PATH = test

LIB_SRCS_NAKED = test_class.cpp
ENTRY_SRCS_NAKED = main.cpp
TARGET_SRCS_NAKED = test.cpp

SUB_DIRS_STRIPED = math world

# nan2 internal libs
NAN2_LIBS += Math World

# external libs
EXT_LIBS += boost_log boost_system boost_thread cpp_redis tacopie

include $(PROJ_ROOT)/includePost.mk
