#define F_CPU 8000000 
//#define F_CPU 1200000 
// clock 1.2MHz, internal oscillator
 
 #include <avr/io.h>
 #include <util/delay.h>
 #include <avr/interrupt.h>
 

#define sbi(a, b) (a) |= (1 << (b))
#define cbi(a, b) (a) &= ~(1 << (b))

 
 int main(void) {
 volatile char last_state;
 volatile char sisend;
 
 char errors=0;
 char ch_switch1;
 char ch_switch2;

 //sbi(PINB,0);
PORTB |= 0b00011001;
 //sei(); // int - Global enable interrupts
 
 DDRB |= ((1 << PB1)|(1 << PB2));
 
 TCCR0B  |= (1 << CS02);


errors=0;
TCNT0 = 0x00;
 

#define CHANNEL_OK  cbi(PORTB,1);cbi(PORTB,2);
#define CHANNEL_NOK  sbi(PORTB,1);sbi(PORTB,2);
sbi(PORTB,1);
sbi(PORTB,2);
/*
00 - no signal
01 - kanal 1,3
10 - kanal 2,4

*/


while (0) {

if(TCNT0%2){
cbi(PORTB,1);cbi(PORTB,2);
}else{
sbi(PORTB,1);sbi(PORTB,2);
}
}
while (1) {

    ch_switch1 = bit_is_clear(PINB,4);
    ch_switch2 = bit_is_clear(PINB,3);
    sisend = bit_is_set(PINB,0);
    if (!sisend){
        TCNT0 = 0x00;
        while(!sisend){
            sisend = bit_is_set(PINB,0);
            if(TCNT0>=75) break;
        }
            if ( TCNT0 < 37 ){
                //kanal 1
               
                if( ch_switch2==1 && ch_switch1 == 1){
                CHANNEL_OK
                } else {
                    CHANNEL_NOK
                }
                errors=0;
                //siia sleep
            }else if(TCNT0 < 50 ) {
                //kanal 2
                if( ch_switch2==1 && ch_switch1 == 0){
                CHANNEL_OK
                }else {
                    CHANNEL_NOK
                }
                errors=0;
            }else if(TCNT0 < 58 ) {
                //kanal 3
                if( ch_switch2==0 && ch_switch1 == 1){
                CHANNEL_OK
                }else {
                    CHANNEL_NOK
                }
                
                errors=0;
            }else if(TCNT0 < 75 ){
                //kanal 4
                if( ch_switch2==0 && ch_switch1 == 0){
                CHANNEL_OK
                }else {
                    CHANNEL_NOK
                }
                errors=0;
            } else {
                CHANNEL_NOK
            } 
                 
        
    } else if (TCNT0==255) {
        if((errors++)>50) {
            CHANNEL_NOK
            errors=0;
        }
    }
    
}


} 




 

