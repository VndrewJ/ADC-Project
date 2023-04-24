#include <avr/interrupt.h>
#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>

//Macros for robustness (add later)
#define RING_BUFFER_SIZE 1500

//state variable
bool state=0;

//ring buffer variables
volatile uint8_t ringBuffer[RING_BUFFER_SIZE];
volatile int i=0;
int index = 0;

//light variables
uint32_t light_counter=0;

//button interrupt
ISR(INT0_vect){
    state = !state;                                                     //toggle state                                                   
    ADCSRA ^= (1<<ADIE);                                                //toggle adc interrupt                           
    ringBuffer[RING_BUFFER_SIZE] = {0};                                 //reset ring buffer
}

//ADC Timer interrupt
ISR(TIMER1_COMPA_vect){
    light_counter++;                                
    
    //flash light at 1hz in state 1, at 2hz in state 2
    if(light_counter % (100/((int)state+1)) == 0){
        PORTB ^= (1<<PIN5);
        light_counter = 0;                                  //reset light counter
    }

    ADCSRA |= (1<<ADSC);
}

//ADC completion interrupt
ISR(ADC_vect){
    ringBuffer[i%RING_BUFFER_SIZE] = ADCH;                  //add into ring buffer
    i++;
}

int main(void){
  	Serial.begin(9600);
    cli(); 

    //ADC setup                                                       
    ADMUX |= (1<<ADLAR) | (1<<REFS0);                               //left shift for 8 bit results and turned on reference voltage of 5V
    DDRB |= (1<<PIN5);                                              //Set Port B pin 5 to output

    //enable ADC and interrupts, then set prescaler to 128
    ADCSRA |= (1<<ADEN) | (1<<ADIE) | (1<<ADPS0);  

    //ADC timer setup
    TCCR1B |= (1<<WGM12) | (1<<CS11) | (1<<CS10);                   //enable CTC mode and set prescaler to 64
    OCR1A = 1250;                                                   //set compare value to 5ms
    TIMSK1 |= (1 << OCIE1A);                                        // Enable CTC interrupt for OCR1A compare match

    //light and button setup
    DDRB |= (1<<PIN5);                                              //set pin 5 to output (light)
    DDRD &= ~(1<<PIN2);                                             //set pin 2 portb D to input
    EIMSK |= (1 << INT0);                                           //enable interrupts for INT0 (pin 2 portd)
    EICRA |= (1 << ISC00) | (1 << ISC01);                           //set interrupt to occur on rising edge  

    sei();

    //superloop
    while(1){
      switch(state){
        case 0:                             //does nothing in state 1, interrupts take care of everything
          asm("nop");
          break;

        case 1:

            if(i>=RING_BUFFER_SIZE){                    //checks if ring buffer is full
                index=RING_BUFFER_SIZE;
            } else {
                index = i;
            }
            //print the ring buffer till wherever it stopped reading at i
            for(int j = 0; j < index; j++){
                Serial.println((String)"The voltage at " +j+ " is " + ringBuffer[j]);
            }
            i = 0;                              //reset index
            while(state == 1){                  //finish printing and do nothing until the button is pressed again
              //Serial.println("capped");              
              asm("nop");
            }
        }
    }
}