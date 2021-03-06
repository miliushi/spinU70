#include <TROOT.h>
#include <TApplication.h>
#include <TCanvas.h>
#include <TGCanvas.h>
#include <TStyle.h>
#include <TFormula.h>
#include <TBrowser.h>
#include <TGraph.h>
#include <TF1.h>
#include <TH1F.h>
#include <TBuffer.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <TFile.h>
#include <TMath.h>
#include <TAxis.h>
#include <TGraphErrors.h>
#include <iostream>
#include <sstream> 
#include <string> 

#define MYSIZE 1000
#define N 20
const int MAXBUF=60000;




double y(unsigned short b1,unsigned short b2);




double Terehof(char * filename)
{
  Int_t num = 0; 
  char c;
  FILE* data_fileT;
  int flopenT, s, ss, n;

  unsigned short lengthT;
  unsigned short bufT[MAXBUF];
  char dfnameT[MYSIZE];
  int recL,recT=0;


  ssize_t res;
 
  double y8[1000],y1[1000],y2[1000],y3[1000],
  y4[1000],y5[1000],y6[1000],y7[1000];
  double tot1, tot2, tot3, tot4, tot5, tot6, tot7, tot8;

  tot1=0;
  //printf("argc=%d EXE=%s\n",argc,argv[0]);          //Opening file with list of files
  
    printf("File= %s ",filename);
    data_fileT=fopen(filename, "r");
    if (data_fileT>0) { 
      printf(" opened succefully\n");
    }
    else {
      printf(" can't open\n ");
      exit(1);
    }
  

  printf("\n");

  int dataread;


//********************************************************************************************************** 

                                  
  while(fgets(dfnameT,MYSIZE,data_fileT)!= NULL){                   //Opening file of list

    int ss = 0;
    int n = 0;




    for(s=0;s<MYSIZE;s++){
      c = dfnameT[s];
      if( c==' ') break;
    }
    if(c=='|') {s=s-1;}


    if(s<MYSIZE){
      dfnameT[s]=0;
      printf("File = %s",dfnameT);
      printf("\n");
      dataread=open(dfnameT,O_RDONLY);

      if(dataread>0){
        printf(" opened succesfully\n");

      }
      else{
        printf("can't open\n");
        continue; 
      }
    }
    printf("\n");

    while((res=read(dataread,&lengthT,sizeof(lengthT)))>0){
      if(res == 0) {
        printf("EOF has been read at record# %d\n",recT);
        close(dataread);
        return 0;
      } else if(res == -1) {
         printf("Read error at record# %d\n",recT);
         //perror(dataread);
        } else {
         recT++;
         if(MAXBUF<lengthT){
    	     printf("Error: MAXBUF<lengthT");
    	     close(dataread);
    	     return 0;
         }
         res = read(dataread,bufT,(lengthT-1)*2);
         if(res > 0) {  
           if(bufT[0]==0x4553){
             recL=0;
             while(recL<lengthT-2){
               if(bufT[2+recL]==0x5653){
                 y1[n]=y(bufT[recL+3],bufT[recL+4]);
    	         y2[n]=y(bufT[recL+5],bufT[recL+6]);
    	         y3[n]=y(bufT[recL+7],bufT[recL+8]);
    	         y4[n]=y(bufT[recL+9],bufT[recL+10]);
    	         y5[n]=y(bufT[recL+11],bufT[recL+12]);
    	         y6[n]=y(bufT[recL+13],bufT[recL+14]);
                 y7[n]=y(bufT[recL+15],bufT[recL+16]);
    	         y8[n]=y(bufT[recL+17],bufT[recL+18]);
                 tot1+=y1[n];
                 tot2+=y2[n];
                 tot3+=y3[n];
                 tot4+=y4[n];
                 tot5+=y5[n];
                 tot6+=y6[n];
                 tot7+=y7[n];
                 tot8+=y8[n];
               }
               recL+=bufT[1+recL]; 
             }
           }

      } else if (res == -1) {
         printf("Read error at record# %d\n",recT);
         close(dataread);
      } else {
         printf("EOF has been read at record# %d\n",recT);
         close(dataread);
      }
    }
  } 
  close(dataread);
  printf("Terehov=%10.0f\n",tot1);

  

  }


return tot1;
}

double y(unsigned short b1,unsigned short b2){
	double u=	b1&0x000F+10*((b1&0x00F0)>>4)+
	   100*((b1&0x0F00)>>8)+1000*((b1&0xF000)>>12)+
	   10000*(b2&0x000F)+100000*((b2&0x00F0)>>4)+
	   1000000*((b2&0x0F00)>>8)+10000000*((b2&0xF000)>>12);
	return u;
}



