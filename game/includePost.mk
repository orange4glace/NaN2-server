LIBS = $(addprefix -lNan2, $(NAN2_LIBS)) $(addprefix -l, $(EXT_LIBS))
INC_ARGS = -I$(INC_PATH) -I$(ROOT_INC_DIR)
LIB_SRCS = $(addprefix $(SRC_PATH)/, $(LIB_SRCS_NAKED))
ENTRY_SRCS = $(addprefix $(SRC_PATH)/, $(ENTRY_SRCS_NAKED))
TARGET_LIBS = $(addprefix $(TARGET_PATH)/, $(TARGET_LIBS_NAKED))
TARGET_SRCS = $(addprefix $(TARGET_PATH)/, $(TARGET_SRCS_NAKED))
LIB_HEADERS := $(shell find $(INC_PATH) -name '*.h')
LIB_INC_COPIED_DIR = $(ROOT_INC_DIR)/$(NAMESPACE)/$(LIB_NAMESPACE)
LIB_INC_COPIED = $(addprefix $(LIB_INC_COPIED_DIR)/, $(LIB_HEADERS:$(INC_PATH)/%=%))

LIB_FULL_NAME = libNan2$(LIB_NAME).a
LIB_OBJS = $(LIB_SRCS:%.cpp=$(OBJS_DIR)/%.o)

ALL_LIBS = -lNan2$(LIB_NAME) $(LIBS)

TARGET_LIBS_OBJS = $(TARGET_LIBS:%.cpp=$(OBJS_DIR)/%.o)
TARGET_NAMES = $(TARGET_SRCS:%.cpp=$(OBJS_DIR)/%)
ENTRY_NAMES = $(ENTRY_SRCS:%.cpp=$(OBJS_DIR)/%)

DEPEND_SHELL_SCRIPT = $(SCRIPTS_DIR)/make_depend.sh
MAKE_BATCH_SHELL_SCRIPT = $(SCRIPTS_DIR)/make_all.sh

.SUFFIXES : .cpp .o

all : makeall PRE_CHECK $(LIB_INC_COPIED) depend $(LIB_FULL_NAME) $(ENTRY_NAMES) $(TARGET_LIBS_OBJS) $(TARGET_NAMES) clean_for_main

PRE_CHECK :
	@echo "==================================================="
	@echo "= Build started "
	@echo "= $(BUILD_TYPE) mode "
	@echo "==================================================="
	@echo "= Directory check "
	@`[ -d $(LIB_INC_COPIED_DIR) ] || $(MKDIR) -p $(LIB_INC_COPIED_DIR)`
	@`[ -d $(OBJS_DIR)/$(SRC_PATH) ] || $(MKDIR) -p $(OBJS_DIR)/$(SRC_PATH)`
	@`[ -d $(OBJS_DIR)/$(TARGET_PATH) ] || $(MKDIR) -p $(OBJS_DIR)/$(TARGET_PATH)`

$(LIB_INC_COPIED) :
	@echo "= Copying $@ "
	@`[ -d $(@D) ] || $(MKDIR) -p $(@D)`
	$(CP) $(@:$(LIB_INC_COPIED_DIR:./%=%)/%=$(INC_PATH)/%) $@

$(LIB_FULL_NAME) : $(LIB_OBJS)
	@echo "==================================================="
	@echo "= Packaging lib $@ "
	$(AR) rcv $@ $(LIB_OBJS)
	$(RANLIB) $@

$(OBJS_DIR)/%.o : %.cpp
	@echo "==================================================="
	@echo "= Compiling $@ "
	@`[ -d $(@D) ] || $(MKDIR) -p $(@D)`
	$(CC) $(CFLAGS) $(DBG_FLAGS) $(INC_ARGS) -c $< -o $@

.SECONDEXPANSION:
$(ENTRY_NAMES): $$@.o
	@echo "==================================================="
	@echo "= Linking $@ "
	@echo "$(CC) -o $@ $< $(LIB_DIRS) $(ALL_LIBS)"
	$(CC) -o $(@:$(OBJS_DIR)/$(SRC_PATH)/%=%) $< $(LIB_DIRS) $(ALL_LIBS)

.SECONDEXPANSION:
$(TARGET_NAMES): $$@.o
	@echo "==================================================="
	@echo "= Linking $@ "
	@echo "$(CC) -o $@ $< $(TARGET_LIBS_OBJS) $(LIB_DIRS) $(ALL_LIBS)"
	$(CC) -o $(@:$(OBJS_DIR)/%=%) $< $(TARGET_LIBS_OBJS) $(LIB_DIRS) $(ALL_LIBS)

depend :
	@`[ -d $(OBJS_DIR) ] || $(MKDIR) -p $(OBJS_DIR)`
	@echo "==================================================="
	@echo "= Depend $@ "
	@$(RM) -f $(DEPEND_FILE)
	$(DEPEND_SHELL_SCRIPT) \
		"$(LIB_SRCS:%.cpp=%) $(TARGET_SRCS:%.cpp=%) $(TARGET_LIBS:%.cpp=%)" \
		"$(CC)" "$(OBJS_DIR)" \
		"$(CFLAGS)" "$(DBG_FLAGS)" "$(INC_ARGS)" "$(DEPEND_FILE)"

clean_for_main :
ifeq ($(NOT_LIB), 1)
	$(RM) -fr $(LIB_FULL_NAME) $(LIB_INC_COPIED_DIR)
endif

clean :
	$(RM) -fr $(BUILD_DIR) $(LIB_FULL_NAME) $(LIB_INC_COPIED_DIR) $(ENTRY_SRCS_NAKED:%.cpp=%) $(TARGET_SRCS:%.cpp=%)

ifneq ($(MAKECMDGOALS), clean)
ifneq ($(MAKECMDGOALS), cleanall)
ifneq ($(MAKECMDGOALS), depend)
ifneq ($(MAKECMDGOALS), dependall)
ifneq ($(strip $(LIB_SRCS) $(TARGET_SRCS)),)
-include $(DEPEND_FILE)
endif
endif
endif
endif
endif

include $(PROJ_ROOT)/includeCommon.mk
