LDFLAGS =-lcurses -lm
SRC =$(wildcard src/*.c)

build/game: $(SRC)
	gcc -o build/game $(SRC) $(LDFLAGS)

.PHONY: debug setdebugflag test rebuild clean

debug: clean $(SRC)
	gcc -g -o build/game $(SRC) $(LDFLAGS)
	gdb -ex --se build/game
	
	
test: rebuild
	build/game

rebuild: clean build/game

clean: 
	rm build/game -f > /dev/null
