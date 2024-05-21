# Compiler and linker configurations
CC := gcc
CFLAGS := -Wall -I./lib -g
LDFLAGS := -L/usr/lib
LDLIBS := -lcunit -lgcov

# Directory definitions
BUILD_DIR := build
SRC_DIR := lib
TEST_DIR := tests
OBJ_DIR := $(BUILD_DIR)/obj
BIN_DIR := $(BUILD_DIR)/bin
COV_DIR := $(BUILD_DIR)/coverage

# File lists
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
TEST_FILES := $(wildcard $(TEST_DIR)/*.c)

# Object files
OBJ_FILES := $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
TEST_OBJ_FILES := $(TEST_FILES:$(TEST_DIR)/%.c=$(OBJ_DIR)/%.o)
MAIN_OBJ := $(OBJ_DIR)/main.o

# Executables
EXEC := $(BIN_DIR)/zb
TEST_EXEC := $(BIN_DIR)/test_runner

# Default target
all: $(EXEC)

# Ensure the output directories exist
$(shell mkdir -p $(BIN_DIR) $(OBJ_DIR) $(COV_DIR))

# Executable rules
$(EXEC): $(OBJ_FILES) $(MAIN_OBJ)
	$(CC) $^ $(LDLIBS) -o $@ $(LDFLAGS)

$(TEST_EXEC): $(filter-out $(MAIN_OBJ), $(OBJ_FILES)) $(TEST_OBJ_FILES)
	$(CC) $^ $(LDLIBS) -o $@ $(LDFLAGS)

# Object file rules
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(MAIN_OBJ): main.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Testing and Debugging
run_tests: $(TEST_EXEC)
	@./$<
	@$(MAKE) coverage

test_leaks: $(TEST_EXEC)
	@echo "Checking for memory leaks..."
	@valgrind --track-origins=yes --leak-check=full ./$<

debug: $(TEST_EXEC)
	gdb $<

# Coverage analysis
coverage:
	@echo "Generating coverage report..."
	@lcov --directory $(OBJ_DIR) --capture --output-file $(COV_DIR)/coverage.info
	@lcov --remove $(COV_DIR)/coverage.info "/usr/*" "tests/*" --output-file $(COV_DIR)/coverage_filtered.info
	@genhtml $(COV_DIR)/coverage_filtered.info --output-directory $(COV_DIR)
	@echo "Coverage report generated in $(COV_DIR)"

# Cleaning up
clean:
	rm -rf $(BUILD_DIR)

# Phony targets
.PHONY: all clean run_tests test_leaks debug coverage

