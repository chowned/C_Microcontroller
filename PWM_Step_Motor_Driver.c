#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

 void TIM_init()
{
TSCR1_TEN = 1; // enable the counter
TSCR2_PR = 7; // set the prescaler
}


void main(void) {
  /* put your own code here */
  

  /*potentiometer*/
  ATDCTL1_SRES = 0 ;
  ATDCTL3_DJM = 1;
  ATDCTL5_SCAN = 1;
  
  
  PWMCLKAB_PCLKAB0 = 0; //Clock A channel 0
  PWMCLK_PCLK0 = 0;
  PWMCLKAB_PCLKAB1 = 0; //Clock A channel 1
  PWMCLK_PCLK1 = 0;
  PWMCLKAB_PCLKAB2 = 2; //Clock A channel 2
  PWMCLK_PCLK2 = 0;
  PWMCLKAB_PCLKAB3 = 0; //Clock A channel 3
  PWMCLK_PCLK3 = 0;
  
  PWMPOL_PPOL0 = 1; //Polarity channel 0
  PWMPOL_PPOL1 = 0; //Polarity channel 1
  PWMPOL_PPOL2 = 0; //Polarity channel 2
  PWMPOL_PPOL3 = 1; //Polarity channel 3
  
  PWMPRCLK_PCKA = 3; // Set prescaler 
  
  PWMCAE_CAE0 = 1; //Alignment channel 0 center
  PWMCAE_CAE1 = 1; //Alignment channel 1 center
  PWMCAE_CAE2 = 0; //Alignment channel 2 left 
  PWMCAE_CAE3 = 0; //Alignment channel 3 left
  
  PWMPER0 = 50;   //Set period 0
  PWMPER1 = 50;   //Set period 1
  PWMPER2 = 100;  //Set period 2
  PWMPER3 = 100;  //Set period 3
  //DC @ 50%               
  PWMDTY0 = 50;
  PWMDTY1 = 50;
  PWMDTY2 = 50;
  PWMDTY3 = 50;
  
  PWMCNT0 = 0;
  PWMCNT1 = 0;
  PWMCNT2 = 0;
  PWMCNT3 = 0;
  //PWM ENABLE
  PWME_PWME0 = 1;
  PWME_PWME1 = 1;
  PWME_PWME2 = 1;
  PWME_PWME3 = 1;
  //A/D
  ATDCTL01_SRES = 0 ; // Set res to 8 bit
  ATDCTL2_ASCIE = 1; //Enable interrupt gen every conversion 
  ATDCTL3_DJM = 1; //Right justified data in the result registers
  ATDCTL5_SCAN = 1; //continous scan
  
  TIM_init();
	EnableInterrupts;


  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}

#pragma CODE_SEG NON_BANKED

interrupt 22 void WaveControl(void) {
  
   int tmp;
   if(ATDSTAT2L_CCF0 == 1){
    tmp = ATDDR0;
    ATDSTAT2L_CCF0 = 1; // CLEAR CCF0
    PWMPER0 = tmp;
    PWMPER1 = tmp;
    PWMPER2 = tmp;
    PWMPER3 = tmp;
    
    PWMDTY0 = tmp;
    PWMDTY1 = tmp;
    PWMDTY2 = tmp;
    PWMDTY3 = tmp;
   }
}
#pragma CODE_SEG DEFAULT
