#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

#define QUEUE_SIZE 100

pthread_t tid[3];
pthread_mutex_t lock1,lock2,lock3,lock;
pthread_cond_t cond1,cond2,cond3;
int secim,sayac,satir;
int done;

struct florist{

	char name[10];
	int x;
	int y;
	double time;
	char cicek[3][10];
	int queue[100];
	int f; 
    int r;
	int	count; 	
	int adet;
	double total;
	

};
struct client{

	char name[10];
	int x;
	int y;
	char cicek[10];
	double mesafe;


};

struct florist cicekci[3];
struct client *musteri;
void push(int item, int q[], int *r,int *c);
int pop(int q[], int *f, int *r,int *c);
int qempty(int f, int r);
int qfull(int r);
void init_florist(struct florist strc[3]);
void init_client(FILE *file,struct client *musteri,char ad[20]);
int mesafe(struct florist strc[3],struct client musteri,double *mesafe);


void* doSomeThing(void *arg)
{

	int item;
	double sure;
	double randomnumber;
	double sleep_time;
	while(done<satir){
	

				pthread_mutex_lock (&lock1);
				while(cicekci[0].count>0){
					while ( cicekci[0].count==0){

						pthread_cond_wait (&cond1, &lock1);
					}
					item=pop(cicekci[0].queue,&(cicekci[0].f),&(cicekci[0].r),&(cicekci[0].count));
					sure=musteri[item].mesafe/cicekci[0].time;
					randomnumber = ((rand() % 11)*4)+10;
			
					sleep_time=(randomnumber+sure);
					cicekci[0].total+=sleep_time;
					sleep(sleep_time/1000.0);
					printf("Florist %s has delivered a %s to client%d in %dms\n",cicekci[0].name,musteri[item].cicek,item+1,(int)sleep_time);
					cicekci[0].adet+=1;
			
					done+=1;

					}
					pthread_mutex_unlock (&lock1);

				pthread_mutex_lock (&lock2);
				while(cicekci[1].count>0){
					while ( cicekci[1].count==0){

						pthread_cond_wait (&cond2, &lock2);
					}
					item=pop(cicekci[1].queue,&(cicekci[1].f),&(cicekci[1].r),&(cicekci[1].count));
					sure=musteri[item].mesafe/cicekci[1].time;
					randomnumber = ((rand() % 11)*4)+10;
					sleep_time=(randomnumber+sure);
					cicekci[1].total+=sleep_time;
					sleep(sleep_time/1000.0);				

					printf("Florist %s has delivered a %s to client%d in %dms\n",cicekci[1].name,musteri[item].cicek,item+1,(int)sleep_time);
					cicekci[1].adet+=1;

					done+=1;

					}								
					pthread_mutex_unlock (&lock2);

				pthread_mutex_lock (&lock3);
				while(cicekci[2].count>0){
					while ( cicekci[2].count==0){

						pthread_cond_wait (&cond3, &lock3);
					}
					item=pop(cicekci[2].queue,&(cicekci[2].f),&(cicekci[2].r),&(cicekci[2].count));
					sure=musteri[item].mesafe/cicekci[2].time;
					randomnumber = ((rand() % 11)*4)+10;
					sleep_time=(randomnumber+sure);
					cicekci[2].total+=sleep_time;			
					sleep(sleep_time/1000.0);
				
					printf("Florist %s has delivered a %s to client%d in %dms\n",cicekci[2].name,musteri[item].cicek,item+1,(int)sleep_time);
					cicekci[2].adet+=1;

					done+=1;
					}				
					pthread_mutex_unlock (&lock3);
		}
		return NULL;
}



