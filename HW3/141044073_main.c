#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include "tlpi_hdr.h"

#define BUFFER 100
void catcher(int signum){

	if(signum==SIGTERM){
		printf("SIGTERM SİNYAL YAKALANDI...!!!\n");
		exit(0);
	}
		
}

int main(void) {
    char buf[BUFFER];
	char split_buf[1024][1024];
	int i=0,j=0,k=0;
	int size;
	int pid;
	char mybuffer[BUFFER];
	char tmp_buffer[BUFFER];
	int index_pipe=0;
	int index_in=0;
	int index_out=0;
	int ret;
	int pip_adet;
	sigset_t sigset;
	struct sigaction sact;
	sigemptyset(&sact.sa_mask);
	sact.sa_flags=0;
	sact.sa_handler=catcher;
	sigfillset(&sigset);
	sigdelset(&sigset,SIGTERM);

	if(sigaction(SIGTERM,&sact,NULL)!=0){
		perror("HATA..!");
	}

getcwd(mybuffer,BUFFER);
strcpy(tmp_buffer,mybuffer);

    while(1) {
        printf("1411044073/Bedirhan_ÇAĞLAR:@ ");

        if(!scanf ("%[^\n]%*c", buf)){

			break;
		}

    size=strlen(buf);
	i=0;
	k=0;
	j=0;
	pip_adet=0;
	while(k<size){


		if(buf[k]==' '){

			split_buf[i][j]='\0';
			i++;
			k++;
			j=0;
		}
		else{
			if(buf[k]=='|')
				pip_adet++;
			split_buf[i][j]=buf[k];
			j++;
			k++;
		}

		
	}
	split_buf[i][j]='\0';


	size=i+1;
	index_pipe=0;
	index_in=0;
	index_out=0;
	for(int j=0; j<size; j++){

		if(strcmp(split_buf[j],"|")==0){
			index_pipe=j;
		
		}
		else if(strcmp(split_buf[j],"<")==0){
			index_in=j;

		}
		else if(strcmp(split_buf[j],">")==0){
			index_out=j;
		}
	}


	pid= fork();  			


    if(pid==0) {

		if((size<3) || (index_out>0) || (index_in>0)){

			if(strcmp(split_buf[0], "ls")==0){

				sprintf(mybuffer,"%s/%s",tmp_buffer,"ls");
			
				if(size==1){
					char *args[]={mybuffer,NULL};
					execv(args[0], args);
				}else{
					if(index_out==0 && index_in==0){
						char *args[]={mybuffer,split_buf[1],NULL};
						execv(args[0], args);
					}
					else{

				 		char *myfile = split_buf[index_out+1];

						FILE *ifp;
						ifp = fopen(myfile, "w");

						 dup2(fileno(ifp), STDOUT_FILENO);
						fclose(ifp);

						if(size==3){
		
							char *args[]={mybuffer,NULL};
							execv(args[0], args);	
		
						}else{
							char *args[]={mybuffer,split_buf[1],NULL};
							execv(args[0], args);

						}
					}
				}			
		   		
			    perror("exec");
			    exit(1);
			}
			else if(strcmp(split_buf[0], "wc")==0){
			
				sprintf(mybuffer,"%s/%s",tmp_buffer,"wc");

				if(size==1){

					char *args[]={mybuffer,NULL};
					execv(args[0], args);
				}else{
					if(index_out==0 && index_in==0){
						char *args[]={mybuffer,split_buf[1],NULL};
						execv(args[0], args);
					}
					else{

				 		char *myfile = split_buf[index_out+1];

						FILE *ifp;
						ifp = fopen(myfile, "w");

						dup2(fileno(ifp), STDOUT_FILENO);
						fclose(ifp);
					 

						if(size==3){
		
							char *args[]={mybuffer,NULL};
							execv(args[0], args);	
		
						}else{
							char *args[]={mybuffer,split_buf[1],NULL};
							execv(args[0], args);

						}
					}
				}			
		   		

			    perror("exec");
			    exit(1);

			}
			else if(strcmp(split_buf[0], "cat")==0){

				sprintf(mybuffer,"%s/%s",tmp_buffer,"cat");
				if(size==1){
					char *args[]={mybuffer,NULL};
					execv(args[0], args);
				}else{
					if(index_out==0 && index_in==0){
						char *args[]={mybuffer,split_buf[1],NULL};
						execv(args[0], args);
					}
					else{

				 		char *myfile = split_buf[index_out+1];

						FILE *ifp;
						ifp = fopen(myfile, "w");

						dup2(fileno(ifp), STDOUT_FILENO);
						fclose(ifp);

						if(size==3){
		
							char *args[]={mybuffer,NULL};
							execv(args[0], args);	
		
						}else{
							char *args[]={mybuffer,split_buf[1],NULL};
							execv(args[0], args);

						}
					}
				}			
		   		

			    perror("exec");
			    exit(1);
			}else{
						
						exit(0);

			}
 			
		}else{
	
		if(pip_adet=1){
	
			int pfd[2];                                  

			if (pipe(pfd) == -1)                           
				perror("pipe");

			switch (fork()) {
			case -1:
				perror("fork");

			case 0:
            
				if (close(pfd[0]) == -1)                  
				    perror("close 1");


				if (pfd[1] != STDOUT_FILENO) {             
				    if (dup2(pfd[1], STDOUT_FILENO) == -1)
				        perror("dup2 1");
				    if (close(pfd[1]) == -1)
				        perror("close 2");
				}

				if(strcmp(split_buf[0], "ls")==0){

					sprintf(mybuffer,"%s/%s",tmp_buffer,"ls");
			
					if(size==3){
						char *args[]={mybuffer,NULL};
						execv(args[0], args);
					}else{
						char *args[]={mybuffer,split_buf[1],NULL};
						execv(args[0], args);
					}			
			   		
					perror("exec");
					exit(1);
				}
				else if(strcmp(split_buf[0], "wc")==0){
					
					sprintf(mybuffer,"%s/%s",tmp_buffer,"wc");

					if(size==3){
						char *args[]={mybuffer,NULL};
						execv(args[0], args);
					}else{
						char *args[]={mybuffer,split_buf[1],NULL};
						execv(args[0], args);
					}
					perror("exec");
					exit(1);

				}
				else if(strcmp(split_buf[0], "cat")==0){

					sprintf(mybuffer,"%s/%s",tmp_buffer,"cat");
					if(size==3){
						char *args[]={mybuffer,NULL};
						execv(args[0], args);
					}else{
						char *args[]={mybuffer,split_buf[1],NULL};
						execv(args[0], args);
					}
					perror("exec");
					exit(1);
				}
				else if(strcmp(split_buf[0], "help")==0){

				fprintf( stdout, "-ls 		which will list file type, access rights, file size and file name of all files in the present working directory\n-pwd 		which will print the present working directory\n-cd 		which will change the present working directory to the location provided as argument\n-cat 		which will print on standard output the contents of the file provided to it as argument\n-wc 		which will print on standard output the number of lines in the file provided to it as argument\n-exit 		which will exit the shell\n-help 		which will print the list of supported commands\n");

					exit(1);
				}
				else if(strcmp(split_buf[0],"pwd")==0 ){

					char buffer[BUFFER];
					getcwd(buffer,BUFFER);
					fprintf( stdout, "%s\n",buffer);
					//printf("%s\n",buffer);	
					exit(1);
				}

				else{
					exit(1);

				}
			default:   
			
				 wait(NULL);         
				 if (close(pfd[1]) == -1)                    
				    perror("close 3");

				if (pfd[0] != STDIN_FILENO) { 
				    if (dup2(pfd[0], STDIN_FILENO) == -1)
				        perror("dup2 2");
				    if (close(pfd[0]) == -1)
				        perror("close 4");
				}

				if(strcmp(split_buf[index_pipe+1], "ls")==0){

					sprintf(mybuffer,"%s/%s",tmp_buffer,"ls");
			
					if(size<=4){
						char *args[]={mybuffer,NULL};
						execv(args[0], args);
					}else{
						char *args[]={mybuffer,split_buf[index_pipe+1],NULL};
						execv(args[0], args);
					}			
			   		
					perror("exec");
					exit(1);
				}
				else if(strcmp(split_buf[index_pipe+1], "wc")==0){
					
					sprintf(mybuffer,"%s/%s",tmp_buffer,"wc");

					if(size<=4){
						char *args[]={mybuffer,NULL};
						execv(args[0], args);
					}else{
						char *args[]={mybuffer,split_buf[index_pipe+2],NULL};
						execv(args[0], args);
					}
					perror("exec");
					exit(1);

				}
				else if(strcmp(split_buf[index_pipe+1], "cat")==0){

					sprintf(mybuffer,"%s/%s",tmp_buffer,"cat");
					if(size<=4 ){
						char *args[]={mybuffer,NULL};
						execv(args[0], args);
					}else{
				
						char *args[]={mybuffer,split_buf[index_pipe+2],NULL};
						execv(args[0], args);
					}
					perror("exec");
					exit(1);
				}


				else{
					exit(1);

				}
			}

		
		 }

	 }
  }

  else {  
			wait(NULL);
			
			if(strcmp(split_buf[0], "exit")==0){
		 			break;
			}
			if(strcmp(split_buf[0], "kill")==0){
				if(strcmp(split_buf[1], "-15")==0)
					kill(getpid(),SIGTERM);
			}
			if(size<3 || index_out>0 || index_in>0 ){  

					if(strcmp(split_buf[0],"cd")==0 ){

						int	ret_ = chdir (split_buf[1]);
				
						if(ret_!=0){
							perror("bash: cd: Desk: No such file or directory");

						}
								
					}
					else if(strcmp(split_buf[0], "help")==0){

					char temp[]="-ls 		which will list file type, access rights, file size and file name of all files in the present working directory\n-pwd 		which will print the present working directory\n-cd 		which will change the present working directory to the location provided as argument\n-cat 		which will print on standard output the contents of the file provided to it as argument\n-wc 		which will print on standard output the number of lines in the file provided to it as argument\n-exit 		which will exit the shell\n-help 		which will print the list of supported commands";
						if(index_out>0){

							int file=open(split_buf[index_out+1],O_RDWR|O_CREAT|O_TRUNC,0666);
							write(file,temp,strlen(temp));	
							close(file);
						}else{

							printf("%s\n",temp);
						}						
					
					}
					else if(strcmp(split_buf[0],"pwd")==0 ){

				 		char *myfile = split_buf[index_out+1];
	  					char buffer[BUFFER];

						getcwd(buffer,BUFFER);

						if(index_out>0){

							int file=open(split_buf[index_out+1],O_RDWR|O_CREAT|O_TRUNC,0666);
							write(file,buffer,strlen(buffer));	
							close(file);
						}else{

							printf("%s\n",buffer);
						}

					}

			}
	}
}

 return 0;

}
