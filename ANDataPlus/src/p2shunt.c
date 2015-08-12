#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "p2shunt.h"

float *p2shunt(double p, int Version)
{
  /*
  // M. Ukhanov 6-Nov-2010
  // the function p2shunt converts particle momentum to magnet 
  // shunt voltages.
  // The conversion formulas are provided by Artem Semak.
  // The function returns a pointer to an internal static array
  // which contains the calculated voltages.
  */
 double q1, q2, q3, q4, m1, m2, m3, moment;
 double fm2, vm2, xx, tmp[3];
 static float Shunt[7];
 int i, j;
 
   Version = 5; //  40 degree 
   moment = p;
   switch(Version) {
//---------------------------------------------------------------------------------------

   case 0:  // Old version of acceptance (2009)
  
     q1 = -2.*moment;
     q2 = 1.44*moment;
     q3 = -1.*moment;
     q4 = 0.45*moment;
     
     m1 = 5.192*moment/3.0;
     fm2 = (2.420922e-3) - 1.89546*moment -(7.06497e-3)*moment*moment 
          -(1.714418e-3)*moment*moment*moment;
     m3 =-0.21553 + 2.3533*moment -0.067443*moment*moment 
          + 0.0053338*moment*moment*moment;
 
     if(q1 < -13.3) q1 = -13.3;
     if(q2 > 13.3)  q2 = 13.3;
     if(q3 < -13.3) q3 = -13.3;
     if(q4 > 13.3)  q4 = 13.3;
 
     if(m1 > 14.5)  m1 = 14.5; /* istochnik derjit tolko do 1200 A */
     if(m2 < -14.1) m2 = -14.1; /* maksimalniy tok CP-94 - 640 A*/
     if(m3 > 18.)   m3 = 18.;
 
  /* -------------------- kalibrovki ----------- */
  
     q1 = q1*5.0/(1600.*0.01);
     q2 = q2*5.0/(1600.*0.01);
     q3 = q3*5.0/(1600.*0.01);
     q4 = q4*5.0/(1600.*0.01);
 
     m1 = m1*35.*1.5955/(13.635*16.);
     m3 = m3 *1.5955/13.635;
 
     q1 *= -1.;  q3 *= -1.; fm2 *= -1.;
 
     vm2 = 0.;
     for(j=0;j<1000000; j++){
       xx = 100. + (double)j*2100./1.e6; 
       m2 =-(2.868954e-2)+(7.215012e-3)*xx+(2.302251e-6)*xx*xx
	   -(1.577176e-9)*xx*xx*xx
	   +(1.807626e-13)*xx*xx*xx*xx;
       
       if(m2 > fm2*0.99999 && m2 < fm2*1.00001) {vm2 = xx; break;}
     }
     vm2 *= 1.02e-3;  /*popravka na 2 %!!!*/
   break;

//---------------------------------------------------------------------------------------

   ///////////////////////////////////////////////////////////////////////////////////
   case 1:  // New version of acceptance (2010-I)
 
     q1 = -2.0*moment; 
     q2 =  1.596*moment;  
     q3 = -1.3151*moment*0.94; 
     q4 = 0.6438*moment;
     
     m1  = 1.7333*moment;
     fm2 =-0.11119 -1.7574*moment -0.045852*moment*moment 
          +0.00197*moment*moment*moment;
     m3  =-0.21553 + 2.3533*moment -0.067443*moment*moment 
          + 0.0053338*moment*moment*moment;
 
     if(q1 < -13.3) q1 = -13.3;
     if(q2 > 13.3)  q2 = 13.3;
     if(q3 < -13.3) q3 = -13.3;
     if(q4 > 13.3)  q4 = 13.3;
 
     if(m1 > 14.5)  m1 = 14.5; /* istochnik derjit tolko do 1200 A */
     if(m2 < -14.1) m2 = -14.1; /* maksimalniy tok CP-94 - 640 A*/
     if(m3 > 18.)  m3 = 18.;
 
  /* -------------------- kalibrovki ----------- */
  
     q1 = q1*5.0/(1600.*0.01);
     q2 = q2*5.0/(1600.*0.01);
     q3 = q3*5.0/(1600.*0.01);
     q4 = q4*5.0/(1600.*0.01);
 
     m1 = m1*35.*1.5955/(13.635*16.);
     m3 = m3 *1.5955/13.635;
 
     q1 *= -1.;  q3 *= -1.; fm2 *= -1.;
 
     vm2 = 0.;
     for(j=0;j<1000000; j++){
       xx = 100. + (double)j*2100./1.e6; 
       m2 =-(2.868954e-2)+(7.215012e-3)*xx+(2.302251e-6)*xx*xx
	   -(1.577176e-9)*xx*xx*xx+(1.807626e-13)*xx*xx*xx*xx;
       
       if(m2 > fm2*0.99999 && m2 < fm2*1.00001) {vm2 = xx; break;}
     }
     vm2 *= 1.02e-3;  /*popravka na 2 %!!!*/
   break;
//---------------------------------------------------------------------------------------

   //////////////////////////////////////////////////////////////////////////////////////

   case 2:  // Version of acceptance (2010-II)
  
     q1 = -2.*moment;
     q2 = 1.44*moment;
     q3 = -1.*moment;
     q4 = 0.45*moment;
     
 m1 = 5.192*moment/3.0;
 fm2 =2.420922e-3 -1.89546*moment -7.06497e-3*moment*moment -1.714418e-3*moment*moment*moment;
 m3 =-0.21553 + 2.3533*moment -0.067443*moment*moment + 0.0053338*moment*moment*moment;
 
 if(q1 < -13.3) q1 = -13.3;
 if(q2 > 13.3)  q2 = 13.3;
 if(q3 < -13.3) q3 = -13.3;
 if(q4 > 13.3)  q4 = 13.3;
 
 if(m1 > 14.5)  m1 = 14.5; /* istochnik derjit tolko do 1200 A */
 if(m2 < -14.1) m2 = -14.1; /* maksimalniy tok SP-94 - 640 A*/
 if(m3 > 18.)  m3 = 18.;
 
  /* -------------------- kalibrovki ----------- */
  
   q1 = q1*5.0/(3500.*0.01);        /*pomenyali shunt s 7-im elementom  1600->3500*/
   q2 = q2*5.0/(1600.*0.01);
   q3 = q3*5.0/(1600.*0.01);
   q4 = q4*5.0/(1600.*0.01);
 
   m1 = m1*35.*1.5955/(13.635*16.);
   m3 = m3 *1.5955*2.1875/13.635;   /*pomenyali shunt s 1-im elementom  3500->1600*/
 
 q1 *= -1.;  q3 *= -1.; fm2 *= -1.;
 
    vm2 = 0.;
    for(j=0;j<1000000; j++){
       xx = 100. + (double)j*2100./1.e6; 
       m2 =-2.868954e-2+7.215012e-3*xx+2.302251e-6*xx*xx-1.577176e-9*xx*xx*xx+1.807626e-13*xx*xx*xx*xx;
       
       if(m2 > fm2*0.99999 && m2 < fm2*1.00001) {vm2 = xx; break;}
      }
    vm2 *= 1.02e-3;  /*popravka na 2 %!!!*/
   break;

//---------------------------------------------------------------------------------------

   /////////////////////////////////////////////////////////////////////////////////
   case 3: // Version of acceptance as of 2011 
           //  (run with Liquid Hydrogene target) 
           //  original file is: table35_newM2_10.04.2011.c

     q1 = -2.*moment;
     q2 = 1.54*moment;
     q3 = -1.34*moment;
     q4 = 1.0*moment;
     
     
 m1 = 5.192*moment/3.0;
 m2 =0.0838376 - 2.01874*moment + 0.0407697*moment*moment - 0.00579119*moment*moment*moment;
 m3 =-0.21553 + 2.3533*moment -0.067443*moment*moment + 0.0053338*moment*moment*moment;
 
 if(q1 < -13.3) q1 = -13.3;
 if(q2 > 13.3)  q2 = 13.3;
 if(q3 < -13.3) q3 = -13.3;
 if(q4 > 13.3)  q4 = 13.3;
 
 if(m1 > 14.5)  m1 = 14.5; /* istochnik derjit tolko do 1200 A */
 if(m2 < -17.) m2 = -17.; /* maksimalniy tok CP-94 - 640 A*/
 if(m3 > 18.)  m3 = 18.;
 
  /* -------------------- kalibrovki ----------- */
  
   q1 = q1*5.0/(3500.*0.01);        /*pomenyali shunt s 7-im elementom  1600->3500*/
   q2 = q2*5.0/(1600.*0.01);
   q3 = q3*5.0/(1600.*0.01);
   q4 = q4*5.0/(1600.*0.01);
 
   m1 = m1*35.*1.5955/(13.635*16.);
   m3 = m3 *1.5955*2.1875/13.635;   /*pomenyali shunt s 1-im elementom  3500->1600*/
 
  q1 *= -1.;  q3 *= -1.; m2 *= -1.;
 
  vm2 =  0.0119342 + 0.102905*m2 - 0.00680999*m2*m2 + 0.00134248*m2*m2*m2 - 0.000115031*m2*m2*m2*m2 +  3.68299e-06*m2*m2*m2*m2*m2;
  
  vm2 = -5.94754e-03 + vm2*0.990426;

     break; // 3

//---------------------------------------------------------------------------------------

   case 4: // old35table_02.11.2010_newM2.c  - Acceptance of 2009 with modified M2 pole tips
  
     q1 = -2.*moment;
     q2 = 1.44*moment;
     q3 = -1.*moment;
     q4 = 0.45*moment;
     
 m1 = 5.192*moment/3.0;
 m2 =2.420922e-3 -1.89546*moment -7.06497e-3*moment*moment -1.714418e-3*moment*moment*moment;
 m3 =-0.21553 + 2.3533*moment -0.067443*moment*moment + 0.0053338*moment*moment*moment;
 
 if(q1 < -13.3) q1 = -13.3;
 if(q2 > 13.3)  q2 = 13.3;
 if(q3 < -13.3) q3 = -13.3;
 if(q4 > 13.3)  q4 = 13.3;
 
 if(m1 > 14.5)  m1 = 14.5; /* istochnik derjit tolko do 1200 A */
 if(m2 < -14.1) m2 = -14.1; /* maksimalniy tok CP-94 - 640 A*/
 if(m3 > 18.)   m3 = 18.;
 
  /* -------------------- kalibrovki ----------- */
  
   q1 = q1*5.0/(3500.*0.01);        /*pomenyali shunt s 7-im elementom  1600->3500*/
   q2 = q2*5.0/(1600.*0.01);
   q3 = q3*5.0/(1600.*0.01);
   q4 = q4*5.0/(1600.*0.01);
 
   m1 = m1*35.*1.5955/(13.635*16.);
   m3 = m3 *1.5955*2.1875/13.635;   /*pomenyali shunt s 1-im elementom  3500->1600*/
 
   q1 *= -1.;  q3 *= -1.; m2 *= -1.;
 
   vm2 =  0.0119342 + 0.102905*m2 - 0.00680999*m2*m2 + 0.00134248*m2*m2*m2 - 0.000115031*m2*m2*m2*m2 +  3.68299e-06*m2*m2*m2*m2*m2;
   vm2 = -5.94754e-03 + vm2*0.990426; 

     break; // 4

//---------------------------------------------------------------------------------------

   case 5: // table40_19.10.11_newM2.c run 2011-II 40 degree
     q1 = -2.*moment;
     q2 = 1.44*moment;
     q3 = -1.*moment;
     q4 = 0.45*moment;
     
     m1 = 3.457*moment;
     m2 =0.0882209 - 4.19518*moment + 0.254558*moment*moment - 0.0530077*moment*moment*moment;
     m3 =-0.247994 + 2.37158*moment - 0.0931518*moment*moment + 0.0109945*moment*moment*moment;
 
     if(q1 < -13.3) q1 = -13.3;
     if(q2 > 13.3)  q2 = 13.3;
     if(q3 < -13.3) q3 = -13.3;
     if(q4 > 13.3)  q4 = 13.3;
 
     if(m1 > 14.65)  m1 = 14.65; /* istochnik derjit tol'ko do 1200 A */
     if(m2 < -17.04) m2 = -17.04; /* maksimalniy tok CP-94 - 640 A*/
     if(m3 > 18.)    m3 = 18.;
 
     /* -------------------- kalibrovki ----------- */
  
     q1 = q1*5.0/(3500.*0.01);        /*pomenyali shunt s 7-im elementom  1600->3500*/
     q2 = q2*5.0/(1600.*0.01);
     q3 = q3*5.0/(1600.*0.01);
     q4 = q4*5.0/(1600.*0.01);
 
     m1 = m1*35.*1.5955/(13.635*16.);
     m3 = m3 *1.5955*2.1875/13.635;   /*pomenyali shunt s 1-im elementom  3500->1600*/
 
     q1 *= -1.;  q3 *= -1.; m2 *= -1.;
 
     vm2 =  0.0119342 + 0.102905*m2 - 0.00680999*m2*m2 + 0.00134248*m2*m2*m2 - 0.000115031*m2*m2*m2*m2 +  3.68299e-06*m2*m2*m2*m2*m2;
     vm2 = -5.94754e-03 + vm2*0.990426; 
   

     break; // 5
   default: return 0;
   }

   Shunt[0] = q1;
   Shunt[1] = q2;
   Shunt[2] = m1;
   Shunt[3] = q3;
   Shunt[4] = q4;
   Shunt[5] = vm2;
   Shunt[6] = m3;
   return Shunt;

}

