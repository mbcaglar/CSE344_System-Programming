#include<stdio.h>
#include<string.h>	
#include<sys/socket.h>
#include<arpa/inet.h>	
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


#define PI 3.1415
#define QUEUE_SIZE 2

pthread_t tid[100];
pthread_t client_th;
pthread_mutex_t lock,lock2;
pthread_cond_t cond,cond2;
int secim,sayac,satir,read_size;
int done;
int control=0;
int client_count=0;
char client_message[3][25];
char message[200];
int arr[100];
double sonuc;
bool flag_c=true;
bool flag_no=true;
char log_str[100];
char names[100][20];
char *bosluk="";
FILE *file_log;


struct provider{

	char name[20];
	int performance;
	int price;
	int duration;
	int queue[2];
	double time;
	int f; 
    int r;
	int	count; 	
	int adet;
	double total;
	char message[75];
	int on;
	

};

struct client{

	char name[10];
	char c[1];
	char derece[12];
	int sock_id;


};

struct provider prov[100];
struct client musteri[200];





double find_cos(double degree);
void fill_message(double randomnumber,int count,int k);
void choose(struct provider *prov, char c[1],int *arr,int size);
void swap(int *xp, int *yp);
bool push(int item, int q[], int *r,int *c);
int pop(int q[], int *f, int *r,int *c);
int qempty(int f, int r);
int qfull(int r);
void init_provider(struct provider *prov,char ad[20]);
void statics();




void catcher(int signum){

	if(signum==SIGINT){
		char *mesaj="SERVER SHUTDOWN Good Bye...!!! ";

		fprintf(stderr,"\nTermination signal received\n");
		sprintf(log_str,"\nTermination signal received\n");
		fputs(log_str,file_log);
		strcpy(log_str,bosluk);
	
		for(int i=0; i<client_count; i++){

			if(musteri[i].sock_id!=-1){

			write(musteri[i].sock_id ,mesaj , strlen(mesaj));
			
			}
			close(musteri[i].sock_id);
		}
		fprintf(stderr,"Terminating all clients\n");
		sprintf(log_str,"Terminating all clients\n");
		fputs(log_str,file_log);
		strcpy(log_str,bosluk);

		fprintf(stderr,"Terminating all providers\n");
		sprintf(log_str,"Terminating all providers\n");

		fputs(log_str,file_log);
		strcpy(log_str,bosluk);

		statics();

		for(int i=0; i<satir; i++){
			 pthread_cancel(tid[i]);
		}

		fclose(file_log);

		flag_c=false;
    	pthread_mutex_destroy(&lock);
    	pthread_mutex_destroy(&lock2);

	//	exit(0);
	
	}
		
}



void* do_client(void *i){

		char *mesaj2="NO PROVIDER IS AVAILABLE ";

		pthread_mutex_lock (&lock2);		
		choose(prov,musteri[client_count].c,arr,satir);
			
		for(int i=0; i<satir; i++){

		//	pthread_mutex_lock (&lock);
			if(prov[arr[i]].total<=prov[arr[i]].duration){

			if(push(client_count, prov[arr[i]].queue, &(prov[arr[i]].r),&(prov[arr[i]].count))){
				
				flag_no=false;
				control=1;

				fprintf(stderr,"Client %s (%c %s) connected, forwarded to provider %s\n",musteri[client_count].name,musteri[client_count].c[0],musteri[client_count].derece,names[arr[i]]);

		sprintf(log_str,"Client %s (%c %s) connected, forwarded to provider %s\n",musteri[client_count].name,musteri[client_count].c[0],musteri[client_count].derece,names[arr[i]]);
		fputs(log_str,file_log);
		strcpy(log_str,bosluk);

				pthread_cond_signal(&cond2);			
				pthread_mutex_unlock (&lock2);
				pthread_mutex_lock (&lock);	
				pthread_cond_signal(&cond);
				pthread_mutex_unlock (&lock);
								
				break;
			}else{
				flag_no=true;
			}

		}else{

					if(prov[arr[i]].on!=1){
						fprintf(stderr,"***Provider %s duration süresini doldurmuştur.Yeni clientlara  kapalıdır.***\n",names[arr[i]]);
sprintf(log_str,"***Provider %s duration süresini doldurmuştur.Yeni clientlara  kapalıdır.***\n",names[arr[i]]);
		fputs(log_str,file_log);
		strcpy(log_str,bosluk);
						prov[arr[i]].on=1;
					}
				}

		}

		if(flag_no){

			//printf("NO PROVİDER\n");
			write(musteri[client_count].sock_id ,mesaj2 , strlen(mesaj2));	
			flag_no=true;
			control=1;
			pthread_cond_signal(&cond2);
			pthread_mutex_unlock (&lock2);
			pthread_mutex_lock (&lock);	
			pthread_cond_signal(&cond);
			pthread_mutex_unlock (&lock);
			
		}
		


		return NULL;

}


