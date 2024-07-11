/*
 * 4L2F_control_128a.c
 *
 * Created: 06-07-2024 09:34:07
 * Author : JEYABALAN K
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <string.h>

// uSart configuration
#define FOSC 16000000 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC / 16 / BAUD - 1

//EEPROM address definition
#define light1_State_Address 0x00
#define light2_State_Address 0x01
#define light3_State_Address 0x02
#define light4_state_Address 0x03
#define fan1_State_Address 0x04
#define fan2_State_Address 0x05

typedef struct 
{
	uint8_t light1state;
	uint8_t light2state;
	uint8_t light3state;
	uint8_t light4state;
	uint8_t lastF1state;
	uint8_t lastF2state;
}states_t; 
volatile states_t lastStates_t = {0};
	
volatile uint8_t timer_flag = 0;

void USART_Init(unsigned int ubrr);
void USART_Transmit(uint8_t data);
unsigned char USART_Receive(void);

void timer1_init() {
	TCCR1B |= (1 << WGM12); 
	OCR1A = 3624;                                        //for a 200ms delay
	TIMSK |= (1 << OCIE1A);                              // Enable Timer1 Compare A interrupt
	TCCR1B |= (1 << CS12) | (1 << CS10);                 // Set prescaler to 1024
}
	
ISR(TIMER1_COMPA_vect) {
	timer_flag = 1;
}

void custom_delay_ms(uint16_t ms) {
	static uint16_t counter = 0;

	if (timer_flag) {
		timer_flag = 0;
		counter++;

		if (counter >= ms) {
			counter = 0;  // Reset the counter
		}
	}
}

void SPI_init_slave() {
	DDRB |= (1 << DDB4);// Set MISO (Master In Slave Out) as output
	SPCR |= (1 << SPE); // Enable SPI, Set as Slave
	SPCR |= (1 << SPIE); // Enable SPI interrupt
}

//fan 1 speed function

void as0() {                     // Fan1 Speed 0
	PORTC &= ~(1 << PC2);
	PORTC &= ~(1 << PC1);
	PORTC &= ~(1 << PC0);     // Led Pins
	PORTG &= ~(1 << PG1);
	PORTG &= ~(1 << PG0);

	PORTC &= ~(1 << PC7);     //Relay Pins
	custom_delay_ms(200);
	PORTG &= ~(1 << PG2);
	custom_delay_ms(200);
	PORTA &= ~(1 << PA7);
	custom_delay_ms(200);
	PORTA &= ~(1 << PA6);
	custom_delay_ms(200);
}

void as1() {
	PORTC |= (1 << PC2);
	
	PORTC &= ~(1 << PC1);
	PORTC &= ~(1 << PC0);     // Led Pins FAN1 Speed1
	PORTG &= ~(1 << PG1);
	PORTG &= ~(1 << PG0);
	
	PORTC &= ~(1 << PC7);
	custom_delay_ms(200);
	PORTG &=~ (1 << PG2);
	custom_delay_ms(200);
	PORTA |= (1 << PA7);      //2.2K
	custom_delay_ms(200);
	PORTA &= ~(1 << PA6);
	custom_delay_ms(200);
 
}

void as2() {
	PORTC |= (1 << PC2);
	PORTC |= (1 << PC1);

	PORTC &= ~(1 << PC0);     // Led Pins FAN1 Speed2
	PORTG &= ~(1 << PG1);
	PORTG &= ~(1 << PG0);

	PORTC &= ~(1 << PC7);
	custom_delay_ms(200);
	PORTG &= ~(1 << PG2);
	custom_delay_ms(200);
	PORTA &= ~(1 << PA7);
	custom_delay_ms(200);
	PORTA |= (1 << PA6);    //3.3k
	custom_delay_ms(200);
}

void as3() {
	PORTC |= (1 << PC2);
	PORTC |= (1 << PC1);
	PORTC |= (1 << PC0);    // Fan1  Speed 3
	
	PORTG &= ~(1 << PG1);
	PORTG &= ~(1 << PG0);

	PORTC &= ~(1 << PC7);
	custom_delay_ms(200);
	PORTG |= (1 << PG2);      // 4.3K
	custom_delay_ms(200);
	PORTA &= ~(1 << PA7);
	custom_delay_ms(200);
	
	PORTA |= (1 << PA6);
	custom_delay_ms(200);
}

void as4() {
	PORTC |= (1 << PC2);
	PORTC |= (1 << PC1);
	PORTC |= (1 << PC0);    // Fan1  Speed 4
	PORTG |= (1 << PG1);

	PORTG &= ~(1 << PG0);

	PORTC &= ~(1 << PC7);
	custom_delay_ms(200);
	PORTG &= ~(1 << PG2);
	custom_delay_ms(200);
	PORTA |= (1 << PA7);   //5.5K
	custom_delay_ms(200);
	PORTA |= (1 << PA6);
	custom_delay_ms(200);
}

void as5() {
	PORTC |= (1 << PC2);
	PORTC |= (1 << PC1);
	PORTC |= (1 << PC0);    // Fan1  Speed 5
	PORTG |= (1 << PG1);
	PORTG |= (1 << PG0);


	PORTC |= (1 << PC7);     //Direct
	custom_delay_ms(200);
	PORTG &= ~(1 << PG2);
	custom_delay_ms(200);
	PORTA &= ~(1 << PA7);
	custom_delay_ms(200);
	PORTA &= ~(1 << PA6);
	custom_delay_ms(200);
	
}

  //fan2 speed function
  
void af0() {
	PORTD &=~ (1 << PD5);
	PORTD &= ~(1 << PD4);
	PORTD &= ~(1 << PD3);    /// Fan2  Speed 0
	PORTD &= ~(1 << PD2);
	PORTD &= ~(1 << PD1);

	PORTA &=~ (1 << PA3);
	custom_delay_ms(200);
	PORTA &= ~(1 << PA2);
	custom_delay_ms(200);
	PORTA &= ~(1 << PA1);
	custom_delay_ms(200);
	PORTA &= ~(1 << PA0);
	custom_delay_ms(200);
}
void af1() {
	PORTD |= (1 << PD5);

	PORTD &= ~(1 << PD4);
	PORTD &= ~(1 << PD3);    /// Fan2  Speed 1
	PORTD &= ~(1 << PD2);
	PORTD &= ~(1 << PD1);
	
	PORTA &=~ (1 << PA3);
	custom_delay_ms(200);
	PORTA &=~ (1 << PA2);
	custom_delay_ms(200);
	PORTA |= (1 << PA1);    //2.2K
	custom_delay_ms(200);
	PORTA &= ~(1 << PA0);
	custom_delay_ms(200);
}

void af2() {
	PORTD |= (1 << PD5);
	PORTD |= (1 << PD4);    // Fan2  Speed 2
	
	PORTD &= ~(1 << PD3);
	PORTD &= ~(1 << PD2);
	PORTD &= ~(1 << PD1);
	
	PORTA &= ~(1 << PA3);
	custom_delay_ms(200);
	PORTA &= ~(1 << PA2);
	custom_delay_ms(200);
	PORTA &= ~(1 << PA1);
	custom_delay_ms(200);
	PORTA |= (1 << PA0);   //3.3K
	custom_delay_ms(200);
}

void af3() {
	PORTD |= (1 << PD5);
	PORTD |= (1 << PD4);
	PORTD |= (1 << PD3);      // Fan2  Speed 3
	
	PORTD &= ~(1 << PD2);
	PORTD &= ~(1 << PD1);

	PORTA &= ~(1 << PA3);
	custom_delay_ms(200);
	PORTA |= (1 << PA2);  //1K+3.3K = 4.3K
	custom_delay_ms(200);
	PORTA &= ~(1 << PA1);
	custom_delay_ms(200);
	PORTA |= (1 << PA0);
	custom_delay_ms(200);
}

void af4() {
	PORTD |= (1 << PD5);
	PORTD |= (1 << PD4);
	PORTD |= (1 << PD3);      // Fan2  Speed 4
	PORTD |= (1 << PD2);

	PORTD &= ~(1 << PD1);
	
	PORTA &=~ (1 << PA3);
	custom_delay_ms(200);
	PORTA &=~ (1 << PA2);
	custom_delay_ms(200);
	PORTA |= (1 << PA1);  //2.2K +3.3K = 5.5K
	custom_delay_ms(200);
	PORTA |= (1 << PA0);
	custom_delay_ms(200);
}

void af5() {
	PORTD |= (1 << PD5);
	PORTD |= (1 << PD4);
	PORTD |= (1 << PD3);      // Fan2  Speed 5
	PORTD |= (1 << PD2);
	PORTD |= (1 << PD1);

	PORTA |= (1 << PA3);
	custom_delay_ms(200);     //Direct
	PORTA &= ~(1 << PA2);
	custom_delay_ms(200);
	PORTA &= ~(1 << PA1);
	custom_delay_ms(200);
	PORTA &= ~(1 << PA0);
	custom_delay_ms(200);
}

//SPI received data 

void processCommand(const char* command){
	
	if(strcmp(command, "P1") == 0){     //Hard Reset
		USART_Transmit('Q');
	}
	
	else if(strcmp(command, "L1") == 0){      // Light 1
		USART_Transmit('S');
		PORTD ^= (1 << PD7);
		PORTC ^= (1 << PC5);     // toggle the LED & Relay
		uint8_t newLightState_1 = (lastStates_t.light1state == 0) ? 1 : 0;
		if(newLightState_1 != lastStates_t.light1state)
		{
			lastStates_t.light1state = newLightState_1;
			eeprom_write_byte((uint8_t*)light1_State_Address,lastStates_t.light1state );
		}
	}
	
	else if(strcmp(command, "L2") == 0){      //Light 2
		USART_Transmit('P');
		PORTC ^= (1 << PC3);
		PORTC ^= (1 << PC6);
		uint8_t newLightState_2 = ( lastStates_t.light2state == 0) ? 1 : 0;
		if(newLightState_2 != lastStates_t.light2state){
			lastStates_t.light2state = newLightState_2;
			eeprom_write_byte((uint8_t*)light2_State_Address, lastStates_t.light2state);
		}
	}
	
	else if (strcmp(command, "L3") == 0) {     //Light 3
		USART_Transmit('R');
		PORTD ^= (1 << PD6);
		PORTA ^= (1 << PA5);
		uint8_t newLightState_3 = (lastStates_t.light3state == 0) ? 1 : 0;
		if (newLightState_3 != lastStates_t.light3state){
			lastStates_t.light3state = newLightState_3;
			eeprom_write_byte((uint8_t*)light3_State_Address, lastStates_t.light3state); //Store the Light state 3 in EEPROM
		}
	}
	
	else if (strcmp(command, "L4") == 0) {    //Light 4
		USART_Transmit('G');
		PORTC ^= (1 << PC4);
		PORTA ^= (1 << PA4);
		uint8_t newLightState_4 = (lastStates_t.light4state == 0) ? 1 : 0;
		if (newLightState_4 != lastStates_t.light4state){
			lastStates_t.light4state = newLightState_4;
			eeprom_write_byte((uint8_t*)light4_state_Address, lastStates_t.light4state); //Store the Light state 3 in EEPROM
		}
	}
	
	else if (strcmp(command, "F0") == 0) {      // Fan 1 ON
		if (lastStates_t.lastF1state == 5) {
			USART_Transmit('F');
			as5();
			lastStates_t.lastF1state = 5;
		}
		else if (lastStates_t.lastF1state == 4) {
			USART_Transmit('E');
			as4();
			lastStates_t.lastF1state = 4;
		}
		else if (lastStates_t.lastF1state == 3) {
			USART_Transmit('D');
			as3();
			lastStates_t.lastF1state = 3;
		}
		else if (lastStates_t.lastF1state == 2) {
			USART_Transmit('C');
			as2();
			lastStates_t.lastF1state = 4;
		}
		else if (lastStates_t.lastF1state == 1) {
			USART_Transmit('A');
			as1();
			lastStates_t.lastF1state = 5;
		}
		else if(lastStates_t.lastF1state == 0){
			as1();
		}
		eeprom_write_byte((uint8_t*)fan1_State_Address, 1);
	}
	
	else if (strcmp(command, "F1") == 0) {      //Fan 1 OFF
		USART_Transmit('B');
		as0();
		eeprom_write_byte((uint8_t*)fan1_State_Address, 0);
	}
	
	else if (strcmp(command, "F2") == 0) {         // Fan 2 ON
		if (lastStates_t.lastF2state == 5) {
			USART_Transmit('J');
			af5();
			lastStates_t.lastF2state = 5;
		}
		else if (lastStates_t.lastF2state == 4) {
			USART_Transmit('I');
			af4();
			lastStates_t.lastF2state = 4;
		}
		else if (lastStates_t.lastF2state == 3) {
			USART_Transmit('X');
			af3();
			lastStates_t.lastF2state = 3;
		}
		else if (lastStates_t.lastF2state == 2) {
			USART_Transmit('Z');
			af2();
			lastStates_t.lastF2state = 2;
		}
		else if (lastStates_t.lastF2state == 1){
			USART_Transmit('K');
			af1();
			lastStates_t.lastF2state = 1;
		}
		else if (lastStates_t.lastF2state == 0){
			af1();
		}
		eeprom_write_byte((uint8_t*)fan2_State_Address, 1);
	}
	
	else if (strcmp(command, "F3") == 0) {     //Fan 2 OFF
		USART_Transmit('W');
		af0();
		eeprom_write_byte((uint8_t*)fan2_State_Address, 0);
	}
	
	else if (strcmp(command, "U1") == 0) {     //Fan 1 Increment
		if ((PORTC & (1 << PC2)) == 0) {
			USART_Transmit('A');
			as1();
			lastStates_t.lastF1state = 1;
		}
		else if ((PORTC & (1 << PC1)) == 0) {
			USART_Transmit('C');
			as2();
			lastStates_t.lastF1state = 2;
		}
		else if ((PORTC & (1 << PC0)) == 0) {
			USART_Transmit('D');
			as3();
			lastStates_t.lastF1state = 3;
		}
		else if ((PORTG & (1 << PG1)) == 0) {
			USART_Transmit('E');
			as4();
			lastStates_t.lastF1state = 4;
		}
		else if ((PORTG & (1 << PG0)) == 0) {
			USART_Transmit('F');
			as5();
			lastStates_t.lastF1state = 5;
		}
		eeprom_write_byte((uint8_t*)fan1_State_Address, 1);
	}
	
	else if (strcmp(command, "U2") == 0) {        // Fan 2 Increment
		if ((PORTD & (1 << PD5)) == 0) {	
			USART_Transmit('K');
			af1();
			lastStates_t.lastF2state = 1;
		}
		else if ((PORTD & (1 << PD4)) == 0) {
			USART_Transmit('Z');
			af2();
			lastStates_t.lastF2state = 2;
		}
		
		else if ((PORTD & (1 << PD3)) == 0) {
			USART_Transmit('X');
			af3();
			lastStates_t.lastF2state = 3;
		}
		else if ((PORTD & (1 << PD2)) == 0) {
			USART_Transmit('I');
			af4();
			lastStates_t.lastF2state = 4;
		}
		
		else if ((PORTD & (1 << PD1)) == 0) {
			USART_Transmit('J');
			af5();
			lastStates_t.lastF2state = 5;
		}
		eeprom_write_byte((uint8_t*)fan2_State_Address, 1);
	}
	
	else if (strcmp(command, "D1") == 0) {    // Fan 1 Decrement
		if ((PORTG & (1 << PG0)) != 0) {	
			USART_Transmit('E');
			as4();
			lastStates_t.lastF1state = 4;
		}
		else if ((PORTG & (1 << PG1)) != 0) {
			USART_Transmit('D');
			as3();
			lastStates_t.lastF1state = 3;
		}
		else if ((PORTC & (1 << PC0)) != 0) {
			USART_Transmit('C');
			as2();
			lastStates_t.lastF1state = 2;
		}
		else if ((PORTC & (1 << PC1)) != 0) {
			USART_Transmit('A');
			as1();
			lastStates_t.lastF1state = 1;
		}
		eeprom_write_byte((uint8_t*)fan1_State_Address, 1);
	}
	
	else if (strcmp(command, "D2") == 0) {  //Fan 2 Decrement
		if ((PORTD & (1 << PD1)) != 0) {
			USART_Transmit('I');
			af4();
			lastStates_t.lastF2state = 4;
		}
		else if ((PORTD & (1 << PD2)) != 0) {
			USART_Transmit('X');
			af3();
			lastStates_t.lastF2state = 3;
		}
		else if ((PORTD & (1 << PD3)) != 0) {
			USART_Transmit('Z');
			af2();
			lastStates_t.lastF2state = 2;
		}
		else if ((PORTD & (1 << PD4)) != 0) {
			USART_Transmit('K');
			af1();
			lastStates_t.lastF2state = 1;
		}
		eeprom_write_byte((uint8_t*)fan2_State_Address, 1);
	}	
}

ISR(SPI_STC_vect) {
	uint8_t receivedData = SPDR; // Read received data from SPI data register
	static char receivedBytes[3];
	static uint8_t byteIndex = 0;
	if (byteIndex < 2) {
		receivedBytes[byteIndex] = receivedData;
		byteIndex++;
		if (byteIndex == 2) {
			receivedBytes[2] = '\0';
			processCommand(receivedBytes);  // Process the received command
			byteIndex = 0;
		}
	}
	SPSR |= (1 << SPIF);
}

void read_Last_State_From_EEPROM(){
	lastStates_t.light1state = eeprom_read_byte((uint8_t*)light1_State_Address); // Read Light1state from EEPROM
	lastStates_t.light2state = eeprom_read_byte((uint8_t*)light2_State_Address); // Read Light2 state from EEPROM
	lastStates_t.light3state = eeprom_read_byte((uint8_t*)light3_State_Address); // Read Light3 state from EEPROM
	lastStates_t.light4state = eeprom_read_byte((uint8_t*)light4_state_Address); // Read light4 state from EEPROM
	lastStates_t.lastF1state = eeprom_read_byte((uint8_t*)fan1_State_Address);   // Read Fan1 state from EEPROM
	lastStates_t.lastF2state = eeprom_read_byte((uint8_t*)fan2_State_Address);   // Read fan2 state from EEPROM
}

void clearEEPROM() {
	if(lastStates_t.light1state == 0xFF){eeprom_write_byte((uint8_t*)light1_State_Address, 0x00);}
    if(lastStates_t.light2state == 0xFF){eeprom_write_byte((uint8_t*)light2_State_Address, 0x00);}
	if(lastStates_t.light3state == 0xFF){eeprom_write_byte((uint8_t*)light3_State_Address, 0x00);}
    if(lastStates_t.light4state == 0xFF){eeprom_write_byte((uint8_t*)light4_state_Address, 0x00);}
	if(lastStates_t.lastF1state == 0xFF){eeprom_write_byte((uint8_t*)fan1_State_Address, 0x00);}
	if(lastStates_t.lastF2state == 0xFF){eeprom_write_byte((uint8_t*)fan2_State_Address, 0x00);}	
}

void retain_Lights_State_From_EEPROM(){
	if(lastStates_t.light1state){
		PORTD ^= (1 << PD7);
		PORTC ^= (1 << PC5);
		custom_delay_ms(50);
	}
	if(lastStates_t.light2state){
		PORTC ^= (1 << PC3);
		PORTC ^= (1 << PC6);
		custom_delay_ms(50);
	}
	if(lastStates_t.light3state){
		PORTD ^= (1 << PD6);
		PORTA ^= (1 << PA5);
		custom_delay_ms(50);
	}
	if(lastStates_t.light4state){
		PORTC ^= (1 << PC4);
		PORTA ^= (1 << PA4);
		custom_delay_ms(50);
	}
}

void retain_Fans_State_From_EEPROM(){
	if(lastStates_t.lastF1state == 0){
		as0();
		custom_delay_ms(100);
	}
	if(lastStates_t.lastF1state == 1){
		as3();
		custom_delay_ms(100);
	}
	if(lastStates_t.lastF2state == 0){
		af0();
		custom_delay_ms(100);
	}
	if(lastStates_t.lastF2state == 1){
		af3();
		custom_delay_ms(100);
	}
}

void setup(){
	timer1_init();
	SPI_init_slave(); // Initialize SPI as a slave
	USART_Init(MYUBRR); //Initialize Uart
	sei(); // global interrupt
	
	DDRD |= (1 << PD1) |(1 << PD2) |(1 << PD3) |(1 << PD4) |(1 << PD5) |(1 << PD6) |(1 << PD7);              // Set PORTD pins as output
	DDRG |= (1 << PG0) | (1 << PG1)| (1 << PG2);                                                             // Set PORTG pins 0 and 1 as output for LEDs
	DDRC |= (1 << PC5) | (1 << PC6)| (1 << PC7)| (1 << PC0)| (1 << PC1)| (1 << PC2)| (1 << PC3)| (1 << PC4); // Set PORTC pins as output
	DDRA |= (1 << PA5) | (1 << PA4)|(1 << PA7) | (1 << PA6)| (1 << PA3)|(1 << PA2) | (1 << PA1)| (1 << PA0); // Set PORTA pins as output
}

int main(void){
	setup();
	custom_delay_ms(100);
	read_Last_State_From_EEPROM();
	custom_delay_ms(100);
	retain_Lights_State_From_EEPROM();
	custom_delay_ms(100);
	retain_Fans_State_From_EEPROM();
	custom_delay_ms(100);
	clearEEPROM();
	
	while (1) {
		custom_delay_ms(200);
		// Receive data
		if (UCSR0A & (1 << RXC0)) {
			unsigned char receivedData = UDR0;
			// Check the received data for different commands
			if (receivedData == 'L') {
				// Wait for the next character
				receivedData = USART_Receive();
				if (receivedData == '1') {
					PORTD |= (1 << PD7);
					PORTC |= (1 << PC5);
					lastStates_t.light1state = 1;
				}
				else if (receivedData == '0') {
					PORTD &= ~(1 << PD7);
					PORTC &= ~(1 << PC5);
					lastStates_t.light1state = 0;
				}
				eeprom_write_byte((uint8_t*)light1_State_Address, lastStates_t.light1state); // Store the Light1 state in EEPROM
			}
			else if (receivedData == 'M') {
				receivedData = USART_Receive();
				if (receivedData == '1') {
					PORTC |= (1 << PC3);
					PORTC |= (1 << PC6);
					lastStates_t.light2state = 1;
				}
				
				else if (receivedData == '0') {
					PORTC &=~ (1 << PC3);
					PORTC &=~ (1 << PC6);
					lastStates_t.light2state = 0;
				}
				eeprom_write_byte((uint8_t*)light2_State_Address, lastStates_t.light2state); // Store the Light2 state in EEPROM
			}
			else if (receivedData == 'N') {
				receivedData = USART_Receive();
				if (receivedData == '1') {
					PORTD |= (1 << PD6);
					PORTA |= (1 << PA5);
					lastStates_t.light3state = 1;
				}
				else if (receivedData == '0') {
					PORTD &= ~(1 << PD6);
					PORTA &= ~(1 << PA5);
					lastStates_t.light3state = 0;
				}
				eeprom_write_byte((uint8_t*)light3_State_Address, lastStates_t.light3state); // Store the Light state 3 in EEPROM
			}
			else if (receivedData == 'P') {
				receivedData = USART_Receive();
				if (receivedData == '1') {
					PORTC |= (1 << PC4);
					PORTA |= (1 << PA4);
					lastStates_t.light4state = 1;
				}
				else if (receivedData == '0') {
					PORTC &= ~(1 << PC4);
					PORTA &= ~(1 << PA4);
					lastStates_t.light4state = 0;
				}
				eeprom_write_byte((uint8_t*)light4_state_Address, lastStates_t.light4state); // Store the Light state 4 in EEPROM
			}
			else if (receivedData == 'F') {
				receivedData = USART_Receive();
				if (receivedData == '1') {
					as1();
					lastStates_t.lastF1state = 1;
				}
				else if (receivedData == '0') {
					as0();
					lastStates_t.lastF1state = 0;
				}
				eeprom_write_byte((uint8_t*)fan1_State_Address, lastStates_t.lastF1state);
			}
			else if (receivedData == 'U') {
				receivedData = USART_Receive();
				if (receivedData == '2') {
					as2();
					lastStates_t.lastF1state = 2;
				}
				else if (receivedData == '3') {
					as3();
					lastStates_t.lastF1state = 3;
				}
				else if (receivedData == '4') {
					as4();
					lastStates_t.lastF1state = 4;
				}
				else if (receivedData == '5') {
					as5();
					lastStates_t.lastF1state = 5;
				}
				eeprom_write_byte((uint8_t*)fan1_State_Address, 1);
			}
			else if (receivedData == 'G') {
				receivedData = USART_Receive();
				if (receivedData == '1') {
					af1();
					lastStates_t.lastF2state = 1;
				}
				else if (receivedData == '0') {
					af0();
					lastStates_t.lastF2state = 0;
				}
				eeprom_write_byte((uint8_t*)fan2_State_Address, lastStates_t.lastF2state);
			}
			if (receivedData == 'H') {
				receivedData = USART_Receive();
				if (receivedData == '2') {
					af2();
					lastStates_t.lastF2state = 2;
				}
				else if (receivedData == '3') {
					af3();
					lastStates_t.lastF2state = 3;
				}
				else if (receivedData == '4') {
					af4();
					lastStates_t.lastF2state = 4;
				}
				else if (receivedData == '5') {
					af5();
					lastStates_t.lastF2state = 5;
				}
				eeprom_write_byte((uint8_t*)fan2_State_Address, 1);
			}	
		}
	}
	return 0;	
}

void USART_Init(unsigned int ubrr)        // UART Initialization
{
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}

void USART_Transmit(uint8_t data) {    // UART Transmit function
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;	
}

unsigned char USART_Receive(void)     // UART Received Function
{
	while (!(UCSR0A & (1 << RXC0)));
	return UDR0;
}