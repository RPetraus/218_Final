//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

#include "move_gate.h"

//=====[Declaration of private defines]========================================

/*
#define GATE_MOVE_DELAY_MS                   50
#define DUTY_MIN                             0.04
#define DUTY_MAX                             0.0735
#define PERIOD                               0.02
*/

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

DigitalOut greenLED(PE_14);
DigitalOut redLED(PE_15);
//PwmOut servo(PF_2);

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

//=====[Declarations (prototypes) of private functions]=========================

//=====[Implementations of public functions]===================================


void moveGateInit() {
    greenLED = ON;
    redLED = OFF;
    //servo.period(PERIOD);
    //servo.write(DUTY_MIN);
}

/*
void openGate() {
    redLED = ON;
    for (int i = 0; i < 10; i++) {
        servo.write(DUTY_MIN + ((DUTY_MAX - DUTY_MIN) / 10) * i);
        delay(GATE_MOVE_DELAY_MS);
    }
    redLED = OFF;
    greenLED = ON;
}

void closeGate() {
    greenLED = OFF;
    redLED = ON;
    for (int i = 0; i < 10; i++) {
        servo.write(DUTY_MAX - ((DUTY_MAX - DUTY_MIN) / 10) * i);
        delay(GATE_MOVE_DELAY_MS);
    }
    redLED = OFF;
}
*/

//=====[Implementations of private functions]==================================