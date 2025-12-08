CC = gcc

CFLAGS_COMMON = -std=c11 -pedantic -Wall -Wextra
CFLAGS_RELEASE = $(CFLAGS_COMMON) -O2
CFLAGS_DEBUG = $(CFLAGS_COMMON) -O0 -g
CFLAGS_ASAN = $(CFLAGS_COMMON) -fsanitize=address

VG_FLAGS = --leak-check=full --track-origins=yes

DIR_SRC = src
DIR_TESTS = tests
DIR_BUILD = build

FILES_SRC = $(DIR_SRC)/task1.c

all: clean task1

$(DIR_BUILD):
	mkdir -p $(DIR_BUILD)

task1: $(DIR_BUILD) $(FILES_SRC)
	$(CC) $(CFLAGS_COMMON) $(FILES_SRC) -o $(DIR_BUILD)/$@

task1_test: $(DIR_BUILD) $(FILES_SRC)
	$(CC) $(CFLAGS_DEBUG) $(FILES_SRC) -o $(DIR_BUILD)/$@

task1_asan: $(DIR_BUILD) $(FILES_SRC)
	$(CC) $(CFLAGS_ASAN) $(FILES_SRC) -o $(DIR_BUILD)/$@

test: task1_test
	cd $(DIR_TESTS) && ./test_task1.sh

valgrind: task1_test
	valgrind $(VG_FLAGS) ./$(DIR_BUILD)/task1_test -w valgrind

asan: task1_asan
	ASAN_OPTIONS=detect_leaks=1 ./$(DIR_BUILD)/task1_asan -w asan

clean:
	rm -rf $(DIR_BUILD)

.PHONY: all task1 task1_test task1_asan clean