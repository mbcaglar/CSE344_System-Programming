/***********************************************************/
/*                M.BEDİRHAN ÇAĞLAR                        */
/*                                                         */
/*                  20.03.2018                             */
/*                                                         */
/* Bu program verilen tif file çözümleyerek taglerden width*/
/* height elde edip içerisindek siyah beyaz resmin datasını*/
/* 0 ve 1 ler ile ekrana print eder.                       */
/*                                                         */
/*                                                         */
/***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <inttypes.h>
#include <unistd.h>
#include <fcntl.h>
#define INTEL 18761
#define MOTOROLA 19789
void data_print(unsigned char *mybuffer,unsigned int *temp_buffer,unsigned int *i_buffer,int width,int height,int bit_sample,int begin);

int main(int argc, char *argv[])
{

unsigned int off_set;
unsigned char *mybuffer;
unsigned int *temp_buffer,*i_buffer;
char* byte_order="bos";
int fileSize=0,fd=0;
int j=0,k=0,control=0;
int width=0,height=0,bit_sample=0,compressed=-1,begin_data=0;

if (argc !=2 ) { 
		printf("İnput Hatası..!\n");
		printf("Usage: ./tiffProcessor filename.tif \n");
		return -1;
	}  
FILE* file = fopen(argv[1],"r");

//dosyanın açılıp açılmama durumunun controlu.
if(file==NULL){ 
	printf("File open error...!!!\n");
	return -1;

}

//file_size hesabı(stackoverflow.com reference)
if(file) {

  fseek(file, 0 , SEEK_END);
  fileSize = ftell(file);
  fseek(file, 0 , SEEK_SET);
 
  fclose(file);
}
mybuffer=(unsigned char *) malloc(fileSize*2); 
temp_buffer=(unsigned int *) malloc(fileSize*2);
i_buffer=(unsigned int *) malloc(fileSize*2);
fd = open(argv[1],O_RDONLY);

// dosyayı unsigned olarak char arraye okuma.
control=read(fd,mybuffer,fileSize); 

if(control!=-1){
//byte ları 2'şer byte olarak birleştirerek arraye toplama.
	for(int i=0; i<fileSize-2; i=i+2){

		temp_buffer[j]=(mybuffer[i] << 8) + (mybuffer[i+1]);	
		j++;
	}
	for(int i=0; i<fileSize-2; i=i+2){

		i_buffer[k]=(mybuffer[i+1] << 8) + (mybuffer[i]); 	
		k++;
	}
	//byte ordera göre taglerin bulunması		
	if(temp_buffer[0] == MOTOROLA){

		byte_order="Byte order: Motorola";
		off_set= ( (mybuffer[4]<<24) + (mybuffer[5]<<16) + (mybuffer[6]<<8) + (mybuffer[7]) ) /2;
		for(int i=off_set+1; i<j-1; i=i+6){
			if(temp_buffer[i]==256){
				width=temp_buffer[i+4];
			}
			else if(temp_buffer[i]==257){
				height=temp_buffer[i+4];
			}
			else if(temp_buffer[i]==258){
				bit_sample=temp_buffer[i+4];
			}
			else if(temp_buffer[i]==259){
				compressed=temp_buffer[i+4];
			}
			else if(temp_buffer[i]==273){
				begin_data= temp_buffer[i+5];
			}
			if(width>0 && height>0 && bit_sample>0 && compressed>=0 && begin_data>0){
					i=j;
			}
		}
	}
	else if(temp_buffer[0] == INTEL){
		byte_order="Byte order: Intel";
		off_set= ( mybuffer[4] + (mybuffer[5]<<8) + (mybuffer[6]<<16) + (mybuffer[7]<<24) )/2;
		for(int i=off_set+1; i<k-1; i=i+6){
			if(i_buffer[i]==256){
				width=i_buffer[i+4];
			}
			else if(i_buffer[i]==257){
				height=i_buffer[i+4];
			}
			else if(i_buffer[i]==258){
				bit_sample=i_buffer[i+4];
			}
			else if(i_buffer[i]==259){
				compressed=i_buffer[i+4];
			}

			else if(i_buffer[i]==273){
				begin_data= i_buffer[i+4];
			}
		}
	}

//0103 tagini control ederek uncompressed olup olmadığının controlu.
	if(compressed==0){

		printf("Compressed tiff (!) (it must be uncompressed)\n");
		return -1;
	}
	else{
		printf("Width : %d pixels\nHeight : %d pixels\n%s\n",width,height,byte_order);	
		data_print(mybuffer,temp_buffer,i_buffer,width,height,bit_sample,begin_data); //tiff filenın resim datasının çözümlenmesini yapan function
	}
}
else{
	printf("File read hatasi..!!!\n");
	return -1;
}
	free(mybuffer);
	free(i_buffer);
	free(temp_buffer);
	close(fd);
	return 0;
}
//tiff filenın resim datasının çözümlenmesini yapan ve ekrana basan void function

void data_print(unsigned char *mybuffer,unsigned int *temp_buffer,unsigned int *i_buffer,int width,int height,int bit_sample,int begin){

	int k=1,j=0,size=0,alt_satir=0;
	unsigned char byte = 32;
	unsigned char mask = 1;
	unsigned char bits[8]="00000000"; // Extract the bits

	if(temp_buffer[0] == INTEL){
		size=i_buffer[2];
	}
	else if(temp_buffer[0] == MOTOROLA){
		size=temp_buffer[3];
	}
	if(bit_sample==1){
		for(int i=begin; i<size; i++){
			alt_satir=width;
			byte=mybuffer[i];
			for (int t = 7; t >=0; t--){
	  			bits[t] =(byte & (mask << t))!=0; //stackoverflow
				printf("%x",bits[t]);
				j++;
				if(j==alt_satir){
					printf("\n");
					j=0;
					t=-1;		
				}		
			}		
		}
	}
	else{
		for(int i=begin; i<size; i++){
			alt_satir=k*(width);
			if(mybuffer[i]==255){
				printf("1");
				j++;
			}
			if(mybuffer[i]==0){
				printf("0");
				j++;
			}
			if(j== alt_satir ){
				printf("\n");
				k++;		
			}
		}
	}	
	return;
}
