// 5 interrupt,ch3 of TCNT register to generate waveform on PTM1 with variable frequency using potentiometer from 87Hz to 1kHz

#include <hidef.h> /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */

#define INT1_INIT_VAL 500 

#define d_mezzi0    4883  
#define d_mezzi1    48828 
#define d_mezzi2    97656 
#define del 1000
void TIM_init()
{
TSCR1_TEN = 1; // enable the counter
TSCR2_PR = 7; // set the prescaler
}

void delay (int num);
int d_mezzi3;
void main(void)
{

CPMUPOSTDIV_POSTDIV = 1; // set POSTDIV

// wait PLL lock

while (!CPMUFLG_LOCK);

//canale 0
//fqclk(fq timer) = fbus/2^(PR)
//con PR= 7 => 12.5MHz/128 => Tclk=10.24us => 100ms/10.24 = 9765,625 
//delta/2= 4882.8125 => thr = TCNT+4883 

// canale 1
//fqclk(fq timer) = fbus/2^(PR)
//con PR= 7 => 12.5MHz/128 => Tclk=10.24us => 1s/10.24 = 97656,25  
//delta/2= 48828,125 => thr = TCNT+48828 

//canale 2
//fqclk(fq timer) = fbus/2^(PR)
//con PR= 7 => 12.5MHz/128 => Tclk=10.24us => 2s/10.24 = 195312,5  
//delta/2=  97656,25 => thr = TCNT+97656 


TSCR1_TFFCA = 1; // set fast flag clear: updating TC1 resets TFLG1_C1F

// enable canale 3 per interrupt
TIOS_IOS3 = 1;
TIE_C3I = 1;
// enable canale 2 per interrupt
TIOS_IOS2 = 1;
TIE_C2I = 1;
// enable canale 1 per interrupt
TIOS_IOS1 = 1;
TIE_C1I = 1;
// enable canale 0 per interrupt
TIOS_IOS0 = 1;
TIE_C0I = 1;



/*potentiometer*/
ATDCTL1_SRES = 0 ;
ATDCTL3_DJM = 1;
ATDCTL5_SCAN = 1;

DDRM=0x02;
PTM=0x00;

DDRT=0xF0;
PTT=0x00;

DDR1AD=0x00;
PER1AD=0x80;
PPS1AD=0x80;
ATDDIEN=0x0080;
PIE1AD=0x80;
PIF1AD=0x00;

TIM_init();

EnableInterrupts;

d_mezzi3 = ATDDR0/2 + 49; //inizializzazione delta_mezzi
TC3 = TCNT + d_mezzi3; //--
TC2 = TCNT + d_mezzi2;
TC1 = TCNT + d_mezzi1;
TC0 = TCNT + d_mezzi0;
/* never ending loop */

for(;;)
{
                                         
/* Attendi l' interrut*/

_FEED_COP(); /* feeds the dog */
} /* loop forever */
}

void delay (int num){
  volatile unsigned short p;
    while (num > 0)  {
      p= 100;
      while (p > 0){
        p = p - 1 ;
      }
      num = num -1 ;
    }
}

#pragma CODE_SEG NON_BANKED

interrupt 8 void TIM_int0(void)
{

TC0 = TCNT + d_mezzi0;
PTT_PTT4 ^= 1  ;

}
interrupt 9 void TIM_int1(void)
{

TC1 = TCNT + d_mezzi1;
PTT_PTT5 ^=1  ;

}

interrupt 10 void TIM_int2(void)
{

TC2 = TCNT + d_mezzi2;
 PTT_PTT6 ^= 1 ;

}

interrupt 62 void int_SW(void)/* INTERRUPT CHANNEL 4 */
{
delay(del) ;
PTT_PTT7 ^=1;
PIF1AD = 0x40; //1 NEL BIT 7 
} 

interrupt 11 void TIM_int3(void)
{

TC3 = TCNT + d_mezzi3;

PTM_PTM1 ^= 1;

if(ATDSTAT2_CCF0 == 1){

  d_mezzi3 = ATDDR0/2 + 49; 
       
}
} 


#pragma CODE_SEG DEFAULT
