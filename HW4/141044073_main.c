/**************************************************************/
/*                M.BEDİRHAN ÇAĞLAR                           */
/*                   141044073                                */
/*                                                            */
/*					30.03.2018                                */
/*                                                            */
/* Bu program verilen toptancının ortaya random malzeme       */
/* koyarak  o malzemeye ihtiyacı olan chef(process)'in 	      */
/* malzemeyi alıp şekerpare yapmasının iletişimini sağlar     */
/*                                                            */
/*                                                            */
/**************************************************************/



/****************************************************************/
/*                      MALZEMELER                              */
/*      0 -> eggs                                               */
/*      1 -> flour                                              */
/*      2 -> butter                                             */
/*      3 -> sugar                                              */
/*                                                              */
/* Random sayılara karşılık gelen malzemeler yukarıdaki gibidir.*/
/****************************************************************/



#include <stdio.h> 
#include <stdlib.h>         
#include <unistd.h>
#include <sys/types.h>      
#include <sys/wait.h>
#include <sys/shm.h>        
#include <errno.h>          
#include <semaphore.h>      
#include <fcntl.h>                   
#include <time.h>
#include <string.h>


void print_chef();
void print_malzeme(char arr[10]);


sem_t *sem;
sem_t *sem2;

int shmid;
char *p;




void catcher(int signum){

	if(signum==SIGINT){
		printf("\nSIGINT SİNYAL YAKALANDI !!!\nKAPATIYORUZ...\n");
		shmdt (p);
        shmctl (shmid, IPC_RMID, 0);     
        sem_destroy (sem);
        sem_destroy (sem2);
		kill(0, SIGKILL);
		
	}
		
}


