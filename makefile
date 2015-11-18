CC=gcc -lm
OBJ= resolve.o SymbolList.o

resolve: resolve.o SymbolList.o
	gcc -lm resolve.o SymbolList.o -o resolve

resolve.o: resolve.c resolve.h
	gcc -c resolve.c

SymbolList.o: SymbolList.c symbolList.h
	gcc -c SymbolList.c

clean:
	rm -f *.o
