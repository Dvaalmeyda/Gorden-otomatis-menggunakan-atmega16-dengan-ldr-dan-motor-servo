/*
 * Tugas Akhir.cpp
 *
 * Created: 23/11/2022 00:04:54
 * Author : HH
 */ 

#define F_CPU 16000000L
#include <avr/io.h> //library io
#include <util/delay.h>
#include <stdlib.h>

void ADC_INIT(){
	DDRA = 0x00; //PORTA input
	ADCSRA = 0x87; // ADC = enable, prescaler 128
	ADMUX = 0x40; //Vref = AVCC external capasitor, channel ADC0
}

int read_adc(char channel){
	int lower,higher;
	
	ADMUX = ADMUX|(channel & 0x0f);
	ADCSRA |=  (1<<ADSC); //mulai konversi
	while(!(ADCSRA&(1<<ADIF))==0);//konversi selesai flag=1 dan dicek
	_delay_us(10);
	lower = (int)ADCL; //baca bit lower
	higher = (int)ADCH*256; //baca bit higher
	higher = higher+lower;
	return higher;
}

void PWM_servo(){
	TCCR1A=(1<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (1<<WGM11) | (0<<WGM10);
	TCCR1B=(0<<ICNC1) | (0<<ICES1) | (1<<WGM13) | (1<<WGM12) | (0<<CS12) | (1<<CS11) | (0<<CS10);
	TCNT1=0x00;
	ICR1H=0x9E;
	ICR1L=0x3F;
	OCR1A=0x00;
}

int main(void)
{

	ADC_INIT();
	PWM_servo();
	DDRD= (0<<DDD7) | (0<<DDD6) | (1<<DDD5) | (0<<DDD4) | (0<<DDD3) | (0<<DDD2) | (0<<DDD1) | (0<<DDD0);

	while (1)
	{

		int adc_result0 = read_adc(0);


		if (adc_result0 >= 1024)
		{
			_delay_ms(300);
			OCR1A=(1000+(5.6*150))-1;
			
		}
		else
		{
			_delay_ms(300);
			OCR1A=(1000+(5.6*500))-1;
			
		}

	}
}

