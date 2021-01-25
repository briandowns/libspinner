cc      = cc
CFLAGS  = -std=c99 -O3 -Wall 
LDFLAGS = -lpthread 
INCDIR  = /usr/local/include
LIBDIR  = /usr/local/lib

NAME    = libspinner

UNAME_S = $(shell uname -s)

all: $(NAME).so

ifeq ($(UNAME_S),Darwin)
$(NAME).dylib:
	$(CC) -c -dynamiclib -o $(NAME).so $(CFLAGS) $(LDFLAGS)
else
$(NAME).so:
	$(CC) -shared -o $(NAME).so $(CFLAGS)
endif

.PHONY: install
install: 
	cp spinner.h $(INCDIR)
ifeq ($(UNAME_S),Linux)
	cp spinner.h $(INCDIR)
	cp $(NAME).so $(LIBDIR)
else
	cp spinner.h $(INCDIR)
	cp $(NAME).so $(LIBDIR)
endif

uninstall:
	rm -f $(INCDIR)/spinner.h
ifeq ($(UNAME_S),Linux)
	rm -f $(INCDIR)/$(NAME).so
endif
ifeq ($(UNAME_S),Darwin)
	rm -f $(INCDIR)/$(NAME).so
endif

.PHONY: test
test:
	$(CC) -o tests/tests tests/tests.c tests/unity/unity.c $(LDFLAGS)
	tests/tests
	rm -f tests/tests

.PHONY: clean
clean:
	rm -f libspinner.so
	rm -f example

.PHONY: run-example
run-example:
	$(CC) $(CFLAGS) -o example examples/main.c

