#include <avr/interrupt.h>
#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>

void main(){
    //ADC Setup
    ADCRSA |= (1<<ADEN) | (1<<ADSP2) | (1<<ADSP1) | (1<<ADSP0);     //enable ADC then set prescaler to 128

    //pin setup
    DDRC &= ~(1<<PIN0);     //set pin A0 to read

    


    //superloop
    while(1){
        _delay_ms(1000);
        uint16_t voltage;
        voltage = ADC;
        
    }
}