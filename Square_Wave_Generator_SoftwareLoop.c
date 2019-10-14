#include <hidef.h> /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */
// < Put here your constants, e.g. define here N1 and N0 >
#define N1 6250      // PER OTTENERE 2KHZ E DC 50%
#define N2 6250
#define INT1_INIT_VAL 500
void TIM_init()
{
TSCR1_TEN = 1; // enable the counter
TSCR2_PR = 4; // set the prescaler
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
int up_down=0, i;
DDRM=0x02;
PTM=0x00;
CPMUPOSTDIV_POSTDIV = 1; // set POSTDIV
// wait PLL lock
while (!CPMUFLG_LOCK);


TIM_init();
TIM_interrupt_init();
EnableInterrupts;
/* never ending loop */
for(;;)
{
  switch (up_down){
    case 0 :
      PTM_PTM1 = 0;
      for(i=0;i<N1;i++){
        up_down =1;
      }
      break;
    case 1 :
       PTM_PTM1 = 1;
      for(i=0;i<N2;i++){
        up_down =0;
      }
  }
}
_FEED_COP(); /* feeds the dog */
} /* loop forever */

#pragma CODE_SEG NON_BANKED
interrupt 9 void TIM_int1(void)
{
static int x = INT1_INIT_VAL;
x = (x >> 2) | (((x & 1)^(x & 2)) << 11);
TC1 += x;
}
#pragma CODE_SEG DEFAULT