int main(int argc,char *argv[])
{
    int i = 0,j=0;
    int thr;
	char line[256];

	FILE *file=fopen(argv[1],"r");
	
	satir=0;
	srand(time(NULL));


if (argc !=2 ) { 
		printf("İnput Hatası..!\n");
		printf("Usage: ./floristApp data.dat\n");
		return -1;
	}  


if(file==NULL){ 
	printf("File open error...!!!\n");
	return -1;

}

printf("Florist application initializing from file: %s\n",argv[1]);

	init_florist(cicekci);
    while (fgets(line, sizeof(line), file)) {
		i++;
    }
	satir=i-4;
 	fclose(file);
	
	musteri=malloc(satir * sizeof(*musteri));
	init_client(file,musteri,argv[1]);


	i=0;


secim=0;
    while(i < 3)
    {
        thr = pthread_create(&(tid[i]), NULL, &doSomeThing, NULL);
        if (thr != 0)
            printf("\ncan't create thread :[%s]", strerror(thr));
        i++;
		secim++;
    }


    if (pthread_mutex_init(&lock1, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }


    if (pthread_mutex_init(&lock2, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }

    if (pthread_mutex_init(&lock3, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }

printf("3 florists have been created\nProcessing requests\n");
  	sayac=0;
	while(sayac<satir){
	
		secim=mesafe(cicekci,musteri[sayac],&(musteri[sayac].mesafe));

		if(secim==0){
			pthread_mutex_lock (&lock1);
			push(sayac, cicekci[0].queue, &(cicekci[0].r),&(cicekci[0].count));
			
			pthread_cond_signal(&cond1);
			pthread_mutex_unlock (&lock1);

		}
		else if(secim==1){
			pthread_mutex_lock (&lock2);
			push(sayac, cicekci[1].queue, &(cicekci[1].r),&(cicekci[1].count));
			
			pthread_cond_signal(&cond2);
			pthread_mutex_unlock (&lock2);
		}
		else{
			pthread_mutex_lock (&lock3);	
			push(sayac, cicekci[2].queue, &(cicekci[2].r),&(cicekci[2].count));
			
			pthread_cond_signal(&cond3);
			pthread_mutex_unlock (&lock3);
		}
	
		sayac++;
	}

	pthread_cond_signal(&cond1);
	pthread_cond_signal(&cond2);
	pthread_cond_signal(&cond3);

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
	pthread_join(tid[2], NULL);
    pthread_mutex_destroy(&lock1);
    pthread_mutex_destroy(&lock2);
    pthread_mutex_destroy(&lock3);

	printf("...\nAll requests processed.\nAyse closing shop.\nMurat closing shop.\nFatma closing shop.\nSale statistics for today:\n");
	printf("-------------------------------------------------\n");
	printf("Florist         # of sales          Total time\n");
	printf("-------------------------------------------------\n");
	for(int k=0; k<3; k++){
		printf("%s              %d                   %dms\n",cicekci[k].name,cicekci[k].adet,(int)cicekci[k].total);
	}
	printf("-------------------------------------------------\n");
	free(musteri);
    return 0;
}

int mesafe(struct florist strc[3],struct client musteri,double *mesafe){

	int sec=-1;
	double min=1000000,tmp;
	
	for(int j=0; j<3; j++){

		for(int i=0; i<3; i++){

			if(strcmp(strc[j].cicek[i],musteri.cicek)==0){
					
				tmp=sqrt(pow((strc[j].x-musteri.x),2)+pow((strc[j].y-musteri.y),2));
				if(tmp<min){
					min=tmp;
					sec=j;
					
				}
				break;	

			}

		}
	}


	(*mesafe)=min;
	return sec;

}


void init_florist(struct florist strc[3]){
/*
	char tmp[20];
	int i=0;
	FILE *file=fopen("data.dat","r");

	while(i<3){

		fscanf(file, "%s (%d,%d; %lf) : %s, %s, %s", strc[i].name,&strc[i].x,&strc[i].y,&strc[i].time,tmp,tmp,tmp);
		printf("%s (%d,%d; %lf\n)", strc[i].name,strc[i].x,strc[i].y,strc[i].time);
		i++;
	}
	fclose(file);
*/
	strcpy(strc[0].name,"Ayşe");
	strc[0].x=10;
	strc[0].y=25;
	strc[0].time=1.5;
	strc[0].f=0;
	strc[0].r=-1;
	strc[0].count=0;
	strc[0].total=0;
	strcpy(strc[0].cicek[0],"orchid");
	strcpy(strc[0].cicek[1],"rose");
	strcpy(strc[0].cicek[2],"violet");


	strcpy(strc[1].name,"Fatma");
	strc[1].x=-10;
	strc[1].y=-15;
	strc[1].time=1.3;
	strc[1].f=0;
	strc[1].r=-1;
	strc[1].count=0;
	strc[1].total=0;
	strcpy(strc[1].cicek[0],"clove");
	strcpy(strc[1].cicek[1],"rose");
	strcpy(strc[1].cicek[2],"daffodil");

	strcpy(strc[2].name,"Murat");
	strc[2].x=-10;
	strc[2].y=8;
	strc[2].time=1.1;
	strc[2].f=0;
	strc[2].r=-1;
	strc[2].count=0;
	strc[2].total=0;
	strcpy(strc[2].cicek[0],"violet");
	strcpy(strc[2].cicek[1],"daffodil");
	strcpy(strc[2].cicek[2],"orchid");

}

void init_client(FILE *file,struct client *musteri, char ad[20]){

	int i=0;
	int j=0;
	int flag=1;
	char line[256];

    if((file = fopen(ad, "r")) == NULL) {
        printf("File open error...!!!\n");
        exit(1);
    }

    while (flag) {
     
		if(j>3){
			int ret = fscanf(file, "%s (%d,%d): %s", musteri[i].name,&musteri[i].x,&musteri[i].y,musteri[i].cicek);	
			musteri[i].mesafe=0;
			if(ret == EOF) {
				flag=0;
			}


			i++;
		}else{

			fgets(line, sizeof(line), file);
 	
		}
		j++;
        
    }
	fclose(file);
	return;

}


void push(int item, int q[], int *r,int *c)
{
    if (qfull(*r)) 
    {
        printf("\t\t\tQueue overflow\n");
        return;
    }
    
    q[++(*r)] = item; 
	(*c)++;
}





int pop(int q[], int *f, int *r,int *c)
{

	int data; 
    if (qempty(*f, *r))
    {
        printf("\t\t\tQueue underflow\n");
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