void* do_provide(void *i)
{

	int k = *((int *) i);


	int item;
	double sure;
	double randomnumber;
	double sleep_time;
	double derece;
	char str[12];
	while(flag_c){
	

				pthread_mutex_lock (&lock);
				while(prov[k].count>0){
					while ( prov[k].count==0){

						pthread_cond_wait (&cond, &lock);
					}

						
    				randomnumber = (double)rand()/RAND_MAX*2.0;//float in range -1 to 1
					randomnumber = randomnumber*4+5;

					
					sleep(randomnumber);
					item=pop(prov[k].queue,&(prov[k].f),&(prov[k].r),&(prov[k].count));
					


					fprintf(stderr,"Provider %s is processing task number %d: %s\n",names[k],prov[k].adet+1,musteri[item].derece);
		sprintf(log_str,"Provider %s is processing task number %d: %s\n",names[k],prov[k].adet+1,musteri[item].derece);
		fputs(log_str,file_log);
		strcpy(log_str,bosluk);
					sscanf(musteri[item].derece,"%lf",&derece);
					sonuc=find_cos(derece);
					
					fprintf(stderr,"Provider %s completed task number %d: cos(%s)=%.3lf in %.3lf seconds.\n",names[k],(prov[k].adet+1),musteri[item].derece,sonuc,randomnumber);

		sprintf(log_str,"Provider %s completed task number %d: cos(%s)=%.3lf in %.3lf seconds.\n",names[k],(prov[k].adet+1),musteri[item].derece,sonuc,randomnumber);
		fputs(log_str,file_log);
		strcpy(log_str,bosluk);

					prov[k].total = prov[k].total+ randomnumber;
					prov[k].adet+=1;			
				//	pthread_mutex_lock (&lock2);
					fill_message(randomnumber,item,k);
					write(musteri[item].sock_id ,prov[k].message , strlen(prov[k].message));
					musteri[item].sock_id=-1;
					//close(musteri[item].sock_id);
					//control=1;
					//pthread_cond_signal(&cond2);
					//pthread_mutex_lock (&lock2);
								
								
				}
				
	
					pthread_mutex_unlock (&lock);
			
				
		}
		return NULL;
}




