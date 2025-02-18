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
#define WIPER_HI_DELAY_MS 28
#define WIPER_LO_DELAY_MS 37
#define WIPER_INT_DELAY 0
#define DUTY_MIN 0.04
#define DUTY_MAX 0.0735
#define PERIOD 0.02
#define MOTOR_UPDATE_TIME_MS 20

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

AnalogIn wiperMode(A1);
PwmOut servo(PF_9);

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============
WiperMode_t currentWiperMode = WIPER_OFF;

IntDelayTime_t currentIntDelay = INT_SHORT;

float wiperModeReading = 0.0;

//=====[Declaration and initialization of private global variables]============

static bool wiperMinToMax = true;

typedef enum {
   WIPER_MIN_TO_MAX,
   WIPER_MAX_TO_MIN
} WiperDirection_t;

WiperDirection_t currentWiperDirection = WIPER_MIN_TO_MAX;

//=====[Declarations (prototypes) of private functions]=========================
static void stopWipers();
static void runIntermittentWipers();
static void wiperReadUpdateMode();
static void windshieldWiperRunWipers();


static void rotateWiper(float wiperDelay);


//=====[Implementations of public functions]===================================

void windshieldWiperInit() {
    userDisplayInit();
    servo.period(PERIOD);
    servo.write(DUTY_MIN);
}

void windshieldWiperModeUpdate() {
    if ( engineRunning ) { //and user has picked windshield wiper mode
        windshieldWiperRunWipers();
        wiperReadUpdateMode();
        
    }
}

WiperMode_t windshieldWiperMode() {
    return currentWiperMode;
}

IntDelayTime_t intDelayType() {
    return currentIntDelay;
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
    userDisplayUpdate();
}


static void windshieldWiperRunWipers() {
    static float currentAngle = 0.0;
    static bool increasing = true;


    float dutyCycle = DUTY_MIN;
    if (currentWiperMode == WIPER_HI) {
        rotateWiper(WIPER_HI_DELAY_MS);
    } else if (currentWiperMode == WIPER_LO) {
        rotateWiper(WIPER_LO_DELAY_MS);
    } else if (currentWiperMode == WIPER_INT) {
        servo.write(DUTY_MIN);
    } else if (currentWiperMode == WIPER_OFF) {
        servo.write(DUTY_MIN);
        return;
    }
}


static void rotateWiper(float wiperDelay) {
    static int accumulatedWiperTime = 0;
    accumulatedWiperTime = accumulatedWiperTime + 10;
    if (accumulatedWiperTime >= MOTOR_UPDATE_TIME_MS) {
        accumulatedWiperTime = 0;

        switch ( currentWiperDirection ) {
            case WIPER_MIN_TO_MAX:
                for (int i = 0; i < 10; i++){
                    servo.write(DUTY_MIN + ((DUTY_MAX - DUTY_MIN) / 10) * i);
                    delay(wiperDelay);
                }

                currentWiperDirection = WIPER_MAX_TO_MIN;
                break;

            case WIPER_MAX_TO_MIN:
                for (int i = 0; i < 10; i++){
                    servo.write(DUTY_MAX - ((DUTY_MAX - DUTY_MIN) / 10) * i);
                    delay(wiperDelay);
                }

                currentWiperDirection = WIPER_MIN_TO_MAX;
                break;

        }

    }
}

void windshieldWiperStop() {
    currentWiperMode = WIPER_OFF;
    
    
    // MAYBE CAN CALL A FUNCTION TO DO THIS
    userDisplayUpdate(); //USERDISPLAYUPDATE
    servo.write(DUTY_MIN);
}

/*
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
*/
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