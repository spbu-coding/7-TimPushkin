include CONFIG.cfg

CC = gcc
LD = gcc
CPPFLAGS += -MMD -MP

TEST_INP_EXT = in
TEST_OTP_EXT = out
TEST_LOG_EXT = log
TEST_OK_STAT = ok
TEST_FL_STAT = fl

EXEC = $(BUILD_DIR)/$(NAME)
OBJS = $(BUILD_DIR)/main.o $(BUILD_DIR)/str_handler.o
DEPS = $(OBJS:.o=.d)
LOGS = $(patsubst $(TEST_DIR)/%.$(TEST_INP_EXT), $(TEST_DIR)/%.$(TEST_LOG_EXT), $(wildcard $(TEST_DIR)/*.$(TEST_INP_EXT)))

.PHONY: all check clean


all: $(EXEC)

$(EXEC): $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $^ -o $@

-include $(DEPS)

$(BUILD_DIR):
	mkdir -p $@


check: $(LOGS)
	@for log in $< ; do \
        if [ "$$(cat $${log})" != "$(TEST_OK_STAT)" ] ; then \
            exit 1 ; \
        fi ; \
    done

$(TEST_DIR)/%.$(TEST_LOG_EXT): $(TEST_DIR)/%.$(TEST_INP_EXT) $(TEST_DIR)/%.$(TEST_OTP_EXT) $(EXEC)
	@if [ "$$(./$(EXEC) ./$<)" = "$$(cat $(word 2, $^))" ] ; then \
        echo "$(TEST_OK_STAT)" > $@ ; \
        echo "Test $< - success" ; \
    else \
        echo "$(TEST_FL_STAT)" > $@ ; \
        echo "Test $< - fail" ; \
    fi


clean:
	$(RM) $(EXEC) $(OBJS) $(DEPS) $(LOGS)
