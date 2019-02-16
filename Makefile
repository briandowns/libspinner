cc ?= cc
CFLAGS = -Wall -lpthread
LDFALGS = 

.PHONY: clean
clean:
	rm -f $(BINDIR)/$(BINARY)

.PHONY: run-example
run-example:
	$(CC) $(CFLAGS) -o example examples/main.c spinner.c

.PHONY: test
test: clean
	$(CC) -o tests/tests tests/tests.c spinner.c tests/unity/unity.c
	tests/tests
	rm -f tests/tests
