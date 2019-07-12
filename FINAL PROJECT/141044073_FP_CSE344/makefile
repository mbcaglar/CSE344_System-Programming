all: final

final:server.o client.o
	gcc server.o -o homeworkserver -lm -lpthread && gcc client.o -o clientApp -lm -lpthread

	gcc -c -std=c11 server.c -D_XOPEN_SOURCE

client.o:client.c
	gcc -c -std=c11 client.c -D_XOPEN_SOURCE

clean:
	rm homeworkserver && rm clientApp
