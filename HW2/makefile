all:calistir

calistir: main.o
		  gcc 141044073_main.o -o multiprocess_DFT
main.o:
		  gcc -c 141044073_main.c -D_POSIX_C_SOURCE -pedantic-errors -Wall -std=c11

clean:
		rm *.o	multiprocess_DFT
