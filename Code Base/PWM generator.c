#include <avr/io.h>
#include <util/delay.h>

// Function prototypes
void PWMController_init();
void PWMController_setPWMDutyCycle(uint8_t dutyCycle);
uint8_t PWMController_checkPins();
void PWMController_run();

void PWMController_init() {
	// Set digital pins 0-7 as inputs
	DDRD &= ~0xFF;  // Clear lower 8 bits of DDRD to set PORTD0 to PORTD7 as inputs
	
	// Set digital pin 8 as output (for PWM signal)
	DDRB |= (1 << DDB0);  // Set PORTB0 as output (digital pin 8)

	// Set up Timer0 for PWM
	TCCR0A |= (1 << COM0A1) | (1 << WGM00) | (1 << WGM01);  // Fast PWM mode, non-inverted
	TCCR0B |= (1 << CS01);  // Prescaler set to 8
}

void PWMController_setPWMDutyCycle(uint8_t dutyCycle) {
	OCR0A = (dutyCycle * 255) / 100;  // Set PWM duty cycle
}

uint8_t PWMController_checkPins() {
	for (uint8_t i = 0; i < 8; i++) {
		if (PIND & (1 << i)) {
			return (i + 1) * 10;  // Return duty cycle based on pin level (10%, 20%, ..., 80%)
		}
	}
	return 0;  // If no pins are high, return 0% duty cycle
}

void PWMController_run() {
	while (1) {
		uint8_t dutyCycle = PWMController_checkPins();
		PWMController_setPWMDutyCycle(dutyCycle);
		_delay_ms(100);  // Adjust this value to control the cycle speed
	}
}

int main() {
	PWMController_init();
	PWMController_run();
	return 0;  // This line will never be reached
}
