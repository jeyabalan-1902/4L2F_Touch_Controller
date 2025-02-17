#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <atmel_start.h>
#include "touch.h"
// Spi Communication Pins
#define SPI_SS_PIN PB4 // SS pin connected to PB4
#define SPI_MOSI_PIN PB5 // MOSI pin connected to PB5
#define SPI_MISO_PIN PB6 // MISO pin connected to PB6
#define SPI_SCK_PIN PB7 // SCK pin connected to PB7

// Function declarations
void touchpin1();
void touchpin2();
void touchpin3();
void touchpin4();
void touchpin5();
void touchpin6();
void touchpin7();
void touchpin8();
void touchpin9();
void touchpin10();
// Capacitive touch related variables
extern volatile uint8_t measurement_done_touch;
extern uint8_t key_status_1;
extern uint8_t previous_key_status_1;
extern uint8_t touch_counter_1;
extern uint8_t key_status_2;
extern uint8_t previous_key_status_2;
extern uint8_t touch_counter_2;
extern uint8_t key_status_3;
extern uint8_t previous_key_status_3;
extern uint8_t touch_counter_3;
extern uint8_t key_status_4;
extern uint8_t previous_key_status_4;
extern uint8_t touch_counter_4;
extern uint8_t key_status_5;
extern uint8_t previous_key_status_5;
extern uint8_t touch_counter_5;
extern uint8_t key_status_6;
extern uint8_t previous_key_status_6;
extern uint8_t touch_counter_6;
extern uint8_t key_status_7;
extern uint8_t previous_key_status_7;
extern uint8_t touch_counter_7;
extern uint8_t key_status_8;
extern uint8_t previous_key_status_8;
extern uint8_t touch_counter_8;
extern uint8_t key_status_9;
extern uint8_t previous_key_status_9;
extern uint8_t touch_counter_9;
extern uint8_t key_status_10;
extern uint8_t previous_key_status_10;
extern uint8_t touch_counter_10;
// Declare previous key status and touch counter variables for all touch pins
uint8_t key_status_1 = 0;
uint8_t previous_key_status_1 = 0;
uint8_t touch_counter_1 = 0;
uint8_t key_status_2 = 0;
uint8_t previous_key_status_2 = 0;
uint8_t touch_counter_2 = 0;
uint8_t key_status_3 = 0;
uint8_t previous_key_status_3 = 0;
uint8_t touch_counter_3 = 0;
uint8_t key_status_4 = 0;
uint8_t previous_key_status_4 = 0;
uint8_t touch_counter_4 = 0;
uint8_t key_status_5 = 0;
uint8_t previous_key_status_5 = 0;
uint8_t touch_counter_5 = 0;
uint8_t key_status_6 = 0;
uint8_t previous_key_status_6 = 0;
uint8_t touch_counter_6 = 0;
uint8_t key_status_7 = 0;
uint8_t previous_key_status_7 = 0;
uint8_t touch_counter_7 = 0;
uint8_t key_status_8 = 0;
uint8_t previous_key_status_8 = 0;
uint8_t touch_counter_8 = 0;
uint8_t key_status_9 = 0;
uint8_t previous_key_status_9 = 0;
uint8_t touch_counter_9 = 0;
uint8_t key_status_10 = 0;
uint8_t previous_key_status_10 = 0;
uint8_t touch_counter_10 = 0;
void SPI_MasterInit()
{
	// Set SS, MOSI, and SCK as outputs
	DDRB |= (1 << SPI_SS_PIN) | (1 << SPI_MOSI_PIN) | (1 << SPI_SCK_PIN);
	// Enable SPI, Set as Master
	SPCR0 = (1 << SPE) | (1 << MSTR);
	// Set SS pin high (inactive)
	PORTB |= (1 << SPI_SS_PIN);
}
void SPI_MasterTransmit(uint8_t data)
{
	// Set SS pin low (activate slave)
	PORTB &= ~(1 << SPI_SS_PIN);
	// Start transmission
	SPDR0 = data;
	// Wait for transmission to complete
	while (!(SPSR0 & (1 << SPIF)))
	{
		// Add timeout or error handling if necessary
	}
	// Set SS pin high (deactivate slave)
	PORTB |= (1 << SPI_SS_PIN);
}
void loop()
{
	touch_process();
	// Check if touch measurement is done
	if (measurement_done_touch == 1)
	{
		measurement_done_touch = 0;
		touchpin1();
		touchpin2();
		touchpin3();
		touchpin4();
		touchpin5();
		touchpin6();
		touchpin7();
		touchpin8();
		touchpin9();
		touchpin10();
	}
}
int main(void)
{
	/* Initializes MCU, drivers, and middleware */
	atmel_start_init();
	/* Enable interrupts */
	cpu_irq_enable();
	// Initialize SPI as master
	SPI_MasterInit();
	while (1)
	{
		loop();
	}
	return 0;
}
void touchpin1() {
	// Get the status of the first touch sensor
	previous_key_status_1 = key_status_1;
	key_status_1 = get_sensor_state(0) & 0x80;
	if (key_status_1 && !previous_key_status_1) {
		touch_counter_1++;
		if (touch_counter_1 == 1) {
			SPI_MasterTransmitString("L1");
		}
		else if (touch_counter_1 == 2) {
			SPI_MasterTransmitString("L1");
			
			touch_counter_1 = 0;
		}
	}
}
void touchpin2() {
	// Get the status of the first touch sensor
	previous_key_status_2 = key_status_2;
	key_status_2= get_sensor_state(2) & 0x80;
	if (key_status_2 && !previous_key_status_2) {
		touch_counter_2++;
		if (touch_counter_2 == 1) {
			SPI_MasterTransmitString("L2");
		}
		else if (touch_counter_2 == 2) {
			SPI_MasterTransmitString("L2");
			
			touch_counter_2 = 0;
		}
	}
}
void touchpin3() {
	// Get the status of the first touch sensor
	previous_key_status_3 = key_status_3;
	key_status_3 = get_sensor_state(1) & 0x80;
	if (key_status_3 && !previous_key_status_3) {
		touch_counter_3++;
		if (touch_counter_3 == 1) {
			SPI_MasterTransmitString("L3");
		}
		else if (touch_counter_3 == 2) {
			SPI_MasterTransmitString("L3");
			
			touch_counter_3 = 0;
		}
	}
}
void touchpin4() {
	// Get the status of the first touch sensor
	previous_key_status_4 = key_status_4;
	key_status_4 = get_sensor_state(3) & 0x80;
	if (key_status_4 && !previous_key_status_4) {
		touch_counter_4++;
		if (touch_counter_4 == 1) {
			SPI_MasterTransmitString("L4");
		}
		else if (touch_counter_4 == 2) {
			SPI_MasterTransmitString("L4");
			
			touch_counter_4 = 0;
		}
	}
}
void touchpin5()
{
	
	// Get the status of the fourth touch sensor
	previous_key_status_5 = key_status_5;
	key_status_5 = get_sensor_state(4) & 0x80;

	// Toggle LED 4 based on touch status
	if (key_status_5 && !previous_key_status_5) {
		touch_counter_5++;
		if (touch_counter_5 == 1)
		{
			SPI_MasterTransmitString("F0");
		}
		
		else if (touch_counter_5 == 2) {
			
			SPI_MasterTransmitString("F1");
			
			touch_counter_5 = 0;
		}
	}
}



