#include <avr/io.h>
#include "I2C.h"

void TWI_init(void) {
    // Set bit rate register (SCL freq = F_CPU / (16 + 2*TWBR*Prescaler))
    TWSR = 0x00;       // Prescaler = 1
    TWBR = 32;         // SCL freq ~ 100kHz if F_CPU = 16MHz
}

void TWI_start(void) {
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); // Send START
    while (!(TWCR & (1 << TWINT)));                   // Wait for complete
}

void TWI_stop(void) {
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO); // Send STOP
    while (TWCR & (1 << TWSTO));                      // Wait until STOP sent
}

void TWI_write(uint8_t data) {
    TWDR = data;                                      // Load data
    TWCR = (1 << TWINT) | (1 << TWEN);                // Start transmission
    while (!(TWCR & (1 << TWINT)));                   // Wait for complete
}

uint8_t TWI_read_ack(void) {
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);  // Read with ACK
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}

uint8_t TWI_read_nack(void) {
    TWCR = (1 << TWINT) | (1 << TWEN);                // Read with NACK
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}

uint8_t TWI_get_status(void) {
    return TWSR & 0xF8; // Mask prescaler bits
}
