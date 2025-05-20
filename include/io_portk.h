#ifndef IO_PORTK_H
#define IO_PORTK_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include "timemillis.h"

class IO_PortK {
private:
    volatile uint8_t last_state;
    bool interrupt_state; // State of the interrupt

    unsigned long pressed_times[4]; // Array to store pressed times for each button

    unsigned long last_press_time; // Last time a button state was updated

    void initLEDs();                // Initialize LEDs on Port K
    void initButtons();             // Initialize buttons on Port K
    void toggleLED(unsigned int led); // Toggle LED
    void setLED(uint8_t led);       // Set LED pattern


public:
    IO_PortK();
    void initIO();                  // Initialize IO ports
    int buttonUpdate();        
    void attachInterrupt_PCINTK();  // Attach interrupt for Pin Change Interrupt on Port K
    void detachInterrupt_PCINTK(); // Detach interrupt for Pin Change Interrupt on Port K

    // Update button states and return actions
    void controlLEDs(uint8_t led, bool pattern = false);  // Control LED pattern (toggle/set)
};

#endif // IO_PORTK_H
