#include <avr/interrupt.h>
#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>

int main(void){
    //ADC Setup
    ADCSRA |= (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);     //enable ADC then set prescaler to 128

    //pin setup
    DDRC &= ~(1<<PIN0);     //set pin A0 to read
    DDRB |= (1<<PIN5);      //Set Port B pin 5 to output

    //superloop
    while(1){
        //start conversion
        ADCSRA |= (1<<ADSC);
        while(ADCSRA & (1<<ADSC)){      //while converting, do nothing

        }
        uint16_t voltage = ADC;               //initialise voltage and get values

        condition (voltage >= 409) pulse_n(4, 250) : pulse_n(2, 500);
        //No delay here, 1 second delay taken care of by flashes
    }
}

void pulse_n(uint8_t n, uint16_t period){ 
    //Recursive function for flashing onboard LED, toggles state N times
    
    if(n > 0){
        PORTB ^= (1<<PIN5);       
        _delay_ms(period); 
        pulse_n((n-1), period);
    }
}