#include <avr/io.h>
#define F_CPU 16000000u
#include <util/delay.h>

int ind[8];//Массив для вывода цифр
int const st = 150;//Задержка для переключения цифры
int const sp = 75;//Задержка для мигания (st/2)
int but = 0;//Используется ли кнопка

void cifr(void){//Запись цифр в массив
	ind[0] = 0b00000110;
	ind[1] = 0b01011011;
	ind[2] = 0b01001111;
	ind[3] = 0b01100110;
	ind[4] = 0b01101101;
	ind[5] = 0b01111101;
	ind[6] = 0b00000111;
	ind[7] = 0b01111111;
	ind[8] = 0b01101111;
}

//Можно запустить и посмотреть, в основной программе не используется:
/*void Avtonom1(void){//Для обычной дороги с пешеходным переходом
	PORTD = 0b11111110;
	for (int i=8; i>=0; i--){
		PORTG = 1;
		PORTB = ind[i];
		PORTC = ind[i];
		if (i<3){
			PORTD = 0b11111100;
			_delay_ms(sp);
			PORTG = 3;
			_delay_ms(sp);
		} else _delay_ms(st);
	}
	for (int i=8; i>=0; i--){
		PORTG = 2;
		PORTD = 0b11111011;
		PORTC = ind[i];
		if (i>5){
			PORTB = ind[8];
			_delay_ms(sp);
			PORTG = 3;
			_delay_ms(sp);
		} else if (i<3){
			PORTB = ind[i+3];
			_delay_ms(sp);
			PORTD = 0b11111111;
			_delay_ms(sp);
		} else {
			PORTB = ind[i+3];
			_delay_ms(st);
		}
	}
	PORTD = 0b11111101;
	PORTC = 2;
	for (int i=2; i>=0; i--){
		PORTG = 2;
		PORTC = ind[i];
		PORTB = ind[i];
		_delay_ms(st);
	}
}*/

//Светофор 1 жёлтый, светофор 2 красный, пешеходный готовится к запуску
//1)Светофор 1 горит красным, светофор 2 красный, пешеходный зелёный
//2)Светофор 1 горит красным, светофор 2 зелёный, пешеходный красный
//Жёлтый свет на обоих светофорах
//3)Светофор 1 горит зелёным, светофор 2 красный, пешеходный красный
void Avtonom2(void){//Для перекрёстка с пешеходным переходом
	for (int i=8; i>=0; i--){//Состояние 3
		PORTG = 2;
		PORTD = 0b11110011;
		PORTC = ind[i];
		PORTA = ind[8];
		if (i>5){
			PORTB = ind[8];
			_delay_ms(sp);
			PORTG = 3;
			PORTD |= 0b00001000;
			_delay_ms(sp);
		} else if (i<3){
			PORTB = ind[i+3];
			_delay_ms(sp);
			PORTD |= 0b00001100;
			_delay_ms(sp);
		} else {
			PORTB = ind[i+3];
			_delay_ms(sp);
			PORTD |= 0b00001000;
			_delay_ms(sp);
		}
	}
	for (int i=2; i>=0; i--){
		PORTG = 2;
		PORTD = 0b11110101;
		PORTC = ind[i];
		PORTB = ind[i];
		PORTA = ind[8];
		_delay_ms(sp);
		PORTD |= 0b00001000;
		_delay_ms(sp);
	}
	for (int i=8; i>=0; i--){//Состояние 1
		PORTD = 0b11110110;
		PORTG = 1;
		PORTA = ind[i];
		PORTB = ind[i];
		PORTC = ind[8];
		if (i<3) {
			PORTD &= 0b11101111;
			_delay_ms(sp);
			PORTD |= 0b00000001;
			PORTG = 3;
			_delay_ms(sp);
		} else {
		_delay_ms(sp);
		PORTD |= 0b00000001;
		_delay_ms(sp);
		}
	}
	for (int i=8; i>=0; i--){//Состояние 2
		PORTG = 2;
		PORTD = 0b11011110;
		PORTB = ind[8];
		PORTA = ind[i];
		if (i>5){
			PORTC = ind[8];
			_delay_ms(sp);
			PORTG = 3;
			PORTD |= 0b00000001;
			_delay_ms(sp);
		} else if (i<3){
			PORTC = ind[i+3];
			_delay_ms(sp);
			PORTD |= 0b00100000;
			PORTG = 3;
			_delay_ms(sp);
		} else {
			PORTC = ind[i+3];
			_delay_ms(sp);
			PORTG = 3;
			_delay_ms(sp);
		}
	}
	for (int i=2; i>=0; i--){
		PORTG = 2;
		PORTD = 0b11101100;
		PORTC = ind[i];
		PORTA = ind[i];
		_delay_ms(sp);
		PORTG = 3;
		_delay_ms(sp);
	}
}