void touchpin6()
{

	// Get the status of the fourth touch sensor
	previous_key_status_6 = key_status_6;
	key_status_6 = get_sensor_state(5) & 0x80;
	
	// Toggle LED 4 based on touch status
	if (key_status_6 && !previous_key_status_6) {
		touch_counter_6++;
		
		SPI_MasterTransmitString("U1");
	}
	
	
	
}



void touchpin7()
{
	
	// Get the status of the fourth touch sensor
	previous_key_status_7 = key_status_7;
	key_status_7 = get_sensor_state(6) & 0x80;
	
	// Toggle LED 4 based on touch status
	if (key_status_7 && !previous_key_status_7) {
		touch_counter_7++;
		
		SPI_MasterTransmitString("D1");
	}
	
	
	
}
void touchpin8()
{
	
	// Get the status of the fourth touch sensor
	previous_key_status_8 = key_status_8;
	key_status_8 = get_sensor_state(7) & 0x80;

	// Toggle LED 4 based on touch status
	if (key_status_8 && !previous_key_status_8) {
		touch_counter_8++;
		if (touch_counter_8 == 1)
		{
			SPI_MasterTransmitString("F2");
		}
		
		else if (touch_counter_8 == 2) {
			
			SPI_MasterTransmitString("F3");
			
			touch_counter_8 = 0;
		}
	}
}



void touchpin9()
{
	

	// Get the status of the fourth touch sensor
	previous_key_status_9 = key_status_9;
	key_status_9 = get_sensor_state(8) & 0x80;
	
	// Toggle LED 4 based on touch status
	if (key_status_9 && !previous_key_status_9) {
		touch_counter_9++;
		
		SPI_MasterTransmitString("U2");
	}
	
	
	
}



void touchpin10()
{
	
	// Get the status of the fourth touch sensor
	previous_key_status_10 = key_status_10;
	key_status_10 = get_sensor_state(9) & 0x80;
	
	// Toggle LED 4 based on touch status
	if (key_status_10 && !previous_key_status_10) {
		touch_counter_10++;
		
		SPI_MasterTransmitString("D2");

		// Check if all touch counters are equal to 0
		if (touch_counter_1 == 0 && touch_counter_2 == 0 && touch_counter_3 == 0 && touch_counter_4 == 0 && touch_counter_5 == 0 && touch_counter_8 == 0 )
		{
			SPI_MasterTransmitString("Y1");

			// Reset all touch counters after sending the data
			
		}
	}
	
	
	
}
// Function to transmit a string over SPI
void SPI_MasterTransmitString(const char* str) {
	for (int i = 0; str[i] != '\0'; i++) {
		SPI_MasterTransmit(str[i]);
		_delay_ms(100);
	}
}