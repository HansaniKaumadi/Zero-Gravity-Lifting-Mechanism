# include "motor.h"

motor::motor(unsigned long microstep) : microstep(microstep) {
}

void motor::
initMotor() {
    initPWM_TIM1();
    initCounter_TIM5();
    // Set PH2 to PH3 as output
    DDRH |= (1 << PH6) | (1 << PH5) | (1 << PH4) | (1 << PH3) ; // DIR+, DIR-, ENA+, ENA-

    // Optionally set them low or high
    PORTH &= ~((1 << PH6) | (1 << PH5) | (1 << PH4) | (1 << PH3)); // All low

}

void motor::DISmotor() {
    PORTH = (PORTH & ~(1 << PH3)) | (1 << PH4); // Set EN+ high, EN- low
}

void motor::ENmotor() {
    PORTH = (PORTH & ~(1 << PH4)) | (1 << PH3); // Set EN- low, EN+ high
}

void motor::chanageDIR_ccw() {
    PORTH = (PORTH & ~(1 << PH6)) | (1 << PH5); // Set DIR+ low, DIR- high
}

void motor::chanageDIR_cw() {
    PORTH = (PORTH & ~(1 << PH5)) | (1 << PH6); // Set DIR+ high, DIR- low
}

void motor::initPWM_TIM1() {
    DDRB |= (1 << PB5);    // Set OC1A (PB5) as output

    // Set CTC mode: WGM12 = 1 (Mode 4), toggle OC1A on match
    TCCR1A = (1 << COM1A0);                     // Toggle OC1A on compare match
    TCCR1B = (1 << WGM12) | (1 << CS11);        // CTC mode, Prescaler = 8

    // Calculate and set OCR1A for desired frequency
}

void motor::initCounter_TIM5() {

    DDRL &= ~(1 << PL2);      // Set T5 (PL2) as input

    // Clear Timer on Compare Match mode (CTC)
    TCCR5B = (1 << WGM52);  // CTC with OCR5A

    // Clear timer
    TCNT5 = 0;

    sei();  // Enable global interrupts
}

void motor::stopCounter_TIM5() {
    TCCR5B = 0; // Stop counter
}

void motor::stopPWM_TIM1() {
    TCCR1A = 0; // Stop PWM
    OCR1A = 0; // Reset compare value
}

void motor::startCounter_TIM5() {
    TCCR5B |= (1 << CS52) | (1 << CS51) | (1 << CS50); // Start counter
}

void motor::startPWM_TIM1() {
    TCCR1A |= (1 << COM1A0); // Start PWM
}

void motor::resetCounter_TIM5() {
    TCNT5 = 0; // Reset counter
    OCR5A = 0; // Reset compare value
}

void motor::resetPWM_TIM1() {
    OCR1A = 0; // Reset compare value
}

void motor::detachINTERUPT_TIM5() {
    TIMSK5 &= ~(1 << OCIE5A); // Disable interrupt
}

void motor::attachINTERUPT_TIM5() {
    TIMSK5 |= (1 << OCIE5A); // Enable interrupt
}


void motor::setSpeed(unsigned int rpm) {
    if (rpm > 1000) {
        rpm = 1000;
    }

    uint32_t frequency_hz = rpm * microstep / 60; // Convert RPM to frequency in Hz

     // Calculate OCR1A value
     uint32_t ocr_val = (F_CPU / (2 * PRESCALER * frequency_hz)) - 1;
     if (ocr_val > 65535) {
         ocr_val = 65535; // Limit to 16-bit value
     }
 
    OCR1A = ocr_val; // Set the PWM duty cycle

}

void motor::setDirection(bool dir) {
    if (dir) {
        chanageDIR_cw(); // Set direction to clockwise
    } else {
        chanageDIR_ccw(); // Set direction to counterclockwise
    }
    direction = dir; // Update direction variable
}

void motor::setAngle(unsigned int angle) {

    resetCounter_TIM5(); // Reset counter
    uint32_t count = angle * microstep / 360; // Convert angle to count
    if(count > 65536) { // 2^16 
        count = 65536; // Limit to 16-bit value
        //Error handling: angle too large
    }
    OCR5A = count - 1;  // Zero-indexed
    attachINTERUPT_TIM5(); // Attach interrupt
}

void motor::stopMotor() {
    stopCounter_TIM5(); // Stop counter
    resetCounter_TIM5(); // Reset counter
    stopPWM_TIM1(); // Stop PWM
    resetPWM_TIM1(); // Reset compare value
    detachINTERUPT_TIM5(); // Detach interrupt

    direction = 1; // Default direction
    running = false; // Set motor state to stopped
}

void motor::runMotor() {
    startCounter_TIM5(); // Start counter
    startPWM_TIM1(); // Start PWM
    running = true;
    // Set motor state to running
}

void motor::speedcontrol(int rpm) {

    if(rpm > 0 && direction == true) {
        setDirection(true); // Set direction to forward
    } else if(rpm < 0 && direction == false) {
        setDirection(false); // Set direction to backward
    }
    if(rpm < 0) {
        rpm = -rpm; // Make RPM positive
    }
    setSpeed(rpm); // Set speed

    if(!running){
        runMotor(); // Start motor if not already running
    }
}

void motor::turnAngle(int angle, unsigned int rpm) {
    if(angle < 0) {
        rpm = -rpm; // Make RPM negative for backward direction
        angle = -angle; // Make angle positive
    }
    setAngle(angle); // Set angle
    speedcontrol(rpm); // Set speed
}

double motor::getAngle() {
    unsigned long current_count = TCNT5; // Read the current counter value
    double angle = double(current_count * 360) / microstep; // Convert count to angle
    return angle; // Return the angle
}

void motor::resetAngle() {
    resetCounter_TIM5(); // Reset counter
    angle_set = false; // Reset angle set flag
}