cc = cc

CFLAGS  = -std=c99 -O3 -Wall 
LDFLAGS = -lpthread 

NAME    = libspinner

UNAME_S = $(shell uname -s)

# respect traditional UNIX path usage
INCDIR  = /usr/local/include
LIBDIR  = /usr/local/lib

.PHONY: install
install: 
	cp spinner.h $(INCDIR)

uninstall:
	rm -f $(INCDIR)/spinner.h

.PHONY: test
test: clean
	$(CC) -g -o tests/tests tests/tests.c tests/crosscheck.c $(LDFLAGS)
	tests/tests
	rm -f tests/tests

.PHONY: clean
clean:
	rm -f $(NAME).dylib
	rm -f $(NAME).so
	rm -f example
	rm -f tests/tests

.PHONY: example
example: clean
	$(CC) -o $@ example.c $(CFLAGS) $(LDFLAGS)
