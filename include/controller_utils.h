#include "motor.h"
#include "linearControl.h"
#include "ADS1232.h"
#include "IO.h"
#include "UART.h"
#include "timemillis.h"
#include "touchController.h"

#ifndef CONTROLLER_UTILS_H
#define CONTROLLER_UTILS_H

class ControllerUtil{

    private:
        IO *io;
        motor *stepper;
        LinearControl *handle_controller;
        ADS1232 *ads;
        TouchController *touchController; // Touch controller for crane control
        UART *uart;

        int *button = nullptr; // Pointer to button state

        char buffer[100]; // Buffer for formatted strings


        float linear_value = 0.0f; // Previous linear control value

        int speed = 0; // Previous speed value
        int prv_speed = 0; // Previous speed for controller mode

        const float SPEED_TO_RPM = 10.0f; // Convert TouchController speed to RPM (e.g., 100.0f -> 1000 RPM)


        void trapspeedcontrol(float speed, float dt); // Trapezoidal speed control for the motor


    public:

        ControllerUtil(IO *io, motor *stepper, LinearControl *handle_controller, ADS1232 *ads, TouchController* touchController, UART *uart, int *button = nullptr);
    
        void callibrateADS1232_weight(float known_weight = 2500.0f); // Callibrate ADS1232 with a known weight

        void handlLinearControl();
        void handleADS1232Control();
        void handleButtonControl();


};


#endif // CONTROLLER_UTILS_H