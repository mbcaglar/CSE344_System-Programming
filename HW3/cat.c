#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char **argv) {
	
	int file;
	char buffer[1];

	if(argc==1){
  		file =STDIN_FILENO;

	}
	else{
   		const char *myfile = argv[1];
   		file = open(myfile,O_RDONLY);
	}	
 	if(file == -1){
      printf("File Error !!!\n");
	  return -1;
   }

while (read(file,buffer,1) ==1 ){


		printf("%c",buffer[0]);


}

  return 0;

}

