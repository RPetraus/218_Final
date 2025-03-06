#include "motor.h"

//=====[Declaration of private defines]========================================

#define GATE_MOVE_DELAY_MS                   50
#define DUTY_MIN                             0.04
#define DUTY_MAX                             0.0735
#define PERIOD                               0.02



PwmOut servo(PF_2);
//servo.period(PERIOD);
//servo.write(DUTY_MIN);


void moveGateInit() {
    greenLED = OFF;
    redLED = ON;
    
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