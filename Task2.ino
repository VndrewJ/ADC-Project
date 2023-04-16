/*Pseudocode

5ms OCR1A calcualtions
16Mhz->6.25x10^-5ms
using 1024 prescaler -> 0.064ms ->78.125 ticks
using 256 prescaler -> 0.016ms -> 312.5 ticks
using 64 prescaler -> 4x10^-3ms -> 1250 ticks (most ideal)

*/

//james and vlad have 11 semicolons, shorten if possible

#include <avr/interrupt.h>
#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>

//Macros for robustness (add later)

volatile uint8_t ringBuffer[1000];
int i=0;

//timer interrupt
ISR(TIMER1_COMPA_vect){
    ADCSRA |= (1<<ADSC);                                            //start conversion
}

//ADC completion interrupt
ISR(ADC_vect){
    ringBuffer[i%1000] = ADCH;          //add into ring buffer
  	Serial.println((String)"At index " +i%1000+ " the voltage is " +ringBuffer[i%1000]);    //print value at i in ring buffer
    i++;
}

int main(void){
  	Serial.begin(9600);
    cli(); 

    //ADC setup                                                       
    ADMUX |= (1<<ADLAR) | (1<<REFS0);                               //left shift for 8 bit results and turned on reference voltage of 5V
    DDRB |= (1<<PIN5);                                              //Set Port B pin 5 to output

    //enable ADC and interrupts, then set prescaler to 128
    ADCSRA |= (1<<ADEN) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);  

    //timer setup
    TCCR1B |= (1<<WGM12) | (1<<CS11) | (1<<CS10);                   //enable CTC mode and set prescaler to 64
    OCR1A = 1250;                                                   //set compare value to 5ms
    TIMSK1 |= (1 << OCIE1A);                                        // Enable CTC interrupt for OCR1A compare match

    sei();

    //superloop
    while(1){
      asm("nop");
    }
}