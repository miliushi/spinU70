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
#include <string>
#include <cstring>

using namespace std;

#define MYSIZE 1000
#define N 20

const int Np   = 20;
const int Ntar = 7;

double LenTOF  = 12.8;

Double_t A1[N], A2[N], A3[N], A4[N], A5[N];
Double_t S1[N], S2[N], S3[N], S4[N], S5[N];
Double_t t1[N], t2[N], t3[N], t4[N], t5[N];
Int_t num = 0; 
Int_t numP = 0;
Int_t particle = 0;
//const Int_t numP=12;
char c, ch1, par1[N];
char dfname[MYSIZE];
char dfnameF[MYSIZE];
char terFName[MYSIZE];
char momY[3];



char rootname15[10] = "26015;1";
char rootname[10] = "26015;1";
char rootname16[10] = "26016;1";
char rootname17[10] = "26017;1";
char rootresult15[20]="test15.root";
char rootresult16[20]="test16.root";
char rootresult17[20]="test17.root";
char rootBackground[20]="Background.root";
char rootDATA[20]="DATA.root";

//Double_t pk[numP];
Double_t Ipi[Np][7],Id[Np][7],Ik[Np][7], Ip[Np][7], Iall[Np][7];
Int_t part;

const Double_t  L = 12.8/1.08;
const Double_t Vc  = 3e8;

const Double_t mPi = 0.13957;    //massa in GeV
const Double_t mP  = 0.938272;
const Double_t mK  = 0.493677;
const Double_t mD  = 1.875613;
const Double_t mT  = 2.809303;

const Double_t minPd = 1.3;



//Double_t Api_All[N],Ak_All[N],Ap_All[N], SigmaPi_All[N],SigmaK_All[N],SigmaP_All[N],tpi_All[N],tk_All[N],tp_All[N];

Double_t Api_C[N],  Ak_C[N],  Ap_C[N],  SigmaPi_C[N],  SigmaK_C[N],  SigmaP_C[N],  SigmaD_C[N],  tpi_C[N],  tk_C[N],  tp_C[N];
Double_t Api_Cu[N], Ak_Cu[N], Ap_Cu[N], SigmaPi_Cu[N], SigmaK_Cu[N], SigmaP_Cu[N], SigmaD_Cu[N], tpi_Cu[N], tk_Cu[N], tp_Cu[N];
Double_t Api_W[N],  Ak_W[N],  Ap_W[N],  SigmaPi_W[N],  SigmaK_W[N],  SigmaP_W[N],  SigmaD_W[N],  tpi_W[N],  tk_W[N],  tp_W[N];
Double_t Api_Al[N], Ak_Al[N], Ap_Al[N], SigmaPi_Al[N], SigmaK_Al[N], SigmaP_Al[N], SigmaD_Al[N], tpi_Al[N], tk_Al[N], tp_Al[N];
Double_t Api_PE[N], Ak_PE[N], Ap_PE[N], SigmaPi_PE[N], SigmaK_PE[N], SigmaP_PE[N], SigmaD_PE[N],  tpi_PE[N], tk_PE[N], tp_PE[N];


Double_t tpi[Np][Ntar], tk[Np][Ntar], tp[Np][Ntar], td[Np][Ntar], tt[Np][Ntar];
Double_t Api[Np][Ntar], Ak[Np][Ntar], Ap[Np][Ntar], Ad[Np][Ntar], At[Np][Ntar];
Double_t Spi[Np][Ntar], Sk[Np][Ntar], Sp[Np][Ntar], Sd[Np][Ntar], St[Np][Ntar];
Double_t p[Np][Ntar];
Double_t pALL[200];
Int_t    momN[Np][Ntar];

//*************************************FitFunction 1-Gauss**************************************
double FitFunction1( double *x , double *par){
  return par[0]/(par[1]*sqrt(2*3.14159265))*exp(-(x[0]-par[2])*(x[0]-par[2])/(2*par[1]*par[1]));
}
//**********************************************************************************************
//*************************************FitFunction 2-Gauss (time_K - FIX(time_pi))********************************
double FitPiK( double *x , double *par){
  return par[0]/(par[1]*sqrt(2*3.14159265))*exp(-(x[0]-par[2])*(x[0]-par[2])/(2*par[1]*par[1]))+
         par[3]/(par[4]*sqrt(2*3.14159265))*exp(-(x[0]-(par[2]+L*1e9*(sqrt(par[5]*par[5]+mK*mK)-sqrt(par[5]*par[5]
          +mPi*mPi))/(par[5]*Vc)))*(x[0]-(par[2]+L*1e9*(sqrt(par[5]*par[5]+mK*mK)-sqrt(par[5]*par[5]
          +mPi*mPi))/(par[5]*Vc)))/(2*par[4]*par[4]));
}
//**********************************************************************************************

