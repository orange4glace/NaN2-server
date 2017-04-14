makeall :
	@echo "$(CURDIR) $(MAKE_BATCH_SHELL_SCRIPT)"
	$(MAKE_BATCH_SHELL_SCRIPT) "all" "$(SUB_DIRS)" "$(BUILD_ARGS)"

dependall : depend
	$(MAKE_BATCH_SHELL_SCRIPT) "dependall" "$(SUB_DIRS)" "$(BUILD_ARGS)"

cleanall : clean
	$(MAKE_BATCH_SHELL_SCRIPT) "cleanall" "$(SUB_DIRS)" "$(BUILD_ARGS)"