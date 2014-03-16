/*
 * tahometr.cpp
 *
 * Created: 10.03.2014 12:51:45
 *  Author: Vyacheslav
 */ 

#define F_CPU 8000000
#define BLINK_DELAY 90

#include <avr/io.h>
#include <util/delay.h>
#include "avr/interrupt.h"

void blink (uint8_t seg, uint8_t razr);
void initdisp (uint8_t razr1, uint8_t razr2, uint8_t razr3, uint8_t razr4, uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f, uint8_t g, uint8_t h);
void Seg_on (uint8_t num, uint8_t razr);
void Seg_off (uint8_t num, uint8_t razr);
void Disp_out (uint8_t num, uint8_t razr);
void setrazr(uint8_t razr);
void Timer_init(); 
void decod_num ();

volatile int ind_razr1, ind_razr2, ind_razr3, ind_razr4;
uint8_t ind_a, ind_b, ind_c, ind_d, ind_e, ind_f, ind_g, ind_h;
volatile int num_out=2346;
volatile uint8_t num_out_1, num_out_2, num_out_3, num_out_4;
volatile uint8_t registr=0;

int main(void)
{
	initdisp(4,6,7,5,0,1,2,3,4,5,6,7);
	_delay_ms(300);
	decod_num();
	Timer_init();
    while(1)	
    {
		num_out++;
		decod_num ();
//		if (num_out>9999) num_out=0;
		_delay_ms(100);
//		for (num_out=0; num_out<10000; num_out++);
    }
}

void initdisp (uint8_t razr1, uint8_t razr2, uint8_t razr3, uint8_t razr4, uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f, uint8_t g, uint8_t h)
{
	ind_razr1 = razr1;
	ind_razr2 = razr2;
	ind_razr3 = razr3;
	ind_razr4 = razr4;
	ind_a = a;
	ind_b = b;
	ind_c = c;
	ind_d = d;
	ind_e = e;
	ind_f = f;
	ind_g = g;
	ind_h = h;
	
	DDRB = 0xFF;
	DDRD |= (1<<ind_razr1)|(1<<ind_razr2)|(1<<ind_razr3)|(1<<ind_razr4);
	PORTD |= (1<<ind_razr1)|(1<<ind_razr2)|(1<<ind_razr3)|(1<<ind_razr4);
	PORTB = 0x00;
	_delay_ms(300);
	blink(0,0); 
	blink(1,0);
	blink(3,0);
	blink(4,0);		
	blink(6,0);
	blink(7,0);
	blink(5,0);
	blink(4,0);
	blink(0,0);
	blink(1,0);
	blink(3,0);
	blink(5,0);
	blink(7,0);
	blink(6,0);
	blink(0,0);
	blink(1,0);
	blink(2,0);	
}

void blink (uint8_t seg, uint8_t razr)
{
	setrazr(razr);
	Seg_on(seg,0);
	_delay_ms(BLINK_DELAY);
	Seg_off(seg,0);
	
}

void Seg_on (uint8_t seg, uint8_t razr)
{
	setrazr(razr);
	PORTB &= ~(1<<seg);	
}

void Seg_off (uint8_t seg, uint8_t razr)
{
	setrazr(razr);
	PORTB |= (1<<seg);
}