//*************************************FitFunction 3-Gauss**************************************
double FitFunction( double *x , double *par){
  return par[0]/(par[1]*sqrt(2*3.14159265))*exp(-(x[0]-par[2])*(x[0]-par[2])/(par[1]*par[1]))+
         par[3]/(par[4]*sqrt(2*3.14159265))*exp(-(x[0]-par[5])*(x[0]-par[5])/(par[4]*par[4]))+
         par[6]/(par[7]*sqrt(2*3.14159265))*exp(-(x[0]-par[8])*(x[0]-par[8])/(par[7]*par[7])) ;
}
//**********************************************************************************************
//*************************************FitFunction 3-Gauss (time_K - FIX(time_pi))********************************
double FitFunction2( double *x , double *par){
  return par[0]/(par[1]*sqrt(2*3.14159265))*exp(-(x[0]-par[2])*(x[0]-par[2])/(2*par[1]*par[1]))+
         par[3]/(par[4]*sqrt(2*3.14159265))*exp(-(x[0]-(par[2]+L*1e9*(sqrt(par[5]*par[5]+mK*mK)-sqrt(par[5]*par[5]
          +mPi*mPi))/(par[5]*Vc)))*(x[0]-(par[2]+L*1e9*(sqrt(par[5]*par[5]+mK*mK)-sqrt(par[5]*par[5]
          +mPi*mPi))/(par[5]*Vc)))/(2*par[4]*par[4]))+
         par[6]/(par[7]*sqrt(2*3.14159265))*exp(-(x[0]-par[8])*(x[0]-par[8])/(2*par[7]*par[7]));
}
//**********************************************************************************************
//*************************************Sigma**************************************
double polinom6( double *x , double *par){
  return par[0]+par[1]*x[0]+par[2]*x[0]*x[0]+par[3]*x[0]*x[0]*x[0]+par[4]*x[0]*x[0]*x[0]*x[0]+
         par[5]*x[0]*x[0]*x[0]*x[0]*x[0]+par[6]*x[0]*x[0]*x[0]*x[0]*x[0]*x[0];
}
//**********************************************************************************************
//*************************************Sigma4**************************************
double polinom4( double *x , double *par){
  return par[0]+par[1]*x[0]+par[2]*x[0]*x[0]+par[3]*x[0]*x[0]*x[0]+par[4]*x[0]*x[0]*x[0]*x[0];
}
//**********************************************************************************************




#define INTEGRALF(II,A,SIGMA,t,VALUEINT,NUMP,PARTICLE){\
    Double_t Xmax, Xmin;\
    II->FixParameter(0,A);\
    II->FixParameter(1,SIGMA);\
    II->FixParameter(2,t);\
    Double_t dx=0.001;\
    Double_t X = t;\
    Xmin= X;\
    Double_t IN=II->Eval(Xmin);\
    while(IN>0.5) {\
      Xmin=Xmin-dx;\
      IN=II->Eval(Xmin);\
    }\
    X = t;\
    Xmax = X;\
    IN=II->Eval(Xmax);\
    while(IN>0.5) {\
      Xmax=Xmax+dx;\
      IN=II->Eval(Xmax);\
    }\
    IN = II->Integral(Xmin,Xmax)/0.05;\
    VALUEINT[NUMP][PARTICLE]=IN;\
}

Double_t minX,   maxX;
Int_t    minBIN, maxBIN;

#define MINMAX(II,A,SIGMA,t,Xmin,Xmax){\
    Double_t Ymax, Ymin;\
    II->FixParameter(0,A);\
    II->FixParameter(1,SIGMA);\
    II->FixParameter(2,t);\
    Double_t dx=0.001;\
    Double_t X = t;\
    Ymin= X;\
    Double_t IN=II->Eval(Ymin);\
    while(IN>0.5) {\
      Ymin=Ymin-dx;\
      IN=II->Eval(Ymin);\
    }\
    X = t;\
    Ymax = X;\
    IN=II->Eval(Ymax);\
    while(IN>0.5) {\
      Ymax=Ymax+dx;\
      IN=II->Eval(Ymax);\
    }\
    Xmin=Ymin;\
    Xmax=Ymax;\
}


double Terehof(char * filename);
double rMomentum(char * filenameM);
int    rTarget(char * filenameT, int rTargetFlag);

