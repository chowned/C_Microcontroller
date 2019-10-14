#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */


#include <hidef.h> /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */
// < Put here your constants, e.g. define here N1 and N0 >
#define N1 6250      // PER OTTENERE 2KHZ E DC 50%
#define N2 6250
#define INT1_INIT_VAL 500
void TIM_init()
{
TSCR1_TEN = 1; // enable the counter
TSCR2_PR = 0; // set the prescaler
}
/* DO NOT MODIFY THIS FUNCTION !! */
void TIM_interrupt_init()
{
TSCR1_TFFCA = 1; // set fast flag clear: updating TC1 resets TFLG1_C1F
TIOS_IOS1 = 1; // set channel 1 as output compare
TC1 = TCNT + INT1_INIT_VAL; // set channel 1 threshold
TIE_C1I = 1; // enable interrupt generation on channel 1
}

void main(void)
{
int x=0,y=0, delta, thr;
    
    CPMUPROT_PROT = 0; //disable protection of the configuration register
    
    CPMUOSC_OSCE = 0;// set internal oscillator
    
    CPMUCLKS_PLLSEL = 1; // select the pll as clock source
    
    CPMUSYNR_SYNDIV = 24;
    CPMUPOSTDIV_POSTDIV = 3;

    DDRM=0x02;
    PTM=0x00;
// wait PLL lock
while (!CPMUFLG_LOCK);


TIM_init();
TIM_interrupt_init();
EnableInterrupts;
/* never ending loop */

/* loop forever */
     PTM_PTM1 = 1;
    CPMUPOSTDIV_POSTDIV = 1;
      TSCR2_PR = 4; // set the prescaler
      x=200;
      y=5;
      delta = x - y ;
      
      for(;;) {
      thr = TCNT + (delta/2);
      if(thr < (delta/2)){
         while(TCNT > thr){
         } 
      }
      while(TCNT < thr){
          }
        PTM_PTM1 ^= 1; //toggle port M
       
       
      }
 _FEED_COP(); /* feeds the dog */
}
#pragma CODE_SEG NON_BANKED
interrupt 9 void TIM_int1(void)
{
static int x = INT1_INIT_VAL;
x = (x >> 2) | (((x & 1)^(x & 2)) << 11);
TC1 += x;
}
#pragma CODE_SEG DEFAULT


   
