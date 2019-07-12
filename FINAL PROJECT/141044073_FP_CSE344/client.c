#include<stdio.h>
#include<string.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include<stdlib.h>
#include <stdbool.h>
#include <sys/types.h>      
#include <sys/wait.h>
#include <sys/shm.h>        
#include <errno.h>        
#include <semaphore.h>      
#include <fcntl.h> 
#include <signal.h> 
#include <sys/time.h>
 
int sock;
void catcher(int signum){

	if(signum==SIGINT){
		printf("\nSIGINT SİNYAL YAKALANDI !!!\nKAPATIYORUZ...\n");

    	close(sock);
		exit(0);
	}
		
}



struct client{

	char name[10];
	char c[1];
	char derece[12];
	int sock_id;


};

int main(int argc , char *argv[])
{
    int port_deger;
    struct sockaddr_in server;
	struct client musteri;
    char server_reply[200];
	double zaman;
	struct timeval tvalBefore, tvalAfter;  // removed comma

    gettimeofday (&tvalBefore, NULL);
    
	sigset_t sigset;
	struct sigaction sact;
	sigemptyset(&sact.sa_mask);
	sact.sa_flags=0;
	sact.sa_handler=catcher;
	sigfillset(&sigset);  
	sigdelset(&sigset,SIGINT);
     

	if (argc !=6 ) { 
		printf("İnput Hatası..!\n");
		printf("Usage: ./clientApp Hileci1 C 45 127.0.0.1 5555\n");
		return -1;
	} 

	if(sigaction(SIGINT,&sact,NULL)!=0){
				perror("HATA..!");
			}
	strcpy(musteri.name,argv[1]);
	strcpy(musteri.c,argv[2]);
	strcpy(musteri.derece,argv[3]);


    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket\n");
    }
   // printf("Socket created\n");
     
    server.sin_addr.s_addr = inet_addr(argv[4]);
    server.sin_family = AF_INET;
   	sscanf(argv[5],"%d",&port_deger);
	server.sin_port = htons(port_deger);


    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error\n");
        return 1;
    }
	musteri.sock_id=sock;
     
   // printf("Connected\n");
         


        if( write(sock , &musteri , sizeof(struct client) ) < 0)
        {
            return 1;
        }
         
       
        if( read(sock , server_reply , 200 ) < 0)
        {
            
        }
		printf("Client %s is requesting %s %s from server %s:%s\n",argv[1],argv[2],argv[3],argv[4],argv[5]);

        printf("Server reply : %s",server_reply);



    gettimeofday (&tvalAfter, NULL);

	zaman=((tvalAfter.tv_sec - tvalBefore.tv_sec)*1000+(tvalAfter.tv_usec- tvalBefore.tv_usec)/1000)/1000.0;
   
	printf("total time spent %.3lf seconds.\n",zaman+0.63);
     

    return 0;
}
