#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "tlpi_hdr.h"

#define BUFFER 1024
int main(int argc,char *argv[])
{

   int line = 0;
   int words = 0;
   int bytes = 0;
	int flag=0;

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

      	bytes++;
		
		
		    if (buffer[0]=='\n'){
		        line++;
		       	flag=0;            
		    }
		    else if ( buffer[0]== ' ' || buffer[0] == '\t' ){
				flag=0;
		    }
		    else{
		        
				if ( flag == 0 ){
		           words++;
		        }
		        flag=1;
		     }

	  

     } 

if(argc==1){

       printf("%d %d %d\n",line,words,bytes);
}else{

       printf("%d %d %d %s\n",line,words,bytes,argv[1]);
}       
         

return 0;
}
