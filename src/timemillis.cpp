#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned long millis_counter = 0;  // Milliseconds counter
unsigned long last_time = 0; // Last time in milliseconds


ISR(TIMER0_COMPA_vect)
{
    millis_counter++; // 1 ms has passed
}

void timer2_ctc_100hz_init(void)
{
    TCCR2A = (1 << WGM21);                            // CTC mode
    TCCR2B = (1 << CS20) | (1 << CS21) | (1 << CS22); // 1024

    TIMSK2 = (1 << OCIE2A); // Enable Timer2 compare interrupt

    // (F_CPU / (Prescaler * Frequency)) - 1 = (16e6 / (1024 * 125)) - 1 = 124
    OCR2A = 124; // Set compare value for 125 Hz
}


void millis_init(void)
{
    // Set Timer0 to normal mode, prescaler = 64
    // Set Timer0 to CTC mode
    TCCR0A = (1 << WGM01);              // CTC mode (OCR0A as top)
    TCCR0B = (1 << CS01) | (1 << CS00); // Prescaler 64

    OCR0A = 249; // (16MHz / 64 / 1000) - 1 = 249

    TIMSK0 |= (1 << OCIE0A); // Enable Compare Match A interrupt

    sei(); // Enable global interrupts
}

unsigned long millis(void)
{
    unsigned long ms;
    cli();
    ms = millis_counter;
    sei();
    return ms;
}

bool every_5_seconds()
{
    unsigned long now = millis();
    if (now - last_time >= 5000)
    {
        last_time = now;
        return true;
    }
    return false;
}
