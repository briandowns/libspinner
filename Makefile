cc = cc

CFLAGS  = -std=c99 -O3 -Wall 
LDFLAGS = -lpthread 

NAME    = libspinner

UNAME_S = $(shell uname -s)

# respect traditional UNIX path usage
INCDIR  = /usr/local/include
LIBDIR  = /usr/local/lib

ifeq ($(UNAME_S),Darwin)
$(NAME).dylib: clean
	$(CC) -c -dynamiclib -o $@ $(CFLAGS) $(LDFLAGS)
else
$(NAME).so: clean
	$(CC) -shared -o $@ $(CFLAGS) $(LDFLAGS)
endif

.PHONY: install
install: 
	cp spinner.h $(INCDIR)
ifeq ($(UNAME_S),Darwin)
	cp $(NAME).dylib $(LIBDIR)
else
	cp $(NAME).so $(LIBDIR)
endif

uninstall:
	rm -f $(INCDIR)/spinner.h
ifeq ($(UNAME_S),Darwin)
	rm -f $(INCDIR)/$(NAME).dylib
else
	rm -f $(INCDIR)/$(NAME).so
endif

.PHONY: test
test: clean
	$(CC) -g -o tests/tests spinner.c tests/tests.c tests/crosscheck.c $(LDFLAGS)
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
	$(CC) -o $@ spinner.c example.c $(CFLAGS) $(LDFLAGS)
