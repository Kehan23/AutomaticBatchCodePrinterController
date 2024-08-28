#include <LiquidCrystal_I2C.h>

#define F_CPU 16000000UL  // 16 MHz clock speed
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define LCD_ADDR 0x27

// LCD address and dimensions
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Potentiometer PIN A1
int Keyboard = A3;

// Variables capturing current and newly calculated position on the letter board
int New_X = 0;
int Old_X = 0;
double Now_time = 0;

bool toggle;

// Variable capturing output from Keyboard pin (Values 0 - 1023)
int Key_read = 0;
int Prev_Key_read = 1023;
boolean Key_pressed = false;

// String variable holding the text to transmit
String To_Transmit = "";

// Used for displaying Letter board
char Letters[] = "EFDMF123456789.";
char* Letters2[] = { "E", "F", "D", "M", "F", "1", "2", "3", "4", "5", "6", "7", "8", "9", "." };

void TWI_init(void);
void TWI_start(void);
void TWI_stop(void);
void TWI_write(uint8_t data);
void lcd_send(uint8_t value, uint8_t mode);
void lcd_command(uint8_t command);
void lcd_data(uint8_t data);
void lcd_init(void);
void lcd_set_cursor(uint8_t row, uint8_t col);
void lcd_print(char* str);

void setup() {
  Serial.begin(9600);

  // // Initialize the LCD
  // lcd.init();
  // lcd.backlight();

  TWI_init();  // Initialize TWI (I2C) and LCD
  lcd_init();

  // Display the initial "OK" indicator
  lcd_set_cursor(14, 0);
  lcd_print("OK");

  // Display the initial "OK" indicator
  lcd_set_cursor(0, 1);
  lcd_print("EFDMF123456789.");

  // Highlight character 'E' by placing a cursor at the first position
  lcd_set_cursor(0, 1);
  // lcd.blink(); // This will make the cursor blink on the selected character
  // lcd_blink_cell(0, 1, "E", 0, 500, 5);

  // delay(1000);

  Now_time = millis();
  toggle = true;
}

void loop() {

  if (millis() - Now_time > 500) {
    if (toggle) {
      lcd_set_cursor(New_X, 1);
      lcd_print(Letters2[New_X]);
    } else {
      lcd_set_cursor(New_X, 1);
      lcd_print(" ");
    }
    toggle = not toggle;
    Now_time = millis();
  }

  Key_read = analogRead(Keyboard);
  Serial.println(Key_read);
  if (Prev_Key_read > 1000 && Key_read < 1000) {
    lcd_set_cursor(0, 1);
    lcd_print("EFDMF123456789.");
    Key_pressed = true;

    // Move left
    if (Key_read > 470 && Key_read < 520 && Old_X > 0) New_X = Old_X - 1;

    // Move right
    if (Key_read < 10 && Old_X < 20) New_X = Old_X + 1;

    // Select character
    if (Key_read > 690 && Key_read < 780) {
      if (New_X != -1) {
        To_Transmit += Letters[New_X];
        lcd_set_cursor(0, 0);
        lcd_print((char*)To_Transmit.c_str());
        lcd_set_cursor(0, 1);
        lcd_print("EFDMF123456789.");
      }
    }

    // Highlight the new letter if cursor moved
    if (Old_X != New_X) {
      // Highlight_letter(New_X, Old_X);
      // lcd_blink_cell(New_X, 1, Letters2[New_X], 0, 500, 1);
      Old_X = New_X;
    }
  }

  Prev_Key_read = Key_read;
}

//////////////////////////////////LCD////////////////////////////

void lcd_blink_cell(uint8_t row, uint8_t col, char* str, uint8_t blink_pos, uint16_t delay_ms, uint8_t blink_count) {
  for (uint8_t i = 0; i < blink_count; i++) {
    // Print the full string with the specified character blank
    lcd_set_cursor(row, col);
    for (uint8_t j = 0; j < strlen(str); j++) {
      if (j == blink_pos) {
        lcd_data(' ');  // Print a blank space at the blink position
      } else {
        lcd_data(str[j]);
      }
    }
    _delay_ms(delay_ms);

    // Print the full string with the original character
    lcd_set_cursor(row, col);
    lcd_print(str);
    _delay_ms(delay_ms);
  }
}


void TWI_init(void) {
  TWSR = 0x00;
  TWBR = 0x48;
  TWCR = (1 << TWEN);  // Enable TWI
}

void TWI_start(void) {
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);  // Send start condition
  while (!(TWCR & (1 << TWINT)))
    ;  // Wait for TWINT flag
}

void TWI_stop(void) {
  TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);  // Send stop condition
  while (TWCR & (1 << TWSTO))
    ;  // Wait for stop to complete
}

void TWI_write(uint8_t data) {
  TWDR = data;                        // Load data into TWDR
  TWCR = (1 << TWINT) | (1 << TWEN);  // Start transmission
  while (!(TWCR & (1 << TWINT)))
    ;  // Wait for TWINT flag
}

void lcd_send(uint8_t value, uint8_t mode) {
  TWI_start();               // Start I2C communication
  TWI_write(LCD_ADDR << 1);  // Write LCD address

  uint8_t data = (value & 0xF0) | mode | 0x08;  // Send upper nibble
  TWI_write(data);
  TWI_write(data | 0x04);  // Pulse the enable bit
  TWI_write(data & ~0x04);

  data = ((value << 4) & 0xF0) | mode | 0x08;  // Send lower nibble
  TWI_write(data);
  TWI_write(data | 0x04);  // Pulse the enable bit
  TWI_write(data & ~0x04);

  TWI_stop();  // Stop I2C communication
}

void lcd_command(uint8_t command) {
  lcd_send(command, 0);
  _delay_ms(2);
}

void lcd_data(uint8_t data) {
  lcd_send(data, 1);
  _delay_ms(2);
}

void lcd_init(void) {
  _delay_ms(50);      // Wait for LCD to power up
  lcd_command(0x02);  // Initialize LCD in 4-bit mode
  lcd_command(0x28);  // Function set: 4-bit mode, 2 lines, 5x8 dots
  lcd_command(0x0C);  // Display on, cursor off
  lcd_command(0x06);  // Entry mode set: Increment cursor
  lcd_command(0x01);  // Clear display
  _delay_ms(2);
}

void lcd_set_cursor(uint8_t col, uint8_t row) {
  uint8_t address = (row == 0) ? 0x80 : 0xC0;
  address += col;
  lcd_command(address);
}

void lcd_print(char* str) {
  while (*str) {
    lcd_data(*str++);
  }
}