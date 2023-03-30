/*Pseudocode
1. Start ADC conversion
2. Interrupt when ADC is done converting
3. Get voltage value and store in array (array must hold >= 1000 samples)
4. delay for 5ms 
*/

#include <avr/interrupt.h>
#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>

//Macros for robustness (add later)
#define CLOCK_PERIOD 0.064      //Clock period after prescaler

uint8_t volatile factor = 2;    //default factor

ISR(ADC_vect){
    uint16_t volatile voltage = ADC;
}

int main(void){
    //setup
    cli();                                                          //dsiable interrupts
    ADMUX |= (1<<ADLAR);                                            //left shift for 8 bit results   
    DDRB |= (1<<PIN5);                                              //Set Port B pin 5 to output
    //enable ADC and interrupts, then set prescaler to 128
    ADCSRA |= (1<<ADEN) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0) ;  

    sei();

    //superloop
    while(1){

    }
    
}