int main(int argc , char *argv[])
{
    int i = 0,j=0;
    int thr,cln;
	char line[256];
	int parametre[100];
	int client_sock;
	int port_deger;
	int  c;
	int socket_desc;
	struct sockaddr_in server , client;
	FILE *file;
	
	sigset_t sigset;
	struct sigaction sact;
	sigemptyset(&sact.sa_mask);
	sact.sa_flags=0;
	sact.sa_handler=catcher;
	sigfillset(&sigset);  
	
	satir=0;
	srand(time(NULL));


	if (argc !=4 ) { 
		fprintf(stderr,"İnput Hatası..!\n");
		fprintf(stderr,"Usage: ./homeworkserver 5555 data.dat log.dat\n");
		return -1;
	}  
	if((file=fopen(argv[2],"r"))==NULL){ 
		fprintf(stderr,"File open error...!!!\n");
		return -1;
	}
	if((file_log=fopen(argv[3],"w+"))==NULL){ 
		fprintf(stderr,"File open error...!!!\n");
		return -1;
	}

	fprintf(stderr,"Logs kept at %s\n",argv[3]);
	sprintf(log_str,"Logs kept at %s\n",argv[3]);
	fputs(log_str,file_log);
	strcpy(log_str,bosluk);

  	while (fgets(line, sizeof(line), file)) {
		parametre[i]=i;
		i++;
    }
	satir=i-1;
 	fclose(file);
 	

	int boyut=satir*2;

	//prov=malloc(satir * sizeof(*prov));
	//musteri=malloc(boyut*sizeof(*musteri));
	//arr=malloc(satir * sizeof(*arr));
	//tid=malloc(satir * sizeof(*tid));
	 
	init_provider(prov,argv[2]);
	fprintf(stderr,"%d provider threads created\n",satir);
	sprintf(log_str,"%d provider threads created\n",satir);
	fputs(log_str,file_log);

	strcpy(log_str,bosluk);

	fprintf(stderr,"Name        Performance Price Duration\n");
	sprintf(log_str,"Name        Performance Price Duration\n");
	fputs(log_str,file_log);
	strcpy(log_str,bosluk);



	for(int i=0; i<satir; i++){
		fprintf(stderr,"%s        %d           %d      %d\n",names[i],prov[i].performance,prov[i].price,prov[i].duration);
		sprintf(log_str,"%s        %d           %d      %d\n",names[i],prov[i].performance,prov[i].price,prov[i].duration);
		fputs(log_str,file_log);
		strcpy(log_str,bosluk);

	}




	i=0;
	
	sigdelset(&sigset,SIGINT);

    while(i < satir)
    {
 
        thr = pthread_create(&(tid[i]), NULL, &do_provide,&parametre[i]);
        if (thr != 0)
            fprintf(stderr,"\ncan't create thread :[%s]", strerror(thr));
		fprintf(stderr,"Provider %s waiting for tasks\n",names[i]);
		sprintf(log_str,"Provider %s waiting for tasks\n",names[i]);
		fputs(log_str,file_log);
		strcpy(log_str,bosluk);


        i++;
		
    }
	if (pthread_mutex_init(&lock, NULL) != 0)
    {
        fprintf(stderr,"\n mutex init failed\n");
        return -1;
    }
	if (pthread_mutex_init(&lock2, NULL) != 0)
    {
        fprintf(stderr,"\n mutex init failed\n");
        return -1;
    }

	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		fprintf(stderr,"Could not create socket\n");
	}



	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;


	sscanf(argv[1],"%d",&port_deger);
	server.sin_port = htons(port_deger);

	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{

		perror("bind failed. Error\n");
		return 1;
	}

	listen(socket_desc , 3);
	
	fprintf(stderr,"Server is waiting for client connections at port %d\n",port_deger);
	sprintf(log_str,"Server is waiting for client connections at port %d\n",port_deger);
	fputs(log_str,file_log);
	strcpy(log_str,bosluk);


	if(sigaction(SIGINT,&sact,NULL)!=0){
				perror("HATA..!");
			}
	

	for(; flag_c==true;){

		if(flag_c==false){
			break;
		}

		c = sizeof(struct sockaddr_in);

		client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);

		if(flag_c==false){
			break;
		}

		if (client_sock < 0)
		{
			perror("accept failed\n");
			return 1;
		} 
		read_size = read(client_sock, &musteri[client_count] ,sizeof(struct client));
	
		if(read_size == 0)
		{
			fprintf(stderr,"Client disconnected\n");
			fflush(stdout);
		}
		else if(read_size == -1)
		{
			perror("recv failed\n");
		}
		musteri[client_count].sock_id=client_sock;



		cln = pthread_create(&client_th, NULL, &do_client, NULL);

        if (cln != 0)
            fprintf(stderr,"\ncan't create thread :[%s]", strerror(cln));


		pthread_mutex_lock (&lock2);
		while(control==0){
			pthread_cond_wait (&cond2, &lock2);
		}	
			
		client_count++;
		control=0;
		pthread_mutex_unlock (&lock2);


	}	

	close(socket_desc);

	return 0;
}


void init_provider(struct provider *prov, char ad[20]){

	FILE *file;
	int i=0;
	int j=0;
	int flag=1;
	char line[256];
	char tmp[20];
	
	

    if((file = fopen(ad, "r")) == NULL) {
        fprintf(stderr,"File open error...!!!\n");
        exit(1);
    }

    while (flag) {
     
		if(j>0){
			memset(tmp,0,sizeof(tmp));
			
			int ret = fscanf(file, "%s %d %d %d",tmp,&prov[i].performance,&prov[i].price,&prov[i].duration);	


			if(ret == EOF) {
				flag=0;
				break;
			}
			memset(prov[i].name,0,sizeof(prov[i].name));
			strcpy(prov[i].name,tmp);
			strcpy(names[i],tmp);
			//printf("names---------> %s\n",prov[i].name);
			prov[i].time=0;
			prov[i].adet=0;
			prov[i].f=0;
			prov[i].r=-1;
			prov[i].count=0;
			prov[i].total=0;
			prov[i].on=1;
			
//			strcpy(prov[i].message,tmp);

			i++;
		}else{

			fgets(line, sizeof(line), file);
 	
		}
		j++;
        
    }
	fclose(file);
	return;

}

