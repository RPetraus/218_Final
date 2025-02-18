//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"
#include "windshield_wiper_subsystem.h"
#include "user_display.h"
#include "ignition_subsystem.h"



//=====[Declaration of private defines]========================================

#define POTENTIOMETER_MIN_WIPER_HI_LEVEL     0.75
#define POTENTIOMETER_MIN_WIPER_LO_LEVEL     0.50
#define POTENTIOMETER_MIN_WIPER_INT_LEVEL    0.25
#define DUTY_MIN 0.04
#define DUTY_MAX 0.0735
#define PERIOD 0.02
#define TIME_INCREMENT_MS 1

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

AnalogIn wiperMode(A1);
PwmOut servo(PF_9);

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============
WiperMode_t currentWiperMode = WIPER_OFF;

float wiperModeReading = 0.0;

//=====[Declaration and initialization of private global variables]============

static bool wiperMinToMax = true;

//=====[Declarations (prototypes) of private functions]=========================
static void stopWipers();
static void runIntermittentWipers();
static void wiperReadUpdateMode();
static void windshieldWiperRunWipers();


static void rotateWiper();


//=====[Implementations of public functions]===================================

void windshieldWiperInit() {
    userDisplayInit();
    servo.period(PERIOD);
    servo.write(DUTY_MIN);
}

void windshieldWiperModeUpdate() {
    if ( engineRunning ) { //and user has picked windshield wiper mode
        wiperReadUpdateMode();
        windshieldWiperRunWipers();
    }
}

WiperMode_t windshieldWiperMode() {
    return currentWiperMode;
}
/*
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
*/

static void wiperReadUpdateMode() {
   wiperModeReading = wiperMode.read();

    if ( wiperModeReading >= POTENTIOMETER_MIN_WIPER_HI_LEVEL ) {
        currentWiperMode = WIPER_HI;
    } else if ( wiperModeReading < POTENTIOMETER_MIN_WIPER_HI_LEVEL &&
                wiperModeReading >= POTENTIOMETER_MIN_WIPER_LO_LEVEL ) {
        currentWiperMode = WIPER_LO;
    } else if ( wiperModeReading < POTENTIOMETER_MIN_WIPER_LO_LEVEL &&
                wiperModeReading >= POTENTIOMETER_MIN_WIPER_INT_LEVEL ) {
        currentWiperMode = WIPER_INT;   
    } else {
        currentWiperMode = WIPER_OFF;
    }
}


static void windshieldWiperRunWipers() {
    static float currentAngle = 0.0;
    static bool increasing = true;


    float dutyCycle = DUTY_MIN;
    if (currentWiperMode == WIPER_HI) {
        servo.write(DUTY_MAX);
        //rotateWiper();
    } else if (currentWiperMode == WIPER_LO) {
        //dutyCycle = DUTY_MIN + (DUTY_MAX - DUTY_MIN) * 0.5;
        servo.write(DUTY_MIN);
    } else if (currentWiperMode == WIPER_INT) {
        servo.write(DUTY_MIN);
    } else if (currentWiperMode == WIPER_OFF) {
        servo.write(DUTY_MIN);
        return;
    }

    if (increasing) {
        currentAngle += 1.0;
        if (currentAngle >= 67.0) {
            currentAngle = 0.0;
            increasing = true;
        }
    }

    //dutyCycle = DUTY_MIN + (DUTY_MAX - DUTY_MIN) * (currentAngle / 67.0); ????
    //servo.write(dutyCycle);
}


static void rotateWiper() {
    static int accumulatedWiperTime = 0;

    if (wiperMinToMax) {
        for (int i = 0; i < 100; i++){
            servo.write(DUTY_MIN + ((DUTY_MAX - DUTY_MIN) / 100) * i);
            accumulatedWiperTime += TIME_INCREMENT_MS;
            if (accumulatedWiperTime == 10) {
            }
        }
        wiperMinToMax = false;
    } else if (!wiperMinToMax) {
        for (int i = 0; i < 100; i++){
            servo.write(DUTY_MAX - ((DUTY_MAX - DUTY_MIN) / 100) * i);
            
        }
        wiperMinToMax = true;
    }
    accumulatedWiperTime = 0;
}

/*
static void windshieldWiperMove() {
    switch ( windshieldWiperMode() ) {
        case WIPER_HI:
            servo.write(DUTY_MIN);
            delay( TIME_INCREMENT_MS );
            servo.write(DUTY_MAX);
            delay( TIME_INCREMENT_MS );
            break;
        case WIPER_LO:
            servo.write(DUTY_MIN);
            delay( TIME_INCREMENT_MS );
            servo.write(DUTY_MAX);
            delay( TIME_INCREMENT_MS );
            break;

        case WIPER_INT: 
            servo.write(DUTY_MIN);
            delay( TIME_INCREMENT_MS );
            servo.write(DUTY_MAX);
            delay( TIME_INCREMENT_MS );
            break;

        case WIPER_OFF:
            servo.write(DUTY_MIN);
            delay( TIME_INCREMENT_MS );
            servo.write(DUTY_MAX);
            delay( TIME_INCREMENT_MS );
            break;
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
