#include <avr/io.h> 
#define F_CPU 1000000UL 
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>


#define is_high(x,y) (x & _BV(y)) == _BV(y) //(for input) checks if the input is high (Logic 1)

// PD0 out - engine up
// PD1 out - engine down
// PC0 in - ogranicznik dolny (PCINT8)

// *not configured yet
// PC1 in - ogranicznik gorny (PCINT9)
// PC2 in - dol engine down (PCINT10)
// PC3 in - dol engine up (PCINT11)
// PC4 in - gora engine up (PCINT12)
// PC5 in - gora engine down (PCINT13)


#define top_break 0
#define bottom_break 1
#define bottom_down_button 2
#define bottom_up_button 3
#define top_up_button 4
#define top_down_button 5

typedef enum { false, true } bool;

bool is_pressed(char button) {
	return bit_is_set(PINC,button);
}

bool is_up() {
	return bit_is_set(PINC,top_break);
}

bool is_down() {
	return bit_is_set(PINC,bottom_break);
}

void init() {
	DDRC&=0b11000000 ;
	PORTC&=0b00000000 ;
	DDRD|=0b00000011 ;
	PCICR|=_BV(PCIE1);
	PCMSK1|=_BV(PCINT8);
	PCMSK1|=_BV(PCINT9);
	PCMSK1|=_BV(PCINT10);
	PCMSK1|=_BV(PCINT11);
	PCMSK1|=_BV(PCINT12);
	PCMSK1|=_BV(PCINT13);
	sei();
}

void engine_stop() {
	PORTD&=0b11111100;
}

void engine_up() {
	engine_stop();
	PORTD|=0b00000001;
}

void engine_down() {
	engine_stop();
	PORTD|=0b00000010;
}

void engine_error(){
	engine_stop();
	PORTD|=0b00000011;
}

flash(char i){
	volatile char j;
	for(j=i; j>0;j--){
		engine_up();
		_delay_ms(500);
		engine_stop();
		_delay_ms(500);
	}
}



ISR(PCINT1_vect){

	cli();

	_delay_ms(100);

	if((is_pressed(top_up_button) && is_pressed(top_down_button)) || (is_pressed(bottom_up_button) && is_pressed(bottom_down_button))){
		engine_stop();
		_delay_ms(100);
		sei();
		return ;
	}
	

	if(is_pressed(top_up_button) || is_pressed(bottom_up_button)){
		if(is_up()){
			engine_stop();
		}else{
			engine_up();
		}
		sei();
		return ;
	}

	if(is_pressed(top_down_button) || is_pressed(bottom_down_button)){
		if(is_down()){
			engine_stop();
		}else{
			engine_down();
		}
		sei();
		return ;
	}

	if(is_pressed(top_break) || is_pressed(bottom_break)){
		engine_stop();
		sei();
		return ;
	}

	sei();
}


int main() {

	init() ;

	while(1){
	}	


}

