/*
#include "mbed.h"
#include "arm_book_lib.h"
#define DUTY_MIN 0.03  //Duty cycle is pulse_width/period
#define DUTY_MAX 0.1175
#define PERIOD 0.02 //Units of period is seconds
#define FULLY_STOP_DUTY 0.075
PwmOut servo(PF_9);  //Here is where we declare the object
int main() {
    servo.period(PERIOD);
    servo.write(DUTY_MIN);
    while (true) {
        servo.write(DUTY_MIN);
        delay(3000);
        servo.write(DUTY_MAX);
        delay(3000);
    }
}
*/