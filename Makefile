CC = gcc

CFLAGS = -Wall -g -Iinclude -MMD -MP

SRC_DIR = src
CLI_DIR = cli
BUILD_DIR = build

AGENT_SRCS = $(wildcard $(SRC_DIR)/*.c)
CLI_SRCS = $(wildcard $(CLI_DIR)/*.c)

AGENT_OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/src/%.o,$(AGENT_SRCS))
CLI_OBJS = $(patsubst $(CLI_DIR)/%.c,$(BUILD_DIR)/cli/%.o,$(CLI_SRCS))

AGENT = $(BUILD_DIR)/rasp-agent
CLI = $(BUILD_DIR)/rasp-cli

all: $(AGENT) $(CLI)

$(AGENT): $(AGENT_OBJS)
	$(CC) $^ -o $@

$(CLI): $(CLI_OBJS)
	$(CC) $^ -o $@


$(BUILD_DIR)/src/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)/src
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/cli/%.o: $(CLI_DIR)/%.c
	mkdir -p $(BUILD_DIR)/cli
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -rf $(BUILD_DIR)

-include $(OBJS:.o=.d)

.PHONY: all clean
