all: build install

clean:
	-rm ./bin/* 2>/dev/null
	-rm ./obj/*.o 2>/dev/null

build: ./include ./src/main.c ./src/settings.c ./src/sudoku.c
	-mv ./obj/*.o . 2>/dev/null
	cc -I./include -g -c ./src/main.c ./src/settings.c ./src/sudoku.c -lpanel -lncurses -std=gnu99
	cc settings.o sudoku.o main.o -g -I./include -lpanel -lncurses -o sudoku
	mv *.o ./obj/ 2>/dev/null

install:
	-mv sudoku ./bin/ 2>/dev/null
