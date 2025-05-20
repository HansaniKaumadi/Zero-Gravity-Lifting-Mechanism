# include "IO.h"

IO::IO() {
    // Constructor
}

void IO::initButtons() {
    // Set PJ0–PJ3 as input
    DDRJ &= ~((1 << PJ0) | (1 << PJ1) | (1 << PJ2) | (1 << PJ3)); // SELECT, DOWN, UP ,MENU 

    // Enable Pin Change Interrupt for Port J
    attacthINTERUPT_PCINT1();
    PCMSK1 |= (1 << PCINT9) | (1 << PCINT10) | (1 << PCINT11) | (1 << PCINT12);  // Enable specific pins
}

void IO::initLEDs(){

    DDRJ |= (1 << PJ4) | (1 << PJ5) | (1 << PJ6) | (1 << PJ7); // LED4, LED3, LED2, LED1
    // Set LEDs to low
    PORTJ &= ~((1 << PJ4) | (1 << PJ5) | (1 << PJ6) | (1 << PJ7)); // All LEDs off 

}

void IO::initIO() {
    // Initialize LEDs
    initLEDs();

    // Initialize buttons
    initButtons();
}

void IO::attacthINTERUPT_PCINT1() {
    PCICR |= (1 << PCIE1);         // Enable Pin Change Interrupt for Port J
}

void IO::detachINTERUPT_PCINT1() {
    PCICR &= ~(1 << PCIE1);        // Disable Pin Change Interrupt for Port J
}


int IO::buttonUpdate() {
    detachINTERUPT_PCINT1();
    // Read the state of the buttons
    uint8_t current = PINJ & ((1 << PJ0) | (1 << PJ1) | (1 << PJ2) | (1 << PJ3));
    uint8_t changed = current ^ last_state;
    last_state = current;

    if (changed & (1 << PJ0)) {
        if (current & (1 << PJ0)) {
            // SELECT button pressed and rising edge detected
            attacthINTERUPT_PCINT1();
            return 1;
        }else {
            // SELECT button released and falling edge detected
            return -1;
        }
    }else if (changed & (1 << PJ1)) {
        if (current & (1 << PJ1)) {
            // DOWN button pressed and rising edge detected
            return 2;
        }else {
            // DOWN button released and falling edge detected
            return -2;
        }
    }else if (changed & (1 << PJ2)) {
        if (current & (1 << PJ2)) {
            // UP button pressed and rising edge detected
            return 3;
        }else {
            // UP button released and falling edge detected
            return -3;
        }
    }else if (changed & (1 << PJ3)) {   
        if (current & (1 << PJ3)) {
            // MENU button pressed and rising edge detected
            return 4;
        }else {
            // MENU button released and falling edge detected
            return -4;
        }
    }
    return 0; // No button pressed
}


void IO::setLED(uint8_t pattern) {
    // Set the LEDs based on the pattern
    PORTJ = (PORTJ & 0x0F) | ((pattern & 0x0F) << 4);

}

void IO::toggleLED(unsigned int led_number) {
    // Toggle the specified LED
    if (led_number >= 1 && led_number <= 4) {
        PORTJ ^= (1 << (8 - led_number));  // 8 - led_number gives PJ7 to PJ4
    }
}

void IO::controlLEDs(uint8_t led, bool pattern) {
    // Control the LEDs based on the pattern
    if (pattern) {
        setLED(led);
    } else {
        toggleLED(led);
    }
}