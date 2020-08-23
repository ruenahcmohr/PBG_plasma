/*

   This code was written today to do the things.
     By me.


should output 6.4kHz
    
                       +---\/---+
        (/RESET)  PC6  |1     28|  PC5  (ADC5 / SCL)
           (RXD)  PD0  |2     27|  PC4  (ADC4 / SDA)
           (TXD)  PD1  |3     26|  PC3  (ADC3)
          (INT0)  PD2  |4     25|  PC2  (ADC2)
          (INT1)  PD3  |5     24|  PC1  (ADC1)
      (XCK / T0)  PD4  |6     23|  PC0  (ADC0)
                  VCC  |7     22|  GND
                  GND  |8     21|  AREF
8(XTAL1 / TOSC1)  PB6  |9     20|  AVCC
7(XTAL2 / TOSC2)  PB7  |10    19|  PB5  (SCK)          6
            (T1)  PD5  |11    18|  PB4  (MISO)         5 
          (AIN0)  PD6  |12    17|  PB3  (MOSI / OC2)   4
          (AIN1)  PD7  |13    16|  PB2  (SS / OC1B)    3
     1    (ICP1)  PB0  |14    15|  PB1  (OC1A)         2
                       +--------+

  PB0  phase 5 cathode
  PB1  phase 4 cathode
  PB2  phase 3 cathode
  PB3  phase 2 cathode
  PB4  phase 1 cathode
  PB5  side anode
  PB6  other side anode
  PB7  reset   cathode     
		       
		       
*/

#include <avr/io.h>
#include "avrcommon.h"
#include <avr/interrupt.h>
#include <stdint.h>


void InitTimer() ;

volatile uint8_t L1, L2;
volatile uint8_t scanCount;

/****************************| MANE CODY... |***********************************/
 
int main (void)  {

  DDRB = (OUTPUT << PB0)|(OUTPUT << PB1)|(OUTPUT << PB2)|(OUTPUT << PB3)|(OUTPUT << PB4)|(OUTPUT << PB5)|(OUTPUT << PB6)|(OUTPUT << PB7);
  DDRC = (INPUT << PC0)|(INPUT << PC1)|(INPUT << PC2)|(INPUT << PC3)|(INPUT << PC4)|(INPUT << PC5)|(INPUT << PC6);
  DDRD = (OUTPUT << PD0)|(INPUT << PD1)|(INPUT << PD2)|(INPUT << PD3)|(INPUT << PD4)|(INPUT << PD5)|(INPUT << PD6)|(INPUT << PD7);        

  InitTimer();                         
  sei();                                          //set the Global Interrupt Enable Bit

  L1 = 50;
  L2 = 25;

  while(1) {
  
    NOP();
    
    if (scanCount > 200) scanCount = 0; // animate 1 bar
    L1 = scanCount;

  }
  return 0;
}
    
//------------------------| FUNCTIONS |------------------------

void InitTimer() {

  TCCR1B |= ((1<<CS10)); // fast pwm, CLK/1
  TIMSK = (1<<TOIE1);

}

// we have 623 cycles ok!?
ISR(TIMER1_OVF_vect) {
 static uint8_t line   = 0;
 static uint8_t phases = 0x00;
 
 uint8_t m;
 
 TCNT1 = 64912;   // 78us none of the systems for setting TOP are working tonight
 
 line ++;
 m = 0;
 if (line < L1)  m |= 0x20;
 if (line < L2)  m |= 0x40;
  
 if ((line == 202)||(line == 203)) { // I dont know why
   // sit idle while reset
 }else if (line == 204) { // re-fire
  line   = 0;
  phases = 0x00;
  PORTB  = 0x80 | 0x20 | 0x40;
  scanCount++;
 }else {
   phases >>= 1;
   if (!phases) phases = 0x10;
      
   PORTB = phases | m ;
 }
   
} 





