LDFLAGS =-lcurses -lm
SRC =$(wildcard src/*.c)

build/game: $(SRC)
	gcc -o build/game $(SRC) $(LDFLAGS)

.PHONY: all test rebuild clean
	
test: rebuild
	build/game

rebuild: clean all

clean: 
	rm build/snake
