
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <TFile.h>

#include "p2shunt.h"

const int MAXBUF=60000;
#define MYSIZE 1000


double rMomentum(char * filenameM){
  int recL,rec=0;
  unsigned short length;
  unsigned short buf[MAXBUF];
  FILE* data_fileM;
  int flopen, s, ss, n;
  char c;
  int TargetNum;
  int TargetFilter;
  int datareadM;

  ssize_t res;
  char *cbuf;
  float *m;
  float P1;

  char dfnameM[MYSIZE];

  
  bool FlagTarget=true;
  bool FlagMomentum=true;

  cbuf = (char*)buf;

    data_fileM=fopen(filenameM, "r");
    if (data_fileM>0) { 
      printf(" opened succefully\n");
    }
    else {
      printf(" can't open\n ");
      exit(1);
    }

  printf("\n");

  while(fgets(dfnameM,MYSIZE,data_fileM)!= NULL){                   //Opening file of list

    int ss = 0;
    int n = 0;




    for(s=0;s<MYSIZE;s++){
      c = dfnameM[s];
      if( c==' ') break;
    }
    if(c=='|') {s=s-1;}


    if(s<MYSIZE){
      dfnameM[s]=0;
      printf("File = %s",dfnameM);
      printf("\n");
      datareadM=open(dfnameM,O_RDONLY);

      if(datareadM>0){
        printf(" opened succesfully\n");

      }
      else{
        printf("can't open\n");
        continue; 
      }
    }
    printf("\n");

  while((res=read(datareadM,&length,sizeof(length)))>0){ 
    if(res == 0) {
      printf("EOF has been read at record# %d\n",rec);
      close(datareadM);
      return 0;
    } else if(res == -1) {
       printf("Read error at record# %d\n",rec);
    } else {
       rec++;
       if(MAXBUF<length){
    	   printf("Error: MAXBUF<length");
    	   close(datareadM);
    	   return 0;
       }
       res = read(datareadM,buf,(length-1)*2);
       if(res > 0) {
         if(buf[0]==0x5343){
           recL=0;
           while(recL<length-2){ 

             //-------------print MOMENTUM-------------------
             if(buf[2+recL]==0x4D53 && FlagMomentum==true){
               m=(float*)(buf+2+recL+1);
	       if(shunt2p(m,5)>0.9){
	         P1=shunt2p(m,1);                             //Momentum
		 FlagMomentum=false;
                 printf("Momentum: %f\n",P1);
               }
             }
             //----------------------------------------------

    	     recL+=buf[1+recL];
           }               
    	 }
      } else if (res == -1) {
         printf("Read error at record# %d\n",rec);
         close(datareadM);
      } else {
         printf("EOF has been read at record# %d\n",rec);
         close(datareadM);
      }
    }
  }
 }
 close(datareadM);
 return P1;

}


