cc      = cc
CFLAGS  = -std=c99 -O3 -Wall 
LDFALGS = -lpthread 
INCDIR  = /usr/local/include
LIBDIR  = /usr/local/lib

NAME    = libspinner

UNAME_S = $(shell uname -s)

ifeq ($(UNAME_S),Linux)
$(NAME).so:
	$(CC) -shared -o $(NAME).so spinner.c $(CFLAGS)
endif
ifeq ($(UNAME_S),Darwin)
$(NAME).dylib:
	$(CC) -c -dynamiclib -o $(NAME).so spinner.c $(CFLAGS) $(LDFLAGS)
endif

.PHONY: install
install: 
	cp spinner.h $(INCDIR)
ifeq ($(UNAME_S),Linux)
	cp spinner.h $(INCDIR)
	cp $(NAME).so $(LIBDIR)
endif
ifeq ($(UNAME_S),Darwin)
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
	$(CC) -o tests/tests tests/tests.c spinner.c tests/unity/unity.c
	tests/tests
	rm -f tests/tests

.PHONY: run-example
run-example:
	$(CC) $(CFLAGS) -o example examples/main.c spinner.c
