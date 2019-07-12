#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <signal.h>

	FILE *f_parent;
	FILE *f_child;

int flag=0;
off_t fsize(const char *filename) {
    struct stat st; 

    if (stat(filename, &st) == 0)
        return st.st_size;

    return -1; 
}

void catcher(int signum) {
	switch (signum) {
		case SIGUSR1: 
						printf("\n1-catcher caught unexpected signal %d\n",signum);
  
						break;
		case SIGUSR2: 
						printf("\n2-catcher caught unexpected signal %d\n",signum);
  
	break;
		case SIGINT:
				if(flag==0){
					flag=1;
					printf("\nHer şeyi kapatıyoruz---> CTRL_C  %d\n ",signum);
					fclose(f_parent);
					fclose(f_child);
					exit(0);
					}			
					break;

		default: break; 
	} 
}

int line=0;
int main(int argc, char *argv[])
{
    //Create an variable of type struct flock to define the properties of locking 
 
	pid_t child;
	int fileSize;
	int boyut,size;	
	char buffer[1000];
	char tmp[1000];
	int N,M;
	char tmp2[1000];
	int fd;
	char new='\n',bos=' ';
	int random=0;

	f_parent=fopen("parent_log.txt","a");
	f_child=fopen("child_log.txt","a");


	struct sigaction sact_p;
	sigset_t sigset;
	struct sigaction sact_c;
	
	struct flock fl;

	int i=0,p=1,k=0,t=0;


	if (argc !=7 ) { 
		printf("İnput Hatası..!\n");
		printf("Usage: ./multiprocess_DFT -N 5 -X file.dat -M 100 \n");
		return -1;
	}

	N=atoi(argv[2])+1;
	M=atoi(argv[6]);
	fd= open(argv[4], O_RDWR);
	srand(time(0));

  if (fd==-1) {
    perror("Can't open stories.txt");
	exit(0);
  }
	child=fork();	

	if(child==0){

		while(1){
	

	
			memset(&fl,0,sizeof(fl));
			fl.l_type = F_RDLCK;

			fcntl(fd, F_SETLKW, &fl);	


			i=0;
			k=0;
			p=1;
			for(t=0; t<M; t++){
	
				while(i<(((N-1)*p)+k)){

					int j = rand()%10;
					sprintf(buffer+i, "%d", j);
					i++;
					
				}
				k++;
				sprintf(buffer+i,"%c",new);
				i++;
				p++;
								
			}
			fileSize=strlen(buffer);		
			p=1;
		    write(fd, buffer, strlen(buffer));

			for(int t=0; t<fileSize; t++){

				if(p==1 && t==0){

					printf("Process A: i’m producing a random sequence for line %d : ",p);
					fprintf(f_child, "Process A: i’m producing a random sequence for line %d : ",p);
				}
				if((p*(N))==(t+1) && (t+1)!=fileSize){

					p++;
					printf("\nProcess A: i’m producing a random sequence for line %d : ",p);
					fprintf(f_child, "\nProcess A: i’m producing a random sequence for line %d : ",p);
				}
				if(buffer[t]!='\n'){
					printf("%c ",buffer[t]);
					fprintf(f_child, "%c ",buffer[t]);
				}


			}
			//sleep(1);

		    fl.l_type = F_UNLCK;
			fcntl(fd, F_SETLKW, &fl);
		
			kill(getppid(),SIGUSR1);
				
			sigemptyset(&sact_c.sa_mask);
			sact_c.sa_flags = 0;
			sact_c.sa_handler = catcher;

			if(sigaction(SIGUSR2,&sact_c,NULL)==-1)
				perror("hata usr1");
	
		else if(sigaction(SIGINT,&sact_c,NULL)==-1)
				perror("hata usr1");			
			else{

				sigfillset(&sigset);
				sigdelset(&sigset,SIGUSR2);
				sigdelset(&sigset,SIGINT);
			
				printf("\nsinyal beekliyor -- child\n");
				if(sigsuspend(&sigset)==-1)
					perror("sigsuspend()");

			}
	
	}

}

while(1){

	sigemptyset(&sact_p.sa_mask);
	sact_p.sa_flags = 0;
	sact_p.sa_handler = catcher;

	if(sigaction(SIGUSR1,&sact_p,NULL)==-1)
		perror("hata usr1");
	
	else if(sigaction(SIGINT,&sact_p,NULL)==-1)
		perror("hata usr1");			

	else{

		sigfillset(&sigset);
		sigdelset(&sigset,SIGUSR1);
		sigdelset(&sigset,SIGINT);
		printf("\nsinyal beekliyor -- parent\n");
		if(sigsuspend(&sigset)==-1)
			perror("sigsuspend()");

	}

		memset(&fl,0,sizeof(fl));
	
		fl.l_type = F_WRLCK; // set to unlock same region


		fcntl(fd, F_SETLK, &fl);

	t=1;
	boyut=(int)fsize(argv[4]);
	size=boyut;

random=1+rand()%5;
	while(t*N < (boyut-random)){


		lseek(fd,(-N*t),SEEK_END);

		read(fd,tmp,N);
	
		line=(size/N);
		printf("\nProcess B: i’m producing a random sequence for line %d : ",line);
		fprintf(f_parent, "\nProcess B: i’m producing a random sequence for line %d : ",line);

		lseek(fd,(-N*t),SEEK_END);
		for(k=0; k<N; k++){

			if(tmp[k]!='\n')
				printf("%c ",tmp[k]);
				fprintf(f_parent,"%c ",tmp[k]);
			if(k!=(N-1))
				sprintf(tmp2+k,"%c",bos);
		}
		
		write(fd, tmp2, strlen(tmp2));

		if(size/N==1){
			fprintf(f_parent,"%c",new);
			printf("\n");
		}else{

			lseek(fd,(-N*t),SEEK_END);
			read(fd,tmp,N*2);	

		}
		size=size-N;
		t++;

	}

		t--;
		lseek(fd,(-N*t),SEEK_END);

		fl.l_type = F_UNLCK;
		fcntl(fd, F_SETLKW, &fl); 
	//	sleep(2);

		kill(child,SIGUSR2);
	}

	close(fd);

    return 0;
}
