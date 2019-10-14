#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#define thr_1 3125
#define thr_2 1562
#define thr_4 781


  void TIM_init()
{
TSCR1_TEN = 1; // enable the counter
TSCR2_PR = 1; // set the prescaler 
}


void main(void) {
 //PWM configure
  PWMCLK_PCLK0 = 0;  // set clock source
  PWMCLKAB_PCLKAB0 = 0;  //  
  PWMPRCLK_PCKA = 3;    //set prescaler for the clock source
  PWMPOL_PPOL0 = 1;    //set output polarity
  PWMCAE_CAE0 = 0;    // set alignment
  PWMPER0 = 255;   //set period
  PWMDTY0 = 127;   //set D.C.
  PWMCNT0 = 0;         //
  PWME_PWME0 = 1;  //Enable Channel
 //LED Configure
  DDRT=0xF0;
  PTT=0x00;
 //Edge Detection Setting
  TCTL4_EDG0B = 1;
  TCTL4_EDG0A = 1;
 // enable canale 0 per interrupt
  TIOS_IOS0 = 1;
  TIE_C0I = 1; //Timer Interrupt Enable Register
 //TCR
  TIM_init(); 
	EnableInterrupts;


  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}

  interrupt 8 void Wave_Gen(void)
{
  //Bisogna fare i calcoli per creare l'onda quadra che rispetta le specifiche
  int oldv = 0;
  int newv, v, T, T1, DC ; //necessitan inizializzazione?
  if (PTT_PTT0 == 1){
     //RISING EDGE
     newv = TC0 ; 
      if(oldv > newv){
        T = 0xFFFF - oldv + newv ; 
      } else {
          T = newv - oldv;
        }
       oldv = newv ;
  }  else {
     //FALLING EDGE
     v = TC0 ; 
      if(oldv > v){
        T1 = 0xFFFF - oldv +v;
      } else {
          T1 = v- oldv;
        }
    }
   PWMDTY0 = T/T1;
   PWMPER0 = T;    //Assegno periodo e dc alla wave di pwm
   TFLG1_C0F = 1;
   //LED managing
   if(T < thr_1){PTT = 0x3F;}   
   if(thr_1 < T < thr_2){PTT = 0x2F;}
   if(thr_2 < T < thr_4){PTT = 0x1F;}
   if(T > thr_4){PTT = 0x0F;}
   //DC managing
   DC = T1/T;
    if(DC < 0.25){PTT = 0x0F;}
   if(0.25 < DC < 0.5 ){PTT = 0x4F;}
   if(0.5 < DC < 0.75){PTT = 0x8F;}
   if(DC > thr_4){PTT = 0xFF;}
}