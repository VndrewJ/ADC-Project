#include <avr/interrupt.h>
#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>

//Macros for robustness
#define VREF 2
#define VOLTAGE 5
#define THRESHOLD ((VREF*1024)/VOLTAGE)
#define PULSE_TH 4

int main(void){
    //ADC Setup
    ADMUX |= (1<<REFS0);                                            //turn on ref voltage
    ADCSRA |= (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);     //enable ADC then set prescaler to 128

    //pin setup
    DDRB |= (1<<PIN5);      //Set Port B pin 5 to output
  
    //superloop
    while(1){
        //start conversion
        ADCSRA |= (1<<ADSC);
        while(ADCSRA & (1<<ADSC)){      //while converting, do nothing
            asm("nop");
        }
     	
        //check if voltage read is > 2V and delay 
        ADC >= THRESHOLD ? pulse_n(PULSE_TH, 1000/PULSE_TH) : pulse_n(PULSE_TH/2, 1000/(PULSE_TH/2));
    }
}

//Recursive function for flashing onboard LED, toggles state N times
void pulse_n(uint8_t n, uint16_t period){ 
    
    if(n > 0){
        PORTB ^= (1<<PIN5);       
        _delay_ms(period); 
        pulse_n((n-1), period);
    }
}