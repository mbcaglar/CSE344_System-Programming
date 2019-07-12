all: calistir


calistir: ls.o wc.o cat.o 141044073_main.o

		gcc ls.c -o ls && gcc wc.c -o wc && gcc cat.c -o cat && gcc 141044073_main.c -o 141044073_main

ls.o: ls.c
		gcc -c ls.c


wc.o: wc.c
		gcc -c wc.c

cat.o: cat.c
		gcc -c cat.c
	
141044073_main.o:141044073_main.c
		gcc -c 141044073_main.c


clean:
		rm *.o 141044073_main