//Автономный режим, при котором работают только светофоры для автомобилей
//Светофор 1 горит красным, светофор 2 горит зелёным
//Потом жёлтый свет
//Светофор 2 горит красным, светофор 1 горит зелёным
//Потом жёлтый свет
void Before_button(void){
	for (int i=8; i>=0; i--){
		PORTD = 0b11110011;
		PORTG = 2;
		PORTC = ind[i];
		PORTB = ind[8];
		if (i>5){
			PORTA = ind[8];
			_delay_ms(sp);
			PORTG = 3;
			PORTD |= 0b00001000;
			_delay_ms(sp);
		} else if (i<3){
			PORTA = ind[i+3];
			_delay_ms(sp);
			PORTG = 3;
			PORTD = 0b11110111;
			_delay_ms(sp);
		} else {
			PORTA = ind[i+3];
			_delay_ms(sp);
			PORTG = 3;
			_delay_ms(sp);
			}
	}
	PORTD = 0b11100101;
	for (int i=2; i>=0; i--){
		PORTG = 2;
		PORTC = ind[i];
		PORTA = ind[i];
		_delay_ms(sp);
		PORTG = 3;
		_delay_ms(sp);
	}
	for (int i=8; i>=0; i--){
		PORTD = 0b11011110;
		PORTG = 2;
		PORTA = ind[i];
		PORTB = ind[8];
		if (i>5){
			PORTC = ind[8];
			_delay_ms(sp);
			PORTG = 3;
			PORTD |= 0b00000001;
			_delay_ms(sp);
		} else if (i<3){
			PORTC = ind[i+3];
			_delay_ms(sp);
			PORTG = 3;
			PORTD = 0b11111110;
			_delay_ms(sp);
		} else {
			PORTC = ind[i+3];
			_delay_ms(sp);
			PORTG = 3;
			_delay_ms(sp);
			}
	}
	PORTD = 0b11101100;
	for (int i=2; i>=0; i--){
		PORTG = 2;
		PORTC = ind[i];
		PORTA = ind[i];
		_delay_ms(sp);
		PORTG = 3;
		_delay_ms(sp);
	}
}

void Mig(void){//Мигание жёлтых сигналов
	PORTD = 0b11101101;
	PORTC = ind [8];
	PORTA = ind [8];
	_delay_ms(sp);
	PORTD = 255;
	_delay_ms(sp);
}

void Knopk(void){
	 if (PINE & 0b00000001) Before_button();
	 else Avtonom2();
}

void ini(void){//Подготовка портов
	DDRA = 255;
	DDRB = 255;
	DDRC = 255;
	DDRD = 255;
	DDRE = 2;
	DDRG = 255;
	PORTE = 13;
	DDRF = 0;
	PORTF = 0;
}

void adc_init(){
	ADCSRA = 0;
	ADMUX = 0;
	ADCSRA |= 1 << ADEN;
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	// ADATE == 0 - одиночное преобразование
	// без прерываний
	ADMUX |= (1 << REFS1) | (1 << REFS0); // внутренний ист.напряж.
	// вывод PA0 на АЦП
	// калибровочный запуск
	ADCSRA |= (1 << ADSC);
	while(ADCSRA & (1 << ADSC)); // ожидание результата
}

uint16_t adc_read(uint8_t ch){
	ADMUX = ADMUX & (0b00001111 | ch);
	ADCSRA |= (1 << ADSC);
	while(ADCSRA & (1 << ADSC))
		return ADC;
}

int main(void){
	ini();
	adc_init();
	cifr();
	while (1) {
		if ((float) adc_read(0)/2 < 50) {
			PORTE = 13;
			but = 1;
		}
		else {
			PORTE = 15;
			but = 0;
		}
		if (!(PINE & 0b00000100)){
			PORTA = 0;
			PORTB = 0;
			PORTC = 0;
		} else if (!(PINE & 0b00001000)){
			Mig();
		}
		 else if (but == 1) Avtonom2();
		else if (but == 0) Knopk();
	}
}