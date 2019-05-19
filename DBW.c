#define F_CPU 16000000
#define MIN_PWM 4     // in ms (change this according to your servo)
#define MAX_PWM 1    // in ms (change this according to your servo)
#define PWM_FREQUENCY 100  // in Hz
#define PRESCALAR 256
#define UNIT_PULSE ((F_CPU/(PRESCALAR*PWM_FREQUENCY) - 1)/20)  // timer counts for 1ms
#include<avr/io.h>
#include<util/delay.h>
uint16_t adc_input=0;
// Initialize PWM channels - 16bit Timer
// Fast PWM, Non-Inverting, N = 256;
void init_pwm(){
	
	TCCR1B |= (1 << WGM12) ; //| (1 << WGM13)
    TCCR1B |= (1 << CS12);
    TCCR1A |= (1 << WGM11);
    TCCR1A |= (1 << COM1A1); //non-inverting
ICR1 = F_CPU /(PRESCALAR * PWM_FREQUENCY) -1;
}
void InitADC()
{
ADMUX=(1<<REFS0);             // For Aref=AVcc;
ADCSRA=(1<<ADEN)|(7<<ADPS0);
}

uint16_t ReadADC(uint8_t ch)
{
	//Select ADC Channel ch must be 0-7
	ADMUX&=0xf8;
	ch=ch&0b00000111;
	ADMUX|=ch;
	ADCSRA|=(1<<ADSC);
	while(!(ADCSRA & (1<<ADIF)));
	ADCSRA|=(1<<ADIF);
	return(ADC);
}

int main(){
 // Serial.begin(9600);
  InitADC();	 // Set Angle here
   DDRB |= (1 <<PB1); // Set PB.1 as output
	 init_pwm();
	 while(1){
           adc_input= ReadADC(4);
           Serial.println(adc_input);
           adc_input=adc_input/5.68;
     OCR1A = UNIT_PULSE * MIN_PWM + (adc_input * UNIT_PULSE*(MAX_PWM - MIN_PWM))/180;
     Serial.println(UNIT_PULSE);
     Serial.println(OCR1A);  
       _delay_ms(500);
 }
 return 0;
}