int main(int argc, char ** argv)
{
  FILE *data_file;
  int flopen, s, ss, tt, n;

  printf("argc=%d EXE=%s\n",argc,argv[0]);          //Opening file with list of files
  if(argc>1){
    printf("File= %s ",argv[1]);
    data_file=fopen(argv[1], "r");
    if (data_file>0) { 
      printf(" opened succefully\n");
    }
    else {
      printf(" can't open\n ");
      exit(1);
    }
  }
  else {
    printf("File not found\n");
    exit(1);
  }

  

  bool FlagTarget    = false;
  bool FlagBackround = false;
  int FilterTarget   = 0;
  int num0 = 0;  Double_t p0[Np];
  int num1 = 0;  Double_t p1[Np];
  int num2 = 0;  Double_t p2[Np];
  int num3 = 0;  Double_t p3[Np];
  int num4 = 0;  Double_t p4[Np];
  int num5 = 0;  Double_t p5[Np];
  int num6 = 0;  Double_t p6[Np];


  int numb0 = 0;  Double_t pb0[Np];
  int numb1 = 0;  Double_t pb1[Np];
  int numb2 = 0;  Double_t pb2[Np];
  int numb3 = 0;  Double_t pb3[Np];
  int numb4 = 0;  Double_t pb4[Np];
  int numb5 = 0;  Double_t pb5[Np];
  int numb6 = 0;  Double_t pb6[Np]; 
  
  Double_t pp;
  Double_t  pb[Np][7];
  Int_t numPb=0;

  int i_tar = 0;

  for(int i=0;i<Np;i++){
    for(int j=0;j<7;j++){
      Ipi[i][j]=0;
      Ik[i][j]=0;
      Ip[i][j]=0;
      Id[i][j]=0;
    }
  }

 bool FlagTargetC = false;  // Исключения по максимальному пику
 bool FlagTargetW = false;

 bool FlagMinusFon = false;

 TH1F* HistBackground = new TH1F("HistBackground","Background",801,0.,40.);
 TH1F* HBackground = new TH1F("HBackground","Background",801,0.,40.);
 TH1F* HistAll        = new TH1F("HistAll","Background + signal",801,0.,40.);
 TH1F* hnew           = new TH1F("hnew","signal",801,0.,40.);

 Double_t diff1[900][Np][Ntar]; Int_t coef1[Np][Ntar];
 Double_t diff2[900][Np][Ntar]; Int_t coef2[Np][Ntar];
 Double_t diff[900][Np][Ntar];
 Double_t backround[900][Np][Ntar];

 char file_name[MYSIZE][Np][Ntar];
 
 Int_t coef;
 int TargetNum;

 string TitleFile;

//  TApplication theApp("Gr",&argc,argv);

//*****OPTION GRAPH**************
 gStyle->SetTitleXSize(0.045); 
 gStyle->SetTitleYSize(0.045); 
 gStyle->SetLabelSize(0.045,"X");
 gStyle->SetLabelSize(0.045,"Y");  
//*******************************

//*************SigmaP polinom***************************************************************************
  Double_t AA =  6.16247;
  Double_t B = -12.2596;
  Double_t C =  10.764;
  Double_t D =  -5.08367;
  Double_t E =   1.34555;
  Double_t F =  -0.18807;
  Double_t G =   0.0108104;

  TF1* SigmaP_Teor = new TF1("SigmaPTeor",polinom6,0,7,7);
   SigmaP_Teor->FixParameter(0,AA);
   SigmaP_Teor->FixParameter(1,B);
   SigmaP_Teor->FixParameter(2,C);
   SigmaP_Teor->FixParameter(3,D);
   SigmaP_Teor->FixParameter(4,E);
   SigmaP_Teor->FixParameter(5,F);
   SigmaP_Teor->FixParameter(6,G);  
//********************************************************************************************************** 

//*************SigmaD polinom***************************************************************************
  Double_t AA_d =  2.43635 ;
  Double_t B_d  = -1.64381;
  Double_t C_d  =  0.446358;
  Double_t D_d  = -0.0526904;
  Double_t E_d  =  0.00218531;


  TF1* SigmaD_Teor = new TF1("SigmaPTeor",polinom4,0,7,7);
   SigmaD_Teor->FixParameter(0,AA_d);
   SigmaD_Teor->FixParameter(1,B_d);
   SigmaD_Teor->FixParameter(2,C_d);
   SigmaD_Teor->FixParameter(3,D_d);
   SigmaD_Teor->FixParameter(4,E_d);

//********************************************************************************************************** 


//********************************************************************************************************** 

  TCanvas *c1 = new TCanvas("c1","Graphs",200,10,700,500);

  TF1 * f1 = new TF1("f1",FitFunction1,-0.7,1.0,3);
  TF1 * f2 = new TF1("f2",FitFunction2,-0.7,20.,9);
  TF1 * fPi = new TF1("fPi",FitPiK,-0.7,5.,6);
  TF1 * GPion = new TF1("GPion",FitFunction1,-2,20,3);
  TF1 * GProton = new TF1("GProton",FitFunction1,-2,20,3);
  TF1 * GKaon = new TF1("GKaon",FitFunction1,-2,20,3);


  TH1 *HistPi1; 
  TH1 *HistPi2; 
  TH1 *HistPi3; 
  TH1 *HistP1; 
  TH1 *HistP2; 
  TH1 *HistP3; 
  TH1 *HistD1; 
  TH1 *HistD2; 
  TH1 *HistD3; 

  Double_t Xmax, Xmin;
  TF1 * I = new TF1("I","[0]/([1]*sqrt(2*3.14159265))*exp(-(x-[2])*(x-[2])/([1]*[1]))",Xmin,Xmax);

  f1->SetParNames("A","Sigma","t");
  TFile *rootFile15 = new TFile(rootresult15,"recreate");
    rootFile15->mkdir("pion");
    rootFile15->mkdir("proton");
    rootFile15->mkdir("deutron");
    rootFile15->Close();
  TFile *FileBackground = new TFile(rootBackground,"recreate");
    FileBackground->Close();

  TFile *rootFile16 = new TFile(rootresult16,"recreate");
  rootFile16->Close();
  TFile *rootFile17 = new TFile(rootresult17,"recreate");
  rootFile17->Close();
  num = 0;
                                  
  while(fgets(dfname,MYSIZE,data_file)!= NULL){                   //Opening file of list

    ss = 0;
    n = 0;
  
    num++;

    for(s=0;s<MYSIZE;s++){
      c = dfname[s];
      if((!c) || (c=='\r') || (c=='\n') || (c=='|')) break;
    }
    if(c=='|') {s=s-1;}  

    if(s<MYSIZE){
      dfname[s]=0;
      printf("File = %s",dfname);
      printf("\n");

//******************************************************************
    for( tt=0;tt<s;tt++){
      ch1 = dfname[tt];
      terFName[tt]=dfname[tt];
      if(ch1=='r' && dfname[tt-1]=='.') break;
    }
    if(ch1=='r') {tt=tt-1;}
    if(tt<s){
      terFName[tt]=0;
      printf("terFName = %s",terFName);     
    }

    coef            = Terehof(terFName);   
    i_tar           = rTarget(terFName,1); 
    FilterTarget    = rTarget(terFName,0);
    pp              = rMomentum(terFName);
    pp=floor(pp/0.1+0.5)*0.1; 

    if(FilterTarget==0){
      FlagBackround=false;
      if(i_tar==0)                          {num0++; p[num0-1][i_tar] = pp;} //Al-target
      else if(i_tar==1)                     {num1++; p[num1-1][i_tar] = pp;} //Cu-target
           else if(i_tar==2)                {num2++; p[num2-1][i_tar] = pp;} //W-target
                else if(i_tar==3)           {num3++; p[num3-1][i_tar] = pp;} //no_target
                     else if(i_tar==4)      {num4++; p[num4-1][i_tar] = pp;} //PE-target
                          else if(i_tar==5) {num5++; p[num5-1][i_tar] = pp;} //C-target
                               else {printf("Error in target number(i_tar)\n"); break;}
    }
    else if(FilterTarget==1){
           FlagBackround=true;
           if(i_tar==0)                          {numb0++; pb[numb0-1][i_tar] = pp;}
           else if(i_tar==1)                     {numb1++; pb[numb1-1][i_tar] = pp;}
                else if(i_tar==2)                {numb2++; pb[numb2-1][i_tar] = pp;}
                     else if(i_tar==3)           {numb3++; pb[numb3-1][i_tar] = pp;}
                          else if(i_tar==4)      {numb4++; pb[numb4-1][i_tar] = pp;}
                               else if(i_tar==5) {numb5++; pb[numb5-1][i_tar] = pp;}
                                    else {printf("Error in target number(i_tar)\n"); break;}
    }
         else { printf("Error in targets filter\n"); break;}


//********************************************************************  




      TFile * f = new TFile(dfname);
      if(f>0){
        printf(" opened succesfully\n");
          if(i_tar==0)                          {numP = num0; numPb = numb0;}
          else if(i_tar==1)                     {numP = num1; numPb = numb1;}
               else if(i_tar==2)                {numP = num2; numPb = numb2;}
                    else if(i_tar==3)           {numP = num3; numPb = numb3;}
                         else if(i_tar==4)      {numP = num4; numPb = numb4;}
                              else if(i_tar==5) {numP = num5; numPb = numb5;}

//********************************MINUS Background***************************************
          printf("tt=%d\n",tt);
          if(FlagBackround==true){
            f->cd("RPC;1");
            TH1* hist_test1;
            gDirectory->GetObject(rootname15,hist_test1);
            HistBackground = (TH1F*)hist_test1->Clone();
            coef2[numPb-1][i_tar]=coef; printf("coef2=%f, numPb=%d i_tat=%d\n",coef2[numPb-1][i_tar], numPb,i_tar);
            for(Int_t i=0;i<=800;i++){ 
              diff2[i][numPb-1][i_tar] = HistBackground->GetBinContent(i); 


            }
            //FlagMinusFon=false;

            FlagBackround=false;
          }
          else{
            f->cd("RPC;1");
            TH1* hist_test2;
            gDirectory->GetObject(rootname15, hist_test2);
            HistAll = (TH1F*)hist_test2->Clone();
            coef1[numP-1][i_tar]=coef;
            for(Int_t i=0;i<=800;i++){ 
              diff1[i][numP-1][i_tar] = HistAll->GetBinContent(i);   
              //FlagMinusFon=true;
            }
            for(int i=0;i<100;i++){
               file_name[i][numP-1][i_tar]=dfname[i];
            }      
          }
          

//*****************************************************************************************

//*****************************************************************************************
        
      }
      else{
        printf("can't open\n");
        continue; 
      }
      f->Close();
    }

  }
printf("WORK\n");
  Int_t numM;
  for(int k=0;k<=5;k++){ 
    if(k==0)                           {numM=num0;}
    else if(k==1)                      {numM=num1;}
         else if(k==2)                 {numM=num2;}
              else if(k==5)            {numM=num5;}
                   else if(k==4)       {numM=num4;}
 printf("WORK2\n");                       
    i_tar=k;
    for(int j=0;j<numM;j++){ 
      for(int i=0;i<=800;i++){  printf("i=%d j=%d k=%d coef2=%f coef1=%f\n",i,j,k,coef2[j][k],coef1[j][3]);
        if(coef1[j][3]==0 && coef2[j][3]==0 && coef2[j][k]==0){
          backround[i][j][k]=0;
          TitleFile = "with the background";
        }
        else{
          if (coef1[j][3]==0 && coef2[j][3]==0 && coef2[j][k]!=0){
            backround[i][j][k]=diff2[i][j][k]*(coef1[j][k]/coef2[j][k]);
            TitleFile = "minus target_background";
          }
          else{
            backround[i][j][k]=diff2[i][j][k]*(coef1[j][k]/coef2[j][k])+diff1[i][j][6]*(coef1[j][k]/coef1[j][3])-diff2[i][j][3]*(coef1[j][k]/coef2[j][3]);
            TitleFile = "minus background";
          }
        }
        diff[i][j][k]=diff1[i][j][k]-backround[i][j][k];
        if(diff[i][j][k]<0) {diff[i][j][k]=0.;} 
        HBackground->SetBinContent(i,backround[i][j][k]); 
        hnew->SetBinContent(i,diff[i][j][k]); 
      }
          numP=j+1; 
          for(int i=0;i<100;i++){
             dfnameF[i]=file_name[i][j][k];
          }  
          //-------------------------Bacround write--------------------------------------
            TFile *FileBackground = new TFile(rootBackground,"UPDATE");
              HBackground->Write(dfnameF);
              FileBackground->Close();
          //------------------------------End--------------------------------------------
        
          //----------------------------Pi Fitt------------------------------------------
            //-----1 Fitt
            HistPi1 = (TH1F*)hnew->Clone();

            tpi[numP-1][i_tar] = 3.5;
            f1->SetNpx(10000);
            f1->SetParameter(0, 10000);              f1->SetParLimits(0,0.,15000.);
            f1->SetParameter(1, 0.14);               f1->SetParLimits(1,0.07,0.21);
            f1->SetParameter(2, tpi[numP-1][i_tar]); if(p[numP-1][i_tar]<2.5) {f1->SetParLimits(2,3.,4.0);} 
                                                     else                     {f1->SetParLimits(2,3.,3.6);}
             HistPi1->Fit("f1");
             tpi[numP-1][i_tar]=f1->GetParameter(2);


            tp[numP-1][i_tar]  = tpi[numP-1][i_tar]+LenTOF*(sqrt(mP*mP+p[numP-1][i_tar]*p[numP-1][i_tar])-sqrt(mPi*mPi+p[numP-1][i_tar]*p[numP-1][i_tar]))/p[numP-1][i_tar]/3*10; 
            tk[numP-1][i_tar]  = tpi[numP-1][i_tar]+LenTOF*(sqrt(mK*mK+p[numP-1][i_tar]*p[numP-1][i_tar])-sqrt(mPi*mPi+p[numP-1][i_tar]*p[numP-1][i_tar]))/p[numP-1][i_tar]/3*10; 

            f1->SetNpx(10000);
            f1->SetParameter(0, 10000);              f1->SetParLimits(0,0.,15000.);
            f1->SetParameter(1, 0.14);               f1->SetParLimits(1,0.07,0.21);
            f1->SetParameter(2, tpi[numP-1][i_tar]); f1->SetParLimits(2,tpi[numP-1][i_tar]-0.05*tpi[numP-1][i_tar],tpi[numP-1][i_tar]+0.05*tpi[numP-1][i_tar]);
            
            HistPi1->Fit("f1");
            tpi[numP-1][i_tar]=f1->GetParameter(2);

            //-----2 Fitt
            HistPi2 = (TH1F*)hnew->Clone();

            f1->SetNpx(10000);
            f1->SetParameter(0, 10000);             f1->SetParLimits(0,0.,15000.);
            f1->SetParameter(1, 0.14);               f1->SetParLimits(1,0.07,0.21);
            f1->FixParameter(2, tpi[numP-1][i_tar]); 

            HistPi2->Fit("f1");
            Spi[numP-1][i_tar]=f1->GetParameter(1);

            //-----3 Fitt
            HistPi3  = (TH1F*)hnew->Clone();

            f1->SetNpx(10000);
            f1->SetParameter(0, 10000);             f1->SetParLimits(0,0.,15000.);
            f1->FixParameter(1, Spi[numP-1][i_tar]);               
            f1->FixParameter(2, tpi[numP-1][i_tar]); 

            HistPi3->Fit("f1");
            Api[numP-1][i_tar]=f1->GetParameter(0);

            TFile *rootFile1 = new TFile(rootresult15,"UPDATE");
            HistPi3->SetTitle(dfnameF);
            rootFile1->cd("pion");
            HistPi3->Write(dfnameF);
            rootFile1->Close();
            
            // Если амлитуда < 30, то считать сумму бинов
            if(Api[numP-1][i_tar]<30){        
              MINMAX(I,Api[numP-1][i_tar],Spi[numP-1][i_tar],tpi[numP-1][i_tar],minX,maxX);
              minBIN = HistPi3->FindBin(minX)-1;
              maxBIN = HistPi3->FindBin(maxX)+1;
              Ipi[numP-1][i_tar] = HistPi3->Integral(minBIN,maxBIN);
            }
            else{
              INTEGRALF(I,Api[numP-1][i_tar],Spi[numP-1][i_tar],tpi[numP-1][i_tar],Ipi,(numP-1),i_tar);
            }
          

          //----------------------------P Fitt------------------------------------------

            //-----1 Fitt
            HistP1 = (TH1F*)hnew->Clone();

            Sp[numP-1][i_tar]=SigmaP_Teor->Eval(p[numP-1][i_tar]);          
  
            f1->SetNpx(10000);
            f1->SetParameter(0, 10000);              f1->SetParLimits(0,0.,15000.);
            f1->SetParameter(1, Sp[numP-1][i_tar]);  f1->SetParLimits(1,Sp[numP-1][i_tar]-0.5*Sp[numP-1][i_tar],Sp[numP-1][i_tar]+0.5*Sp[numP-1][i_tar]);
            f1->SetParameter(2, tp[numP-1][i_tar]);  f1->SetParLimits(2,tp[numP-1][i_tar]-0.05*tp[numP-1][i_tar],tp[numP-1][i_tar]+0.05*tp[numP-1][i_tar]);
            
            HistP1->Fit("f1");
            tp[numP-1][i_tar]=f1->GetParameter(2);

            //-----2 Fitt
            HistP2 = (TH1F*)hnew->Clone();
             
            f1->SetNpx(10000);
            f1->SetParameter(0, 10000);             f1->SetParLimits(0,0.,15000.);
            f1->SetParameter(1, Sp[numP-1][i_tar]);  f1->SetParLimits(1,Sp[numP-1][i_tar]-0.5*Sp[numP-1][i_tar],Sp[numP-1][i_tar]+0.5*Sp[numP-1][i_tar]);
            f1->FixParameter(2, tp[numP-1][i_tar]); 

            HistP2->Fit("f1");
            Sp[numP-1][i_tar]=f1->GetParameter(1);

            //-----3 Fitt
            HistP3 = (TH1F*)hnew->Clone();

            f1->SetNpx(10000);
            f1->SetParameter(0, 10000);             f1->SetParLimits(0,0.,15000.);
            f1->FixParameter(1, Sp[numP-1][i_tar]);               
            f1->FixParameter(2, tp[numP-1][i_tar]); 

            HistP3->Fit("f1");
            Ap[numP-1][i_tar]=f1->GetParameter(0);

            TFile *rootFile2 = new TFile(rootresult15,"UPDATE");
            HistP3->SetTitle(dfnameF);
            rootFile2->cd("proton");
            HistP3->Write(dfnameF);
            rootFile2->Close();

            if(p[numP-1][i_tar]<=minPd) {rootFile15->Close();}

            INTEGRALF(I,Ap[numP-1][i_tar],Sp[numP-1][i_tar],tp[numP-1][i_tar],Ip,(numP-1),i_tar);

         
          if(p[numP-1][i_tar]>minPd){
          //----------------------------D Fitt------------------------------------------

            //-----1 Fitt

            HistD1 = (TH1F*)hnew->Clone();

            tp[numP-1][i_tar]  = HistD1->GetMaximumBin()*0.05;
            //tpi[numP-1][i_tar] = tp[numP-1][i_tar]-LenTOF*(sqrt(mP*mP+p[numP-1][i_tar]*p[numP-1][i_tar])-sqrt(mPi*mPi+p[numP-1][i_tar]*p[numP-1][i_tar]))/p[numP-1][i_tar]/3*10; 
            //tk[numP-1][i_tar]  = tp[numP-1][i_tar]-LenTOF*(sqrt(mP*mP+p[numP-1][i_tar]*p[numP-1][i_tar])-sqrt(mK*mK+p[numP-1][i_tar]*p[numP-1][i_tar]))/p[numP-1][i_tar]/3*10; 
            td[numP-1][i_tar]  = tp[numP-1][i_tar]+LenTOF*(sqrt(mD*mD+p[numP-1][i_tar]*p[numP-1][i_tar])-sqrt(mP*mP+p[numP-1][i_tar]*p[numP-1][i_tar]))/p[numP-1][i_tar]/3*10; 

            Sd[numP-1][i_tar]=SigmaD_Teor->Eval(p[numP-1][i_tar]);          
  
            f1->SetNpx(10000);
            f1->SetParameter(0, 10000);              f1->SetParLimits(0,0.,15000.);
            f1->SetParameter(1, Sd[numP-1][i_tar]);  f1->SetParLimits(1,Sd[numP-1][i_tar]-0.5*Sd[numP-1][i_tar],Sd[numP-1][i_tar]+0.5*Sd[numP-1][i_tar]);
            f1->SetParameter(2, td[numP-1][i_tar]);  f1->SetParLimits(2,td[numP-1][i_tar]-0.05*td[numP-1][i_tar],td[numP-1][i_tar]+0.05*td[numP-1][i_tar]);
            
            HistD1->Fit("f1");
            td[numP-1][i_tar]=f1->GetParameter(2);

            //-----2 Fitt
            HistD2 = (TH1F*)hnew->Clone();

            f1->SetNpx(10000);
            f1->SetParameter(0, 10000);              f1->SetParLimits(0,0.,15000.);
            f1->SetParameter(1, Sd[numP-1][i_tar]);  f1->SetParLimits(1,Sd[numP-1][i_tar]-0.5*Sd[numP-1][i_tar],Sd[numP-1][i_tar]+0.5*Sd[numP-1][i_tar]);
            f1->FixParameter(2, td[numP-1][i_tar]); 

            HistD2->Fit("f1");
            Sd[numP-1][i_tar]=f1->GetParameter(1);

            //-----3 Fitt
            HistD3 = (TH1F*)hnew->Clone();

            f1->SetNpx(10000);
            f1->SetParameter(0, 10000);             f1->SetParLimits(0,0.,15000.);
            f1->FixParameter(1, Sd[numP-1][i_tar]);               
            f1->FixParameter(2, td[numP-1][i_tar]); 

            HistD3->Fit("f1");
            Ad[numP-1][i_tar]=f1->GetParameter(0);

            TFile *rootFile3 = new TFile(rootresult15,"UPDATE");
            HistD3->SetTitle(dfnameF);
            rootFile3->cd("deutron");
            HistD3->Write(dfnameF);
            rootFile3->Close();

            // Если амлитуда < 30, то считать сумму бинов. Иначе интеграл            
            if(Ad[numP-1][i_tar]<30){        
              MINMAX(I,Ad[numP-1][i_tar],Sd[numP-1][i_tar],td[numP-1][i_tar],minX,maxX); 
              minBIN = (HistD3->FindBin(minX))-1;     
              maxBIN = (HistD3->FindBin(maxX))+1;
              Id[numP-1][i_tar] = HistD3->Integral(minBIN,maxBIN);
            }
            else{
            INTEGRALF(I,Ad[numP-1][i_tar],Sd[numP-1][i_tar],td[numP-1][i_tar],Id,(numP-1),i_tar);
            } 

          } 
   
          //FlagMinusFon=false;
         
       
          
//*****************************************************************************************
      }      
    }


   for(int i=0;i<num0;i++){
     p0[i]= p[i][0];
     SigmaPi_Al[i]=Spi[i][0];
     SigmaP_Al[i]=Sp[i][0];
     SigmaD_Al[i]=Sd[i][0];
   }
   for(int i=0;i<num1;i++){
     p1[i]= p[i][1];
     SigmaPi_Cu[i]=Spi[i][1];
     SigmaP_Cu[i]=Sp[i][1];
     SigmaD_Cu[i]=Sd[i][1];
   } 
   for(int i=0;i<num2;i++){
     p2[i]= p[i][2];
     SigmaPi_W[i]=Spi[i][2];
     SigmaP_W[i]=Sp[i][2];
     SigmaD_W[i]=Sd[i][2];
   } 

   for(int i=0;i<num4;i++){
     p4[i]= p[i][4];
     SigmaPi_PE[i]=Spi[i][4];
     SigmaP_PE[i]=Sp[i][4];
     SigmaD_PE[i]=Sd[i][4];
   }

   for(int i=0;i<num5;i++){
     p5[i]= p[i][5];
     SigmaPi_C[i]=Spi[i][5];
     SigmaP_C[i]=Sp[i][5];
     SigmaD_C[i]=Sd[i][5];
     printf("p5=%f Sigma=%f\n",p5[i],SigmaPi_C[i]);
   }
    
   TGraph * GSigmaPi_Al = new TGraph(num0,p0,SigmaPi_Al); 
   TGraph * GSigmaP_Al  = new TGraph(num0,p0,SigmaP_Al);
   TGraph * GSigmaD_Al  = new TGraph(num0,p0,SigmaD_Al);  

   TGraph * GSigmaPi_Cu = new TGraph(num1,p1,SigmaPi_Cu); 
   TGraph * GSigmaP_Cu  = new TGraph(num1,p1,SigmaP_Cu);
   TGraph * GSigmaD_Cu  = new TGraph(num1,p1,SigmaD_Cu);  

   TGraph * GSigmaPi_W = new TGraph(num2,p2,SigmaPi_W); 
   TGraph * GSigmaP_W  = new TGraph(num2,p2,SigmaP_W);
   TGraph * GSigmaD_W  = new TGraph(num2,p2,SigmaD_W);  

   TGraph * GSigmaPi_PE = new TGraph(num4,p4,SigmaPi_PE);
   TGraph * GSigmaP_PE  = new TGraph(num4,p4,SigmaP_PE); 
   TGraph * GSigmaD_PE  = new TGraph(num4,p4,SigmaD_PE);   

   TGraph * GSigmaPi_C = new TGraph(num5,p5,SigmaPi_C); 
   TGraph * GSigmaP_C  = new TGraph(num5,p5,SigmaP_C);
   TGraph * GSigmaD_C  = new TGraph(num5,p5,SigmaD_C);  


 
   Double_t Idp_0[Np],    Idp_1[Np],    Idp_2[Np],    Idp_4[Np],    Idp_5[Np];
   Double_t ErrIdp_0[Np], ErrIdp_1[Np], ErrIdp_2[Np], ErrIdp_4[Np], ErrIdp_5[Np];
   Double_t Pmom0[Np],    Pmom1[Np],    Pmom2[Np],    Pmom4[Np],    Pmom5[Np];
   
   Double_t LenTOF[20];

   for(int i=0;i<num0;i++){
     Idp_0[i]=Id[i][0]/Ip[i][0];
     ErrIdp_0[i] = (Id[i][0]/Ip[i][0])*sqrt(1/Id[i][0]+1/Ip[i][0]);
     Pmom0[i]=p[i][1];
     LenTOF[i]=(tp[i][0]-tpi[i][0])*Pmom0[i]*0.3/(sqrt(Pmom0[i]*Pmom0[i]+mP*mP)-sqrt(Pmom0[i]*Pmom0[i]+mPi*mPi));
     printf("len=%f p=%f\n",LenTOF[i],Pmom0[i]);
   }
   for(int i=0;i<num1;i++){
     Idp_1[i]=Id[i][1]/Ip[i][1];
     ErrIdp_1[i] = (Id[i][1]/Ip[i][1])*sqrt(1/Id[i][1]+1/Ip[i][1]); 
     Pmom1[i]=p[i][1];
   }
   for(int i=0;i<num2;i++){
     Idp_2[i]=Id[i][2]/Ip[i][2];
     ErrIdp_2[i] = (Id[i][2]/Ip[i][2])*sqrt(1/Id[i][2]+1/Ip[i][2]);
     Pmom2[i]=p[i][2];
   }
   for(int i=0;i<num5;i++){
     Idp_5[i]=Id[i][5]/Ip[i][5];
     ErrIdp_5[i] = (Id[i][5]/Ip[i][5])*sqrt(1/Id[i][5]+1/Ip[i][5]);
     Pmom5[i]=p[i][5];
   }
   for(int i=0;i<num4;i++){
     Idp_4[i]=Id[i][4]/Ip[i][4];
     ErrIdp_4[i] = (Id[i][4]/Ip[i][4])*sqrt(1/Id[i][4]+1/Ip[i][4]);
     Pmom4[i]=p[i][4];
   }

   TGraphErrors *Gdp_0  = new TGraphErrors(num0,Pmom0,Idp_0,0,ErrIdp_0);     Gdp_0->SetMarkerColor(1);
   TGraphErrors *Gdp_1  = new TGraphErrors(num1,Pmom1,Idp_1,0,ErrIdp_1);     Gdp_1->SetMarkerColor(1);
   TGraphErrors *Gdp_2  = new TGraphErrors(num2,Pmom2,Idp_2,0,ErrIdp_2);     Gdp_2->SetMarkerColor(1);
   TGraphErrors *Gdp_4  = new TGraphErrors(num4,Pmom4,Idp_4,0,ErrIdp_4);     Gdp_4->SetMarkerColor(1);
   TGraphErrors *Gdp_5  = new TGraphErrors(num5,Pmom5,Idp_5,0,ErrIdp_5);     Gdp_5->SetMarkerColor(1);

   TGraph *G_TOF  = new TGraph(num0,Pmom0,LenTOF);     G_TOF->SetMarkerColor(1);
  
 

//************************************DATA-FILE*******************************************************************
 TFile *rootFileDATA = new TFile(rootDATA,"recreate");

  rootFileDATA->mkdir("LenTOF");
  rootFileDATA->cd("LenTOF");
    TCanvas *Len_TOF= new TCanvas("Len_TOF","Len_TOF",200,10,700,500);
    G_TOF->SetTitle("LenTOF;p, GeV/c;l,m");   G_TOF->Draw("A*");  Len_TOF->Write("Len_TOF");
     

  rootFileDATA->mkdir("SigmaPi");
  rootFileDATA->cd("SigmaPi");
    TCanvas *Sigma= new TCanvas("Sigma","#sigma",200,10,700,500);
     GSigmaPi_C->SetTitle("C - target;p, GeV/c;#sigma_{#pi}");   GSigmaPi_C->Draw("A*");  Sigma->Write("C_target");
     GSigmaPi_Al->SetTitle("Al - target;p, GeV/c;#sigma_{#pi}"); GSigmaPi_Al->Draw("A*"); Sigma->Write("Al_target");
     GSigmaPi_Cu->SetTitle("Cu - target;p, GeV/c;#sigma_{#pi}"); GSigmaPi_Cu->Draw("A*"); Sigma->Write("Cu_target");
     GSigmaPi_W->SetTitle("W - target;p, GeV/c;#sigma_{#pi}");   GSigmaPi_W->Draw("A*");  Sigma->Write("W_target");
     GSigmaPi_PE->SetTitle("PE - target;p, GeV/c;#sigma_{#pi}"); GSigmaPi_PE->Draw("A*"); Sigma->Write("PE_target");

  rootFileDATA->mkdir("SigmaP");
  rootFileDATA->cd("SigmaP");
     GSigmaP_C->SetTitle("C - target;p, GeV/c;#sigma_{p}");   GSigmaP_C->Draw("A*");  Sigma->Write("C_target");
     GSigmaP_Al->SetTitle("Al - target;p, GeV/c;#sigma_{p}"); GSigmaP_Al->Draw("A*"); Sigma->Write("Al_target");
     GSigmaP_Cu->SetTitle("Cu - target;p, GeV/c;#sigma_{p}"); GSigmaP_Cu->Draw("A*"); Sigma->Write("Cu_target");
     GSigmaP_W->SetTitle("W - target;p, GeV/c;#sigma_{p}");   GSigmaP_W->Draw("A*");  Sigma->Write("W_target");
     GSigmaP_PE->SetTitle("PE - target;p, GeV/c;#sigma_{p}"); GSigmaP_PE->Draw("A*"); Sigma->Write("PE_target");

  rootFileDATA->mkdir("SigmaD");
  rootFileDATA->cd("SigmaD");
     GSigmaD_C->SetTitle("C - target;p, GeV/c;#sigma_{d}");   GSigmaD_C->Draw("A*");  Sigma->Write("C_target");
     GSigmaD_Al->SetTitle("Al - target;p, GeV/c;#sigma_{d}"); GSigmaD_Al->Draw("A*"); Sigma->Write("Al_target");
     GSigmaD_Cu->SetTitle("Cu - target;p, GeV/c;#sigma_{d}"); GSigmaD_Cu->Draw("A*"); Sigma->Write("Cu_target");
     GSigmaD_W->SetTitle("W - target;p, GeV/c;#sigma_{d}");   GSigmaD_W->Draw("A*");  Sigma->Write("W_target");
     GSigmaD_PE->SetTitle("PE - target;p, GeV/c;#sigma_{d}"); GSigmaD_PE->Draw("A*"); Sigma->Write("PE_target");


  rootFileDATA->mkdir("d_p");
  rootFileDATA->cd("d_p");
     Gdp_0->SetTitle("C - target;p, GeV/c;d/p");  Gdp_0->Draw("A*");  Sigma->Write("Al_target");
     Gdp_1->SetTitle("Al - target;p, GeV/c;d/p"); Gdp_1->Draw("A*");  Sigma->Write("Cu_target");
     Gdp_2->SetTitle("Cu - target;p, GeV/c;d/p"); Gdp_2->Draw("A*");  Sigma->Write("W_target");
     Gdp_4->SetTitle("W - target;p, GeV/c;d/p");  Gdp_4->Draw("A*");  Sigma->Write("PE_target");
     Gdp_5->SetTitle("PE - target;p, GeV/c;d/p"); Gdp_5->Draw("A*");  Sigma->Write("C_target");

 rootFileDATA->Close();

return 0;
}




