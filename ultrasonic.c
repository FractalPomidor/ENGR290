#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define TRIG_PIN PB1  // Trig pin (Arduino D9 for home test)
#define ECHO_PIN PB2  // Echo pin (Arduino D10 for home test )

// Function to initialize UART for serial communication
void uart_init() 
{
  UBRR0H = 0;
  UBRR0L = 103; 
  UCSR0B = (1 << TXEN0) | (1 << RXEN0); // tx , rx pins
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); 
}

void uart_transmit_char(char data) {
  while (!(UCSR0A & (1 << UDRE0))); // Wait for empty transmit buffer
  UDR0 = data; // Put data into buffer and send
}

void uart_transmit_string(const char* data) {
  for (int i = 0; data[i] != '\0'; i++) {
    uart_transmit_char(data[i]);
  }
}

void ultrasonic_init() {
  // Configure TRIG_PIN as an output and ECHO_PIN as an input
  DDRB |= (1 << TRIG_PIN);
  DDRB &= ~(1 << ECHO_PIN);
}


int ultrasonic_measure_distance() {
  // Send a pulse to initiate a measurement
  PORTB |= (1 << TRIG_PIN);
  _delay_us(10);
  PORTB &= ~(1 << TRIG_PIN);
  
  
  uint16_t pulse_duration = 0;
  while (!(PINB & (1 << ECHO_PIN))) 
  {
    // wait echo pin to trigger
  } 
  while (PINB & (1 << ECHO_PIN)) 
  {    // Measure the duration while the echo pin is high
    pulse_duration++;
    _delay_us(1);
  }
    // formula using speed of light
  int distance_cm = (int)(((float)pulse_duration * 0.0343) / 2.0);
  
  return distance_cm;
}

int main(void) {
  uart_init();         // Setup Serial comm
  ultrasonic_init();  // Setup US

  while (1) 
  {
    int distance = ultrasonic_measure_distance();  
    char buffer[50];
    snprintf(buffer, sizeof(buffer), "Distance: %d cm\r\n", distance);
    uart_transmit_string(buffer);  // Serial comm

    //  LED FEATURE HERE




    _delay_ms(50);  // Delay 
  }
  
  return 0;
}
