#include "UART.h"

UART::UART() {
    // Set the baud rate
    UBRR0H = 0;
    UBRR0L = 8;

    // Enable receiver and transmitter
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    // Set frame format: 8 data bits, no parity, 1 stop bit
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void UART::transmit(unsigned char data) {
    // Wait for empty transmit buffer
    while (!(UCSR0A & (1 << UDRE0)));

    // Put data into buffer, sends the data
    UDR0 = data;
}
unsigned char UART::receive() {
    // Wait for data to be received
    while (!(UCSR0A & (1 << RXC0)));

    // Get and return received data from buffer
    return UDR0;
}
void UART::transmitString(const char* str) {
    while (*str) {
        transmit(*str++);
    }
}

void UART::transmitNumber(long number) {
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "%ld \n\r", number);
    transmitString(buffer);
}

void UART::transmitFloat(float number, int decimal_places) {
    char buffer[32];  // Increased size for safety
    dtostrf(number, 0, decimal_places, buffer);  // AVR-safe float to string
    transmitString(buffer);
}

void UART::print(const char* str) {
    transmitString(str);
}
void UART::print(long number) {
    transmitNumber(number);
}
void UART::print(float number, int decimal_places) {
    transmitFloat(number, decimal_places);
}

void UART::println(const char* str) {
    transmitString(str);
    transmit('\n');
}

void UART::println(long number) {
    transmitNumber(number);
    transmit('\n');
}

void UART::println(float number, int decimal_places) {
    transmitFloat(number, decimal_places);
    transmit('\n');
}
