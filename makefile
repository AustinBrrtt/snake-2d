LDFLAGS =-lcurses -lm
SRC =$(wildcard src/*.c)

build/%: src/%.c
	gcc -o $@ $< $(LDFLAGS)

.PHONY: all test

all: $(addprefix build/,$(notdir $(basename $(SRC))))
	
test: build/myfind
	build/myfind test
