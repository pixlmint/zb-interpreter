# Compiler and linker configurations
CC = gcc
CFLAGS = -Wall -I./lib -fprofile-arcs -ftest-coverage -g
LDFLAGS = -L/usr/lib -lcunit -lgcov

# Directory definitions
SRC_DIR = lib
TEST_DIR = tests
OBJ_DIR = obj
BIN_DIR = bin
COV_DIR = coverage

# Automatically list source and test files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
TEST_FILES = $(wildcard $(TEST_DIR)/*.c)

# Object files for sources and tests
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
TEST_OBJ_FILES = $(TEST_FILES:$(TEST_DIR)/%.c=$(OBJ_DIR)/%.o)

# Test executable output
TEST_EXEC = $(BIN_DIR)/test_runner

# Default target
all: tests

# Ensure the output directories exist
$(shell mkdir -p $(BIN_DIR) $(OBJ_DIR) $(COV_DIR))

# Rule to make the test executable
tests: $(OBJ_FILES) $(TEST_OBJ_FILES)
	$(CC) $(OBJ_FILES) $(TEST_OBJ_FILES) -o $(TEST_EXEC) $(LDFLAGS)

# Rule to compile source files into objects
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to compile test files into objects
$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Target to run tests
run_tests: tests
	@./$(TEST_EXEC)
	@make coverage

test_leaks: tests $(TEST_EXEC)
	@echo "Checking for memory leaks..."
	@valgrind --track-origins=yes --leak-check=full ./$(TEST_EXEC)

# Clean up the generated files
clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)

debug: tests
	gdb $(BIN_DIR)/test_runner

# Coverage analysis
coverage:
	@echo "Generating coverage report..."
	lcov --directory $(OBJ_DIR) --capture --output-file $(COV_DIR)/coverage.info
	lcov --ignore-errors unused --remove $(COV_DIR)/coverage.info "/usr/*" --remove $(COV_DIR)/coverage.info "tests/*" --output-file $(COV_DIR)/coverage_filtered.info
	genhtml $(COV_DIR)/coverage_filtered.info --output-directory $(COV_DIR)
	@echo "Coverage report generated in $(COV_DIR)"


# Phony targets for clarity and to avoid conflicts
.PHONY: all tests run_tests clean coverage test_leaks