void Disp_out (uint8_t num, uint8_t razr)
{
	PORTD &= ~((1<<ind_razr1)|(1<<ind_razr2)|(1<<ind_razr3)|(1<<ind_razr4));
	switch (num) {
		case 0:
			PORTB |= (1<<2)|(1<<4);			
			PORTB &= ~((1<<0)|(1<<1)|(1<<3)|(1<<5)|(1<<6)|(1<<7));			
			break;
		case 1:
			PORTB |= (1<<0)|(1<<1)|(1<<2)|(1<<4)|(1<<6)|(1<<7);						
			PORTB &= ~((1<<3)|(1<<5));
			break;
		case 2:
			PORTB |= (1<<2)|(1<<3)|(1<<6);							
			PORTB &= ~((1<<0)|(1<<1)|(1<<4)|(1<<5)|(1<<7));
			break;
		case 3:
			PORTB |= (1<<0)|(1<<2)|(1<<6);								
			PORTB &= ~((1<<1)|(1<<3)|(1<<4)|(1<<5)|(1<<7));
			break;
		case 4:
			PORTB |= (1<<0)|(1<<1)|(1<<2)|(1<<7);						
			PORTB &= ~((1<<3)|(1<<4)|(1<<5)|(1<<6));		
			break;
		case 5:
			PORTB |= (1<<0)|(1<<2)|(1<<5);					
			PORTB &= ~((1<<1)|(1<<3)|(1<<4)|(1<<6)|(1<<7));
			break;
		case 6:
			PORTB |= (1<2)|(1<<5);							
			PORTB &= ~((1<<0)|(1<<1)|(1<<3)|(1<<4)|(1<<6)|(1<<7));	
			break;
		case 7:
			PORTB |= (1<<0)|(1<<1)|(1<<2)|(1<<4)|(1<<6);								
			PORTB &= ~((1<<3)|(1<<5)|(1<<7));	
			break;
		case 8:
			PORTB |= (1<<2);								
			PORTB &= ~((1<<0)|(1<<1)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7));
			break;			
		case 9:
			PORTB |= (1<<0)|(1<<2);							
			PORTB &= ~((1<<1)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7));	
			break;			
		default: 
			PORTB |= (1<<1)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7);		
			PORTB &= ~(1<<2);
			break;
	}
	setrazr(razr);
}

void setrazr(uint8_t razr) 
{
	switch (razr) {
		case 1:			
			PORTD &= ~((1<<ind_razr2)|(1<<ind_razr3)|(1<<ind_razr1));
			PORTD |= (1<<ind_razr4);			
			break;
		case 2:
			PORTD &= ~((1<<ind_razr2)|(1<<ind_razr1)|(1<<ind_razr4));
			PORTD |= (1<<ind_razr3);
			break;			
		case 3:
			PORTD &= ~((1<<ind_razr1)|(1<<ind_razr3)|(1<<ind_razr4));
			PORTD |= (1<<ind_razr2);
			break;
		case 4:
			PORTD &= ~((1<<ind_razr2)|(1<<ind_razr3)|(1<<ind_razr4));
			PORTD |= (1<<ind_razr1);
			break;
		default:
			break;
	}	
}

void Timer_init()
{
	TCCR0 |= (1<<CS00)|(1<<CS01); //Задаем несущую частоту 31,25кГц
	SREG |=(1<<7); // Глобальное разрешение прерываний.
	TIMSK |= (1<<TOIE0); //прерывание по переполнению
}

ISR(TIMER0_OVF_vect) //Обработчик прерывания по переполнению счетчика
{
	switch (registr)
	{
		case 0:
			if (num_out_1<11) Disp_out(num_out_1,1);
			registr++;
			break;
		case 1:
			if (num_out_2<11) Disp_out(num_out_2,2);
			registr++;
			break;
		case 2:
			if (num_out_3<11) Disp_out(num_out_3,3);
			registr++;
			break;
		case 3:
			if (num_out_4<11) Disp_out(num_out_4,4);	
			registr=0;
			break;
	}
	
	TCNT0=0; // Периодическая инициализация счётчика.
}

void decod_num() 
{
	num_out_1 = 11;
	num_out_2 = 11;
	num_out_3 = 11;
	num_out_4 = 11;
	if (num_out<1)
	{
		num_out_1 = num_out;
	} else if (num_out<100)
	{
		num_out_1 = num_out%10;
		num_out_2 = num_out/10;
	} else if (num_out<1000)
	{
		num_out_1 = num_out%10;
		num_out_2 = (num_out%100)/10;
		num_out_3 = num_out/100;
	} else if (num_out<10000)
	{
		num_out_1 = num_out%10;
		num_out_2 = (num_out%100)/10;
		num_out_3 = (num_out%1000)/100;
		num_out_4 = num_out/1000;
	} else
	{
		num_out_1 = 8;
		num_out_2 = 8;
		num_out_3 = 8;
		num_out_4 = 8;
	}

}