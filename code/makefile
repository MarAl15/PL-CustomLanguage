.SUFFIXES:

generacion: main.o y.tab.o semantico.o generacion.o
	gcc -o generacion main.o y.tab.o semantico.o generacion.o

semantico.o: semantico.c
	gcc -c -o semantico.o semantico.c

generacion.o: generacion.c
	gcc -c -o generacion.o generacion.c

y.tab.o: y.tab.c
	gcc -c y.tab.c

main.o: main.c
	gcc -c main.c
	
y.tab.c: sintactico.y lex.yy.c
	bison -v -Wconflicts-sr -Wconflicts-rr -o y.tab.c sintactico.y

lex.yy.c: esp.l
	flex -l esp.l

limpia: 
	rm -f generacion main.o y.tab.o y.tab.c lex.yy.c semantico.o y.output generacion.o codigo codigo.c

todo: 
	reset
	make limpia
	make generacion
