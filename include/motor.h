#include <avr/io.h>
#include <avr/interrupt.h>

/*
exmple usage:

1. init the motor with the number of microsteps: motor stepper(200); 200 microsteps this is set by 4 switches on the motor driver. and enable the motor with stepper.ENmotor();

2. its recomeneded to use getAngle() to get the turned angle and throgh a necessary logic to set the speedControl() for most reliable and predicable results.
    2.1 in speedControl() the motor runs continously and the speed can be changed at any time. the direction is determined by the sign of the rpm. 
    2.2 use stopMotor() to stop the motor and reset the counter and PWM. this will also detach the interrupts and reset all the variables that keep track of the motor state.
    2.3 getAngle() has a maximum(depends on microstep) if many rotations are passing it needs to handleed accordigly this feature might be added in future in to this class.
    2.4 resetAngle() will reset the counter but not attach or detach the interrupts. this is useful if you want to reset the angle without stopping the motor.

3.its recomended to use turnAngle() in a single shout manner
    3.1 turnAngle() will set the angle and attach the interrupts. this will make the motor turn the angle.
    3.2 the sign of the angle determines the direction of the motor.
    3.3 if this functions is called contiounsly the counters will be reset contimously and will turn indefinitely with given rpms.

*/

#ifndef MOTOR_H
#define MOTOR_H

#define PRESCALER 8
class motor{

private:
    int current_rpm = 0; // Speed of the motor 
    bool direction = 1; // Direction of the motor (true for forward, false for backward)
    bool running = false; // Motor state (true for running, false for stopped)

    bool int_status = false; // Interrupt flag for Timer5
    bool angle_set = false; // Flag to check if angle is set

    unsigned long microstep = 0; // Microstepping mode 
    int turns = 0; // Number of revolutions 

    void initPWM_TIM1();
    void initCounter_TIM5();

    void stopCounter_TIM5();
    void stopPWM_TIM1();

    void startCounter_TIM5();
    void startPWM_TIM1();

    void resetCounter_TIM5();
    void resetPWM_TIM1();

    void detachINTERUPT_TIM5(); // Detach interrupt for Timer5
    void attachINTERUPT_TIM5(); // Attach interrupt for Timer5

    void chanageDIR_cw(); // Change direction of the motor
    void chanageDIR_ccw(); // Change direction of the motor


    void setSpeed(unsigned int rpm);
    void setDirection(bool dir);
    void setAngle(unsigned int angle);
    void runMotor();

public:
    motor(unsigned long microstep);
    void ENmotor(); // Enable the motor
    void DISmotor(); // Disable the motor
    void initMotor(); // Initialize the motor
    void stopMotor(); // halt the motor ,reset the counter and PWM all all variables and registers that keep track of the motor state, and also detach the interrupts.
    void speedcontrol(int rpm); // Control the motor speed can be contiusly called to change the speed of the motor. sign of the rpm determines the direction of the motor. restart of the motor in handled in the function. but the interrupts are not attched and no angle is set.
    void turnAngle(int angle, unsigned int rpm); // similar to speedcontrol but the angle is set and the interrupts are attached. will ask the motor to turn the angle but not recomnend to call this function in a loop. sign of angle detrmines the direction
    double getAngle(); // Get the current angle of the motor
    void resetAngle(); // Reset the angle of the motor resets the counter but doesnt attch or deatch interrpts.

};




#endif