int main (int argc, char **argv){


	char tmp1[10];
	char tmp2[10];
    int i,j;                        
    key_t shmkey;                
                       
                 
    pid_t pid;                  
                       
    unsigned int n;             
    unsigned int value;
	sigset_t sigset;
	struct sigaction sact;
	sigemptyset(&sact.sa_mask);
	sact.sa_flags=0;
	sact.sa_handler=catcher;
	sigfillset(&sigset);      

    shmkey = ftok("/tmp", 5);    
   
    shmid = shmget (shmkey, 10, 0644 | IPC_CREAT);
    if (shmid < 0){         
        perror ("shmget\n");
        exit (1);
    }

    p = (char *) shmat (shmid, NULL, 0);  
    p[0]='x';
	p[1]='x';

	srand(time(NULL)); 

    sem = sem_open ("child_sem", O_CREAT | O_EXCL, 0644, 0);
    sem2=sem_open ("parent_sem", O_CREAT | O_EXCL, 0644, 1);

    sem_unlink ("parent_sem");
	sem_unlink("child_sem");      


	i=0;

	while(i<6){

        pid = fork ();
        if (pid < 0)              
            perror ("Fork error.\n");
        else if (pid == 0)
            break;                 
		i++;
	}

sigdelset(&sigset,SIGINT);
while(1){

    if (pid != 0){


			

			if(sigaction(SIGINT,&sact,NULL)!=0){
				perror("HATA..!");
			}
	
			sem_wait (sem2);

			int malzeme[2];
			malzeme[0]=rand()%4;
			p[0]=malzeme[0] + '0';

			

			while((malzeme[1]=rand()%4)==malzeme[0]);

			p[1]=malzeme[1] + '0';
			print_chef();
			print_malzeme(p);
	
			sem_post(sem);

    }

    else{

		    if(i==0){
				 sem_wait (sem); 
				if(strncmp(p,"23",2)==0 || strncmp(p,"32",2)==0){

					if(p[0]=='2'){
						strcpy(tmp1,"butter");
						printf("chef(%d) has taken the %s\n",i+1,tmp1);

					}else{
						strcpy(tmp1,"sugar");

						printf("chef(%d) has taken the %s\n",i+1,tmp1);

					}
					printf("wholesaler is waiting for the dessert\n");
					if(p[1]=='2'){
						strcpy(tmp2,"butter");
						printf("chef(%d) has taken the %s\n",i+1,tmp2);

					}else{
						strcpy(tmp2,"sugar");
						printf("chef(%d) has taken the %s\n",i+1,tmp2);

					}
					printf("chef(%d) is preparing the dessert\n",i+1);
					printf("chef(%d) has delivered the dessert to the wholesaler\nwholesaler has obtained the dessert and left to sell it\n",i+1);
					printf("chef(%d) is waiting for %s and %s\n",i+1,tmp1,tmp2);
					printf("-----------------------------------------------------\n");

					sem_post(sem2);
				}else{
					sem_post(sem);
				}

			}
			else if(i==1){
 				sem_wait (sem); 
				if(strncmp(p,"13",2)==0 || strncmp(p,"31",2)==0){

					if(p[0]=='1'){
						strcpy(tmp1,"flour");
						printf("chef(%d) has taken the %s\n",i+1,tmp1);

					}else{
						strcpy(tmp1,"sugar");
						printf("chef(%d) has taken the %s\n",i+1,tmp1);

					}
					printf("wholesaler is waiting for the dessert\n");
					if(p[1]=='1'){
						strcpy(tmp2,"flour");
						printf("chef(%d) has taken the %s\n",i+1,tmp2);

					}else{
						strcpy(tmp2,"sugar");
						printf("chef(%d) has taken the %s\n",i+1,tmp2);

					}
					printf("chef(%d) is preparing the dessert\n",i+1);
					printf("chef(%d) has delivered the dessert to the wholesaler\nwholesaler has obtained the dessert and left to sell it\n",i+1);
					printf("chef(%d) is waiting for %s and %s\n",i+1,tmp1,tmp2);
					printf("-----------------------------------------------------\n");
					sem_post(sem2);
				}else{
					sem_post(sem);
				}
			}
			else if(i==2){
 				sem_wait (sem); 
				if(strncmp(p,"12",2)==0 || strncmp(p,"21",2)==0){

					if(p[0]=='1'){
						strcpy(tmp1,"flour");
						printf("chef(%d) has taken the %s\n",i+1,tmp1);

					}else{
						strcpy(tmp1,"butter");
						printf("chef(%d) has taken the %s\n",i+1,tmp1);

					}
					printf("wholesaler is waiting for the dessert\n");
					if(p[1]=='1'){
						strcpy(tmp2,"flour");

						printf("chef(%d) has taken the %s\n",i+1,tmp2);

					}else{
						strcpy(tmp2,"butter");

						printf("chef(%d) has taken the %s\n",i+1,tmp2);

					}
					printf("chef(%d) is preparing the dessert\n",i+1);
					printf("chef(%d) has delivered the dessert to the wholesaler\nwholesaler has obtained the dessert and left to sell it\n",i+1);
					printf("chef(%d) is waiting for %s and %s\n",i+1,tmp1,tmp2);
					printf("-----------------------------------------------------\n");
					sem_post(sem2);
				}else{
					sem_post(sem);
				}

			}
			else if(i==3){
 				sem_wait (sem); 
				if(strncmp(p,"03",2)==0 || strncmp(p,"30",2)==0){

					if(p[0]=='0'){
						strcpy(tmp1,"eggs");
						printf("chef(%d) has taken the %s\n",i+1,tmp1);

					}else{
						strcpy(tmp1,"sugar");
						printf("chef(%d) has taken the %s\n",i+1,tmp1);

					}
					printf("wholesaler is waiting for the dessert\n");
					if(p[1]=='0'){
						strcpy(tmp2,"eggs");
						printf("chef(%d) has taken the %s\n",i+1,tmp2);

					}else{
						strcpy(tmp2,"sugar");
						printf("chef(%d) has taken the %s\n",i+1,tmp2);

					}
					printf("chef(%d) is preparing the dessert\n",i+1);
					printf("chef(%d) has delivered the dessert to the wholesaler\nwholesaler has obtained the dessert and left to sell it\n",i+1);
					printf("chef(%d) is waiting for %s and %s\n",i+1,tmp1,tmp2);
					printf("-----------------------------------------------------\n");
					sem_post(sem2);
				}else{
					sem_post(sem);
				}

			}
			else if(i==4){

 				sem_wait (sem); 
				if(strncmp(p,"01",2)==0 || strncmp(p,"10",2)==0){

					if(p[0]=='0'){
						strcpy(tmp1,"eggs");
						printf("chef(%d) has taken the %s\n",i+1,tmp1);

					}else{
						strcpy(tmp1,"flour");
						printf("chef(%d) has taken the %s\n",i+1,tmp1);

					}
					printf("wholesaler is waiting for the dessert\n");
					if(p[1]=='0'){
						strcpy(tmp2,"eggs");
						printf("chef(%d) has taken the %s\n",i+1,tmp2);

					}else{
						strcpy(tmp2,"flour");
						printf("chef(%d) has taken the %s\n",i+1,tmp2);

					}
					printf("chef(%d) is preparing the dessert\n",i+1);
					printf("chef(%d) has delivered the dessert to the wholesaler\nwholesaler has obtained the dessert and left to sell it\n",i+1);
					printf("chef(%d) is waiting for %s and %s\n",i+1,tmp1,tmp2);
					printf("-----------------------------------------------------\n");
					sem_post(sem2);
				}else{
					sem_post(sem);
				}
			}
			else if(i==5){
 				sem_wait (sem); 
				if(strncmp(p,"02",2)==0 || strncmp(p,"20",2)==0){

					if(p[0]=='0'){
						strcpy(tmp1,"eggs");
						printf("chef(%d) has taken the %s\n",i+1,tmp1);

					}else{
						strcpy(tmp1,"butter");
						printf("chef(%d) has taken the %s\n",i+1,tmp1);

					}
					printf("wholesaler is waiting for the dessert\n");
					if(p[1]=='0'){
						strcpy(tmp2,"eggs");
						printf("chef(%d) has taken the %s\n",i+1,tmp2);

					}else{
						strcpy(tmp2,"butter");
						printf("chef(%d) has taken the %s\n",i+1,tmp2);

					}
					printf("chef(%d) is preparing the dessert\n",i+1);
					printf("chef(%d) has delivered the dessert to the wholesaler\nwholesaler has obtained the dessert and left to sell it\n",i+1);
					printf("chef(%d) is waiting for %s and %s\n",i+1,tmp1,tmp2);
					printf("-----------------------------------------------------\n");
					sem_post(sem2);
				}else{
					sem_post(sem);
				}
			}else{
					sem_post(sem);

 		}    
	}
}   
 return 0;  
}

