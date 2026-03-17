TESTS_DIR = unit_tests

.PHONY: test mem clean

test:
	$(MAKE) -C $(TESTS_DIR) test

mem:
	$(MAKE) -C $(TESTS_DIR) mem

clean:
	$(MAKE) -C $(TESTS_DIR) clean

clang-format-check:
	cp ./linters/.clang-format .
	clang-format -n *.h ./unit_tests/*.cc ./unit_tests/*.h
	rm ./.clang-format

clang-format-apply:
	cp ./linters/.clang-format .
	clang-format -i *.h ./unit_tests/*.cc ./unit_tests/*.h
	rm ./.clang-format
