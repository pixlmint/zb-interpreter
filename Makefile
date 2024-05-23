# Compiler and linker configurations
CC := gcc
CFLAGS := -Wall -I./lib -g
COVFLAGS := -fprofile-arcs -ftest-coverage
LDFLAGS := -L/usr/lib
TEST_LDLIBS := -lcunit -lgcov

# Directory definitions
BUILD_DIR := build
PROD_DIR := $(BUILD_DIR)/prod
TEST_DIR := $(BUILD_DIR)/test
SRC_DIR := lib
TEST_SRC_DIR := tests
TEST_OBJ_DIR := $(TEST_DIR)/obj
TEST_BIN_DIR := $(TEST_DIR)/bin
TEST_COV_DIR := $(TEST_DIR)/coverage

PROD_OBJ_DIR := $(PROD_DIR)/obj
PROD_BIN_DIR := $(PROD_DIR)/bin

# File lists
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
TEST_FILES := $(wildcard $(TEST_SRC_DIR)/*.c)

# Object files
PROD_OBJ_FILES := $(SRC_FILES:$(SRC_DIR)/%.c=$(PROD_OBJ_DIR)/%.o)
TEST_OBJ_SRC_FILES := $(SRC_FILES:$(SRC_DIR)/%.c=$(TEST_OBJ_DIR)/%.o)
TEST_OBJ_TEST_FILES := $(TEST_FILES:$(TEST_SRC_DIR)/%.c=$(TEST_OBJ_DIR)/%.o)
MAIN_OBJ := $(PROD_OBJ_DIR)/main.o

# Executables
EXEC := $(PROD_BIN_DIR)/zb
TEST_EXEC := $(TEST_BIN_DIR)/test_runner

# Default target
all: $(EXEC)

# Ensure the output directories exist
$(shell mkdir -p $(TEST_BIN_DIR) $(TEST_OBJ_DIR) $(TEST_COV_DIR) $(PROD_BIN_DIR) $(PROD_OBJ_DIR))

# Executable rules
$(EXEC): $(PROD_OBJ_FILES) $(MAIN_OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

$(TEST_EXEC): $(TEST_OBJ_SRC_FILES) $(TEST_OBJ_TEST_FILES)
	$(CC) $^ $(TEST_LDLIBS) -o $@ $(LDFLAGS) $(COVFLAGS)

$(PROD_OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
$(TEST_OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(TEST_OBJ_DIR)
	$(CC) $(CFLAGS) $(COVFLAGS) -c $< -o $@
$(TEST_OBJ_DIR)/%.o: $(TEST_SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(COVFLAGS) -c $< -o $@

$(MAIN_OBJ): main.c | $(PROD_OBJ_DIR)
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
	@lcov --directory $(TEST_OBJ_DIR) --capture --output-file $(TEST_COV_DIR)/coverage.info
	@lcov --list $(TEST_COV_DIR)/coverage.info  # This command will list what is actually captured before filtering.
	@lcov --ignore-errors unused --ignore-errors empty --remove $(TEST_COV_DIR)/coverage.info "tests/*" --output-file $(TEST_COV_DIR)/coverage_filtered.info
	@genhtml $(TEST_COV_DIR)/coverage_filtered.info --output-directory $(TEST_COV_DIR)
	@echo "Coverage report generated in $(TEST_COV_DIR)"

install: all
	@sudo cp build/prod/bin/zb /usr/local/bin/zb

# Cleaning up
clean:
	rm -rf $(BUILD_DIR)

# Phony targets
.PHONY: all clean run_tests test_leaks debug coverage