void print_chef(){

	printf("-----------------------------------------------------\n");
	printf("chef(1) is waiting for butter and sugar\n");
	printf("chef(2) is waiting for flour and sugar\n");
	printf("chef(3) is waiting for flour and butter\n");
	printf("chef(4) is waiting for eggs and sugar\n");
	printf("chef(5) is waiting for eggs and flour\n");
	printf("chef(6) is waiting for eggs and butter\n");
   printf("-----------------------------------------------------\n");
return;
}

void print_malzeme(char arr[10]){

			printf("wholesaler delivers ");

			if(strncmp(arr,"01",2)==0 ){
				printf("eggs and flour\n");
			}
			else if(strncmp(arr,"10",2)==0){
				printf("flour and eggs\n");
			}

			else if(strncmp(arr,"02",2)==0){
				printf("eggs and butter\n");
			}

			else if(strncmp(arr,"20",2)==0){
				printf("butter and eggs\n");
			}

			else if(strncmp(arr,"30",2)==0){
				printf("sugar and eggs\n");
			}


			else if(strncmp(arr,"03",2)==0){
				printf("eggs and sugar\n");
			}

			else if(strncmp(arr,"21",2)==0){
				printf("butter and flour\n");
			}

			else if(strncmp(arr,"12",2)==0){
				printf("flour and butter\n");
			}
			else if(strncmp(arr,"32",2)==0){
				printf("sugar and butter\n");
			}

			else if(strncmp(arr,"23",2)==0 ){
				printf("butter and sugar\n");
			}

			else if(strncmp(arr,"31",2)==0){
				printf("sugar and flour\n");
			}

			else if(strncmp(arr,"13",2)==0){
				printf("flour and sugar\n");
			}

return;


}



