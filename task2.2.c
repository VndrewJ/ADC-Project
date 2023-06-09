#include <avr/interrupt.h>
#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>

//Macros for robustness
#define RINGBUFFER_SIZE 1000

volatile uint8_t ringBuffer[RINGBUFFER_SIZE];
int i=0;
int counter = 0;

//timer interrupt
ISR(TIMER1_COMPA_vect){

    counter++;

    /*
    //Function used to validate 200SA/s
    if(counter == 200){
      PORTB ^= (1<<PIN4);
      counter = 0;
    }
    */

    ADCSRA |= (1<<ADSC);                                            //start conversion
}

//ADC completion interrupt
ISR(ADC_vect){
    ringBuffer[i%RINGBUFFER_SIZE] = ADCH;          //add into ring buffer
    Serial.println((String)"At index " +i%RINGBUFFER_SIZE+ " the voltage is " +ringBuffer[i%RINGBUFFER_SIZE]);    //print value at i in ring buffer
    i++;
}

int main(void){
  	Serial.begin(9600);
    cli(); 

    //ADC setup                                                       
    ADMUX |= (1<<ADLAR) | (1<<REFS0);                               //left shift for 8 bit results and turned on reference voltage of 5V

    //enable ADC and interrupts, then set prescaler to 128
    ADCSRA |= (1<<ADEN) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);  

    //timer setup
    TCCR1B |= (1<<WGM12) | (1<<CS11) | (1<<CS10);                   //enable CTC mode and set prescaler to 64
    OCR1A = 1247;                                                   //set compare value to 5ms
    TIMSK1 |= (1 << OCIE1A);                                        // Enable CTC interrupt for OCR1A compare match

    DDRB |= (1<<PIN4);                                              //Setting pin 4 to output for sampling rate demonstration

    sei();

    //superloop
    while(1){
      asm("nop");
    }
}