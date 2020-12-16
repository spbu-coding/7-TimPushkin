include CONFIG.cfg

CC = gcc
LD = gcc

TEST_INP_EXT = in
TEST_OTP_EXT = out
TEST_TS_FILE = $(TEST_DIR)/last-check.ts

TARGET = $(BUILD_DIR)/$(NAME)
OBJS = $(BUILD_DIR)/main.o $(BUILD_DIR)/str_handler.o

.PHONY: all check clean

all: $(TARGET)

$(TARGET): $(OBJS) | $(BUILD_DIR)
	$(LD) $(LDFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $^ -o $@

check: $(TARGET)
	@TEST_FAILED=false ; \
	for inp in $(TEST_DIR)/*.$(TEST_INP_EXT) ; do \
        if  [ ! -f $(TEST_TS_FILE) ] || [ $${inp} -nt $(TEST_TS_FILE) ] || [ $(TARGET) -nt $(TEST_TS_FILE) ] ; then \
            if [ "$$(./$(TARGET) ./$${inp})" = "$$(cat $${inp%.*}.$(TEST_OTP_EXT))" ] ; then \
                echo "Test $${inp} - ok" ; \
            else \
                echo "Test $${inp} - failed" ; \
                TEST_FAILED=true ; \
            fi ; \
        fi \
    done ; \
    touch $(TEST_TS_FILE) ; \
    if $${TEST_FAILED} ; then \
        exit 1 ; \
    fi

$(BUILD_DIR):
	mkdir -p $@

clean:
	$(RM) $(TARGET) $(OBJS) $(TEST_TS_FILE)