#define GET_MOM_OF_SHUNT(SHUNT) {   \
                                    \
  M1 = fabs((double)shunt[SHUNT]);         \
 p_low = P_MIN;                     \
 p_high= P_MAX;                     \
 p_curr = (p_high + p_low)/2.;      \
 do {                               \
   res = p2shunt(p_curr, Version);  \
   if(res[SHUNT] > M1){                 \
     p_high = p_curr;               \
   } else {                         \
     p_low = p_curr;                \
   }                                \
   p_curr = (p_high + p_low)/2.;    \
   delta = p_high - p_low;          \
 } while (delta > P_TOLERANCE);     \
                                    \
}


float shunt2p( float *shunt, int Version)
{
  /*        M. Ukhanov 17-Nov-2010
    It converts shunt voltages to the momentum.
    Algorithm: take voltage of M1  and in succesive
               approximation guess the momentum it
	       corresponds to (whithin P_TOLERANCE).
	       Look at the other magnets. If some of
	       them is V_TOLERANCE away from the guessed 
	       momentum then return zero. 
	       If all of them appears to be intact then return
	       the guessed momentum.
   */

const float P_TOLERANCE = 0.001;  /* 1 MeV/c */
const float V_TOLERANCE = 0.3;   /* 100 mV   */
const float P_MAX = 7;
const float P_MIN = 0.5;

 float M1,p_low,p_high,p_curr,delta,*res,p1,p2,p3;
 int i;

 GET_MOM_OF_SHUNT(2);
 p1 = p_curr;
 GET_MOM_OF_SHUNT(5);
 p2 = p_curr;
 GET_MOM_OF_SHUNT(6);
 p3 = p_curr;

 p_curr = (p1+p2+p3)/3.;
 /* Now check setting of other elements 
 for(i=0; i<7; i++) {
   delta = fabs((double)(shunt[i] - res[i]));
   if(delta > V_TOLERANCE) p_curr = 0;
 }
 */
 return p_curr;
}

float *AllShunts2Mom(float *shunt, int Version)
{
  /*  Returns pointer to local static array
   *  filled with momentum converted from shunt 
   *  setting.
   */

const float P_TOLERANCE = 0.001;  /* 1 MeV/c */
const float P_MAX = 7;
const float P_MIN = 0.5;

 float M1,p_low,p_high,p_curr,delta,*res,p1,p2,p3;
  static float Mom[7];
  int i;

  for( i= 0; i<7; i++) {
    GET_MOM_OF_SHUNT(i);
    Mom[i] = p_curr;
  }

  return Mom;
}

#if 0
int main(int argc, char **argv)
{
  int i;
  float shunt[7],p;

  for(i=0; i<7; i++) shunt[i] = atof(argv[i+1]);
  p = shunt2p(shunt);
  printf("Mom=%f for shunt= ",p);
  for(i=0; i<7; i++) printf(" %f",shunt[i]);
  printf("\n delta V= ");
  for(i=0; i<7; i++) printf(" %f",shunt[i]-p2shunt(p)[i]);
  printf("\n");
}

#endif
