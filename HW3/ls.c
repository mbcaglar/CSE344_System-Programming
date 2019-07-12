#include <stdio.h>
#include <string.h>
#include <sys/types.h> 
#include <stdlib.h>
#include <errno.h>
#include <dirent.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>



int main(int argc, char ** argv) {

	DIR *director;

    struct dirent *direntptr;
	struct stat fileStat;
	//char tempPath[PATH_MAX]; 

//getcwd(tempPath,PATH_MAX);

if(argc==2){

	int	ret_ = chdir (argv[1]);
				
	if(ret_!=0){
		perror("bash: cd: Desk: No such file or directory");

	}

}
    if((director = opendir("./")) != NULL)
    {
      while((direntptr = readdir(director)) != NULL)
      {
		if( (direntptr->d_type) == DT_DIR )
		{

			printf("d");

		}				
		else if(direntptr->d_type == DT_REG) 
		{
			printf("-");

		}

		if(stat(direntptr->d_name,&fileStat) < 0)
 			return -1;

		
		printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
		printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
		printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
		printf("	%ld",fileStat.st_size);
		printf("	%s\n",direntptr->d_name);
	
      }
		closedir(director);  
    }


if(argc==2){

//sprintf(tempPath,"%s/%s",tempPath,"..");
	int	ret_ = chdir ("..");
				
	if(ret_!=0){
		perror("bash: cd: Desk: No such file or directory");

	}


}
    return 0;
}
