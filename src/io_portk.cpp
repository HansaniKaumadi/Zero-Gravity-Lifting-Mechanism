#include "IO_PORTK.h"

IO_PortK::IO_PortK() {
    // Constructor
}

void IO_PortK::initLEDs() {
    // Set PK4, PK5, PK6, PK7 as output for LEDs
    DDRK |= (1 << PK4) | (1 << PK5) | (1 << PK6) | (1 << PK7); // LED pins
    PORTK &= ~((1 << PK4) | (1 << PK5) | (1 << PK6) | (1 << PK7)); // Set all LEDs off
}

void IO_PortK::initButtons() {
    // Set PK0, PK1, PK2, PK3 as input for buttons
    DDRK &= ~((1 << PK0) | (1 << PK1) | (1 << PK2) | (1 << PK3)); // Button pins
    PORTK &= ~((1 << PK0) | (1 << PK1) | (1 << PK2) | (1 << PK3)); // Disable pull-ups on buttons

    // Attach Pin Change Interrupt for PK0 to PK3
    attachInterrupt_PCINTK();
    PCMSK2 |= (1 << PCINT16) | (1 << PCINT17) | (1 << PCINT18) | (1 << PCINT19); // Enable interrupts on PK0 to PK3
}

void IO_PortK::attachInterrupt_PCINTK() {
    // Enable Pin Change Interrupts for Port K (PK0 to PK7)
    PCICR |= (1 << PCIE2);  // Enable pin change interrupt group 2 (PK0-PK7)
}

void IO_PortK::detachInterrupt_PCINTK() {
    // Disable Pin Change Interrupts for Port K (PK0 to PK7)
    PCICR &= ~(1 << PCIE2);  // Disable pin change interrupt group 2
}

void IO_PortK::initIO() {
    // Initialize LEDs
    initLEDs();

    // Initialize buttons
    initButtons();
}

int IO_PortK::buttonUpdate() {
    // Read the state of the buttons (PK0 to PK3)

    interrupt_state = false; // Set the interrupt state to false
    if(millis() - last_press_time < 100) {
        // Debounce time check
        return 0; // No button pressed
    }
    last_press_time = millis(); // Update last press time

    uint8_t current = PINK & ((1 << PK0) | (1 << PK1) | (1 << PK2) | (1 << PK3)); // Read PK0 to PK3
    uint8_t changed = current ^ last_state;
    last_state = current;
    
    int value = 0;

    if (changed & (1 << PK0)) {
        if (current & (1 << PK0)) {
            // Button on PK0 pressed (rising edge detected)
            value = 1;
        } else {
            // Button on PK0 released (falling edge detected)
            value = -1;
        }
    } else if (changed & (1 << PK1)) {
        if (current & (1 << PK1)) {
            // Button on PK1 pressed (rising edge detected)
            value = 2;
        } else {
            // Button on PK1 released (falling edge detected)
            value = -2;
        }
    } else if (changed & (1 << PK2)) {
        if (current & (1 << PK2)) {
            // Button on PK2 pressed (rising edge detected)
            value = 3;
        } else {
            // Button on PK2 released (falling edge detected)
            value = -3;
        }
    } else if (changed & (1 << PK3)) {
        if (current & (1 << PK3)) {
            // Button on PK3 pressed (rising edge detected)
            value = 4;
        } else {
            // Button on PK3 released (falling edge detected)
            value = -4;
        }
    }
    return value; // No button pressed
}


void IO_PortK::setLED(uint8_t pattern) {
    // Set the LEDs based on the pattern (4 LEDs on PK4 to PK7)
    PORTK = (PORTK & 0x0F) | ((pattern & 0x0F) << 4);
}

void IO_PortK::toggleLED(unsigned int led_number) {
    // Toggle the specific LED
    if (led_number >= 1 && led_number <= 4) {
        PORTK ^= (1 << (PK4 + led_number - 1));  // Toggle LED4 to LED7
    }
}

void IO_PortK::controlLEDs(uint8_t led, bool pattern) {
    // Control LEDs, either toggle or set to a pattern
    if (pattern) {
        setLED(led);
    } else {
        toggleLED(led);
    }
}
