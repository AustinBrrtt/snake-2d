LDFLAGS =-lcurses -lm
SRC =$(wildcard src/*.c)

build/%: src/%.c
	gcc -o $@ $< $(LDFLAGS)

.PHONY: all test rebuild clean

all: $(addprefix build/,$(notdir $(basename $(SRC))))
	
test: rebuild
	build/snake

rebuild: clean all

clean: 
	rm build/snake