/*
void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}
 */

void choose(struct provider *prov, char c[1],int *arr,int size){


   int i, j,tmp;
	int sira[size];

	for(int t=0; t<size; t++){

		sira[t]=t;
		if(c[0]=='T'){
		
			arr[t]=prov[t].count;
		}
		
		else if(c[0]=='Q'){
		
			arr[t]=prov[t].performance;
		}
		else if(c[0]=='C'){
		
			arr[t]=prov[t].price;
		}

	}


for (i = 0; i < size-1; i++){      
 
       for (j = 0; j < size-i-1; j++){ 


			if(c[0]=='T' || c[0]=='C' ){

		       if (arr[j] > arr[j+1]){
				  tmp=arr[j+1];
				  arr[j+1]=arr[j];
				  arr[j]=tmp;

				  tmp=sira[j+1];
				  sira[j+1]=sira[j];
				  sira[j]=tmp;
		          //swap(&arr[j], &arr[j+1]);
				  //swap(&sira[j], &sira[j+1]);
				}
			}
			else{
		       if (arr[j] < arr[j+1]){

				  tmp=arr[j+1];
				  arr[j+1]=arr[j];
				  arr[j]=tmp;

				  tmp=sira[j+1];
				  sira[j+1]=sira[j];
				  sira[j]=tmp;
//		          swap(&arr[j], &arr[j+1]);
	//			  swap(&sira[j], &sira[j+1]);
				}


			}	
		}

}


	for(int i=0; i<satir; i++){
		arr[i]=sira[i];
	//	printf("%s %d %d %d\n",prov[i].name,prov[i].performance,prov[i].price,prov[i].duration);
	}


}


double find_cos(double degree){

	
	double cevap=1;
	double tmp=1;
	int n=10;
	double radyan=degree*PI/180;

	for(int i=1; i<n; i+=2){
		tmp=tmp*(-1)*(radyan*radyan)/(i*(i+1));
		cevap=cevap+tmp;

	}
	
	return cevap;

}

void fill_message(double randomnumber,int count,int k){


	sprintf(prov[k].message,"Hileci’s task completed by %s in %.3lf seconds, cos(%s)=%.3lf, cost is %dTL,",names[k],randomnumber,musteri[count].derece,sonuc,prov[k].price);
	
}

bool push(int item, int q[], int *r,int *c)
{
    if (qfull(*r)) 
    {
        
        return false;
    }
    
    q[++(*r)] = item; 
	(*c)++;
	return true;
}

void statics(){

	fprintf(stderr,"Statistics\nName        Number of clients served\n");
	sprintf(log_str,"Statistics\nName        Number of clients served\n");
	fputs(log_str,file_log);
	strcpy(log_str,bosluk);
	for(int i=0; i<satir; i++){

		fprintf(stderr,"%s           %d\n",names[i],prov[i].adet);
		sprintf(log_str,"%s          %d\n",names[i],prov[i].adet);
		fputs(log_str,file_log);
		strcpy(log_str,bosluk);

	}	
		fprintf(stderr,"Goodbye.\n");
		sprintf(log_str,"Goodbye.\n");
		fputs(log_str,file_log);
		strcpy(log_str,bosluk);

	return;
}



int pop(int q[], int *f, int *r,int *c)
{

	int data; 
    if (qempty(*f, *r))
    {
       // printf("\t\t\tQueue bos silemezsin\n");
        return -1;
    }
    
	data = q[(*f)++];
	
    if(*f> *r)
    {
        *f=0,*r=-1;
    }
	(*c)--;
	return data;
}

int qempty(int f, int r)
{
    return (f>r)?1:0;
    
}
int qfull(int r)
{ 
    return (r==QUEUE_SIZE-1)?1:0;
}




