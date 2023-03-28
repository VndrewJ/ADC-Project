#include <avr/interrupt.h>
#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>

void main(){
    //ADC Setup
    ADCRSA |= (1<<ADEN) | (1<<ADSP2) | (1<<ADSP1) | (1<<ADSP0);     //enable ADC then set prescaler to 128

    //pin setup
    DDRC &= ~(1<<PIN0);     //set pin A0 to read
    DDRB |= (1<<PIN1);      //Set Port B pin 1 to output

    //superloop
    while(1){
        //start conversion
        ADCRSA |= (1<<ADSC);
        while(ADCRSA & (1<<ADSC)){      //while converting, do nothing
            asm("nop");
        }
        uint16_t voltage;               //initialise voltage
        voltage = ADC;                  //get values

        if(voltage >= 2){               //if voltage is above 2V turn on and off a couple times
            PORTB |= (1<<PIN0);       
            _delay_ms(50);
            PORTB &= ~(1<<PIN0);
            _delay_ms(50);  
            PORTB |= (1<<PIN0);       
            _delay_ms(50);
            PORTB &= ~(1<<PIN0);
            _delay_ms(50);  
        }else{                          //else just do it once
            PORTB |= (1<<PIN0);       
            _delay_ms(50);
            PORTB &= ~(1<<PIN0);
            _delay_ms(50);  
        }
    }
}