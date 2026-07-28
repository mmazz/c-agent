CC = gcc

CFLAGS = -Wall -g -Iinclude -MMD -MP

SERVER_DIR   = server
CLI_DIR   = cli
TEST_DIR  = tests
BUILD_DIR = build

AGENT_SRCS = $(wildcard $(SERVER_DIR)/*.c)
CLI_SRCS   = $(wildcard $(CLI_DIR)/*.c)
TEST_SRCS  = $(wildcard $(TEST_DIR)/*.c)

AGENT_OBJS = $(patsubst $(SERVER_DIR)/%.c,$(BUILD_DIR)/server/%.o,$(AGENT_SRCS))
CLI_OBJS   = $(patsubst $(CLI_DIR)/%.c,$(BUILD_DIR)/cli/%.o,$(CLI_SRCS))
TEST_OBJS  = $(patsubst $(TEST_DIR)/%.c,$(BUILD_DIR)/tests/%.o,$(TEST_SRCS))

# main.c del agente no debe linkearse con los tests
AGENT_MAIN = $(BUILD_DIR)/server/main.o
AGENT_LIB_OBJS = $(filter-out $(AGENT_MAIN),$(AGENT_OBJS))

AGENT = $(BUILD_DIR)/rasp-agent
CLI    = $(BUILD_DIR)/rasp-cli
TEST   = $(BUILD_DIR)/rasp-test

DEPS = \
	$(AGENT_OBJS:.o=.d) \
	$(CLI_OBJS:.o=.d) \
	$(TEST_OBJS:.o=.d)

all: $(AGENT) $(CLI) $(TEST)

$(AGENT): $(AGENT_OBJS)
	$(CC) $^ -o $@

$(CLI): $(CLI_OBJS)
	$(CC) $^ -o $@

$(TEST): $(TEST_OBJS) $(AGENT_LIB_OBJS)
	$(CC) $^ -o $@

$(BUILD_DIR)/server/%.o: $(SERVER_DIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/cli/%.o: $(CLI_DIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/tests/%.o: $(TEST_DIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

-include $(DEPS)

.PHONY: all clean
