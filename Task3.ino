/*
Task 3 pseudocode

2 timers
    1 timer for blinking
    1 timer for sampling

Switch case for blinking + sampling

or

switch case in interrupt?

*/

#include <avr/interrupt.h>
#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>

//Macros for robustness (add later)
#define CLOCK_FREQ 16*10^3
#define DEFAULT_STATE_MS 1000
#define PRESCALER 

bool state=0;

//ring buffer variables
volatile uint8_t ringBuffer[1000];
int i=0;

//light variables
int light_counter=0;

//ADC Timer interrupt
ISR(TIMER1_COMPA_vect){
    light_counter++;

    //finite state switch
    switch(state){
        case 0:

            //Light toggles every 1000ms, ADC toggles every 5ms, light needs to occur before ADC start
            if(light_counter == 200){
                PORTB ^= (1<<PIN5);                                         //toggle pin5 light
                light_counter = 0;                                          //reset light counter
            }
            ADCSRA |= (1<<ADSC);                                        //start conversion
            break;
        case 1:
            if(light_counter == 100){                                   //same check but for 500ms
                PORTB ^= (1<<PIN5);
                light_counter = 0;
                break;
            }
    }
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

    //ADC timer setup
    TCCR1B |= (1<<WGM12) | (1<<CS11) | (1<<CS10);                   //enable CTC mode and set prescaler to 64
    OCR1A = 1250;                                                   //set compare value to 5ms
    TIMSK1 |= (1 << OCIE1A);                                        // Enable CTC interrupt for OCR1A compare match

    //light setup
    DDRB |= (1<<PIN5);                                              //set pin 5 to output (light)

    sei();

    //superloop
    while(1){
      asm("nop");
    }
}