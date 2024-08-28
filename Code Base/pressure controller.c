#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL

#define PRESSURE_SENSOR_PIN 5
#define PWM_PIN 4
#define STATUS_PIN 2

#define ADC_MAX_VALUE 1023
#define V_REF 5.0
#define IDEAL_VOLTAGE 2.2
#define MIN_VOLTAGE 1.2
#define MAX_VOLTAGE 3.2

// Function prototypes
void ADC_setup();
uint16_t ADC_read(uint8_t channel);
void PWM_setup();
void PWM_setDutyCycle(uint8_t dutyCycle);
void StatusPin_setup();
void StatusPin_set(uint8_t status);
void PressureControl_run();

void ADC_setup() {
	// Select Vref=AVcc
	ADMUX = (1 << REFS0);
	// Set ADC prescaler to 128 for 125kHz ADC clock
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t ADC_read(uint8_t channel) {
	// Select ADC channel
	ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
	// Start conversion
	ADCSRA |= (1 << ADSC);
	// Wait for conversion to complete
	while (ADCSRA & (1 << ADSC));
	return ADC;
}

void PWM_setup() {
	// Set PWM_PIN as output
	DDRD |= (1 << DDD4);
	// Set Timer1 to Fast PWM mode, non-inverted
	TCCR1A = (1 << COM1B1) | (1 << WGM11);
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11); // Prescaler 8
	ICR1 = 39999; // Set TOP value for 20ms period (50Hz)
}

void PWM_setDutyCycle(uint8_t dutyCycle) {
	OCR1B = (ICR1 * dutyCycle) / 100;
}

void StatusPin_setup() {
	// Set STATUS_PIN as output
	DDRD |= (1 << DDD2);
}

void StatusPin_set(uint8_t status) {
	if (status) {
		PORTD |= (1 << PORTD2);
		} else {
		PORTD &= ~(1 << PORTD2);
	}
}

void PressureControl_run() {
	ADC_setup();
	PWM_setup();
	StatusPin_setup();

	while (1) {
		// Read the analog value from the pressure sensor
		uint16_t adcValue = ADC_read(PRESSURE_SENSOR_PIN);
		// Convert ADC value to voltage
		float voltage = (adcValue * V_REF) / ADC_MAX_VALUE;
		// Calculate PWM duty cycle
		float dutyCycle = 50.0 + (IDEAL_VOLTAGE - voltage) * 50.0;

		if (voltage < MIN_VOLTAGE || voltage > MAX_VOLTAGE) {
			StatusPin_set(0);
			} else {
			StatusPin_set(1);
			if (voltage < IDEAL_VOLTAGE) {
				dutyCycle += (IDEAL_VOLTAGE - voltage) * 50.0 / (IDEAL_VOLTAGE - MIN_VOLTAGE);
				} else if (voltage > IDEAL_VOLTAGE) {
				dutyCycle -= (voltage - IDEAL_VOLTAGE) * 50.0 / (MAX_VOLTAGE - IDEAL_VOLTAGE);
			}
		}

		if (dutyCycle > 100) dutyCycle = 100;
		if (dutyCycle < 0) dutyCycle = 0;

		PWM_setDutyCycle((uint8_t)dutyCycle);
		_delay_ms(100);
	}
}

int main() {
	PressureControl_run();
	return 0; // This line will never be reached
}
