
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


int rTarget(char * filenameM, int rTargetFlag){
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

             //--------------print TARGET---------------------------------------
             if(buf[2+recL]==0x5753 && FlagTarget==true){             
               for(int i=0;i<buf[1+recL];i++){  
                  printf("%c",cbuf[6+recL*2+i]);
               }
               printf("\n");
                 
                 TargetNum=(cbuf[6+recL*2+12])-'0';
                 printf("TargetNum=%d\n",TargetNum);
                 TargetFilter=(cbuf[6+recL*2+10])-'0';
                 printf("TargetFilter=%d\n",TargetFilter);
               FlagTarget=false;         
    	     }
             //------------------------------------------------------------------

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
 if(FlagTarget==true) {
   int prN = 30;
   char *print0;
   char *print1;
   char *print2;
   char *print3;
   char *print4;
   char *print5;
   char *print6;


   print0=strstr(dfnameM,"Al");            
   print1=strstr(dfnameM,"Cu");            
   print2=strstr(dfnameM,"W");            
   print3=strstr(dfnameM,"no_tgt");       
   print4=strstr(dfnameM,"PE");            
   print5=strstr(dfnameM,"C");            
   print6=strstr(dfnameM,"background");    



   if(print6!=NULL) {TargetFilter=1;}
   else {TargetFilter=0;}

   if(print0!=NULL)                            {TargetNum=0;}
   else if(print1!=NULL)                       {TargetNum=1;}
        else if(print2!=NULL)                  {TargetNum=2;}
             else if(print3!=NULL)             {TargetNum=3;}
                  else if(print4!=NULL)        {TargetNum=4;}
                       else if(print5!=NULL)   {TargetNum=5;}
                            else { printf("ERROR: name file\n"); return 10;}
 }

 if(rTargetFlag==0) {return TargetFilter;}
 else if(rTargetFlag==1) {return TargetNum;}
      else {printf("Error in rTargetFlag\n"); return 10;}

}




