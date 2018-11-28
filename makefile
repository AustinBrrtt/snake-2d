LDFLAGS =-lcurses -lm
SRC =$(wildcard *.c)

build/%: %.c
	gcc -o $@ $< $(LDFLAGS)

.PHONY: all test

all: $(addprefix build/,$(basename $(SRC)))
	
test: build/myfind
	build/myfind test