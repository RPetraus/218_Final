/*
#include "mbed.h"
#include "arm_book_lib.h"
#include "windshield_wiper_subsystem.h"
#include "user_display.h"



//=====[Defines]===============================================================
#define POTENTIOMETER_MAX_HIGH_MODE     0.75
#define POTENTIOMETER_MAX_LO_MODE       0.50
#define POTENTIOMETER_MAX_INT_MODE      0.25


typedef enum{
   OFF,
   HIGH,
   LO,
   INT
} wiperMode_t;


wiperMode_t currentWiperMode = OFF;
float lastWiperChangeTime = 0;
float wiperDelayTime = 0;



//=====[Declaration of public data types]======================================


//=====[Declaration and initialization of public global objects]===============
AnalogIn wiperModePin(D10); //change to an available pin
AnalogIn wiperDelayMode(A1); //change to an available pin




//=====[Declaration and initialization of public global variables]=============


//=====[Declarations (prototypes) of public functions]=========================
static void stopWipers();
static void runIntermittentWipers();
static void updateWiperMode(int wiperModeValue);


//=====[Implementations of public functions]===================================

void handleWiperControl(int wiperModeValue, int delayTimeValue) {
    if (engineRunning()) {
        updateWiperMode(wiperModeValue);
        updateDelayTime(delayTimeValue);
            if (currentWiperMode == HIGH || currentWiperMode == LO) {
      runWipers(currentWiperMode);
    } else if (currentWiperMode == INT) {
      runIntermittentWipers();
    } else {
      stopWipers();
    }
  } else {
    stopWipers();
  }
}

static void stopWipers() {
    //write! (private)
}

static void runIntermittentWipers() {
    //write! (private)
}

static void updateWiperMode(int wiperModeValue) {
  if (wiperModeValue < POTENTIOMETER_MAX_HIGH_MODE) {
    currentWiperMode = HIGH;
  } else if (wiperModeValue < POTENTIOMETER_MAX_LO_MODE) {
    currentWiperMode = LO;
  } else if (wiperModeValue < POTENTIOMETER_MAX_INT_MODE) {
    currentWiperMode = INT;
  } else {
    currentWiperMode = OFF;
  }
}
*/




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
