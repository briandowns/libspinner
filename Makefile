cc ?= cc
CFLAGS = -std=c99 -O3 -Wall -lpthread 
LDFALGS = 

.PHONY: run-example
run-example:
	$(CC) $(CFLAGS) -o example examples/main.c spinner.c

.PHONY: test
test:
	$(CC) -o tests/tests tests/tests.c spinner.c tests/unity/unity.c
	tests/tests
	rm -f tests/tests
