CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -I$(INCLUDE_DIR) -I$(TEST_DIR)
GCOV_FLAGS = -fprofile-arcs -ftest-coverage -O0 -g

OS := $(shell uname -s)
ifeq ($(OS), Linux)
    TEST_FLAGS = -lcheck -lm -lpthread -lrt -lsubunit
else
    TEST_FLAGS = -lcheck -lm -lpthread
endif

SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = obj
TEST_DIR = tests

TARGET = arp_parser
TEST_BIN = test_runner

SRC_FILES = $(wildcard $(SRC_DIR)/*.c) main.c
TEST_SRC = $(wildcard $(TEST_DIR)/*.c)
LIB_SRC = $(wildcard $(SRC_DIR)/*.c)

.PHONY: all clean test gcov_report check_style format_style valgrind rebuild

all: $(TARGET)

$(TARGET): $(SRC_FILES)
	$(CC) $(CFLAGS) $^ -o $@

test: clean
	$(CC) $(CFLAGS) $(LIB_SRC) $(TEST_SRC) $(TEST_FLAGS) -o $(TEST_BIN)
	./$(TEST_BIN)

gcov_report: clean
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(LIB_SRC) $(TEST_SRC) $(TEST_FLAGS) -o $(TEST_BIN)
	-./$(TEST_BIN) || true
	lcov --capture --directory . --output-file coverage.info --rc lcov_branch_coverage=1
	lcov --remove coverage.info '*/tests/*' -o coverage.info --rc lcov_branch_coverage=1
	genhtml coverage.info --output-directory report_gcov --branch-coverage --rc lcov_branch_coverage=1
	@echo "Отчет создан: report_gcov/index.html"
	@case "$(OS)" in \
		"Linux") xdg-open report_gcov/index.html ;; \
		"Darwin") open report_gcov/index.html ;; \
		*) echo "Откройте отчет вручную: report_gcov/index.html" ;; \
	esac

clean:
	rm -rf $(OBJ_DIR) $(TARGET) $(TEST_BIN) coverage.info report_gcov
	find . -name "*.gcda" -delete
	find . -name "*.gcno" -delete

check_style:
	find . -name '*.h' -o -name '*.c' | xargs clang-format -n --style=Google

format_style:
	find . -name '*.h' -o -name '*.c' | xargs clang-format -i --style=Google

valgrind: test
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TEST_BIN)

rebuild: clean all