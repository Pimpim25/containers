CC = g++
STD = -std=c++20
CC_FLAGS = -Wall -Werror -Wextra
DEBUG = -g
CC_TEST = --coverage
GTEST = -lgtest

TESTS = $(shell find . -type f -name "*.cc")

TESTS_TARGET = ./tests.out

.PHONY: test mem clean

test:
ifdef NOFLAGS
	$(CC) $(STD) $(DEBUG) $(TESTS) -o $(TESTS_TARGET) $(GTEST)
else
	$(CC) $(STD) $(CC_FLAGS) $(DEBUG) $(TESTS) -o $(TESTS_TARGET) $(GTEST)
endif
ifdef MODULE
	$(TESTS_TARGET) --gtest_filter=$(MODULE)*
else
	$(TESTS_TARGET)
endif

mem:
	valgrind -s --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all $(TESTS_TARGET)

clean:
	rm -f tests.out *.gcda *.gcno
