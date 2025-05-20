#include <avr/io.h>
#include <avr/interrupt.h>

class IO{

    private:

        volatile uint8_t last_state;

        void initLEDs(); // Initialize LEDs
        void initButtons(); // Initialize buttons
        void toggleLED(unsigned int led); // Toggle LED
        void setLED(uint8_t led); // Set LED

        void attacthINTERUPT_PCINT1(); // Attach interrupt for Pin Change Interrupt 1
        void detachINTERUPT_PCINT1(); // Detach interrupt for Pin Change Interrupt 1

    public:

        IO();
        void initIO();
        int buttonUpdate();
        void controlLEDs(uint8_t led, bool pattern = false); // toggle a specific LED or set a specific LED pattern(set pattern to true)

};