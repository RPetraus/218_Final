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
#define POTENTIOMETER_MIN_INT_MED_LEVEL      0.33
#define POTENTIOMETER_MIN_INT_LONG_LEVEL     0.66
#define WIPER_HI_DELAY_MS 28
#define WIPER_LO_DELAY_MS 37
#define WIPER_INT_SHORT_DELAY_MS 1200
#define WIPER_INT_MED_DELAY_MS 2750
#define WIPER_INT_LONG_DELAY_MS 4750
#define DUTY_MIN 0.04
#define DUTY_MAX 0.0735
#define PERIOD 0.02
#define MOTOR_UPDATE_TIME_MS 20

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

AnalogIn intMode(A0);
AnalogIn wiperMode(A1);
PwmOut servo(PF_9);

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============
WiperMode_t currentWiperMode = WIPER_OFF;

IntDelayTime_t currentIntDelay = INT_LONG;

float wiperModeReading = 0.0;

float intModeReading = 0.0;

//=====[Declaration and initialization of private global variables]============

static bool wiperMinToMax = true;

typedef enum {
   WIPER_MIN_TO_MAX,
   WIPER_MAX_TO_MIN
} WiperDirection_t;

WiperDirection_t currentWiperDirection = WIPER_MIN_TO_MAX;

//=====[Declarations (prototypes) of private functions]=========================
static void runIntermittentWipers();
static void wiperReadUpdateMode();
static void windshieldWiperRunWipers();
static void intTypeReadUpdateMode();


static void rotateWiper(float wiperDelay);
static void intDelayWiper();
static void intDelayDisplayUpdateWiper(int delayTime, int iterations);


//=====[Implementations of public functions]===================================

void windshieldWiperInit() {
    userDisplayInit();
    servo.period(PERIOD);
    servo.write(DUTY_MIN);
}

void windshieldWiperUpdate() {
    if ( engineRunning ) {
        windshieldWiperRunWipers();
        wiperReadUpdateMode();
        intTypeReadUpdateMode();
    } else if (!engineRunning) {
        servo.write(DUTY_MIN);
    }
}

WiperMode_t windshieldWiperMode() {
    return currentWiperMode;
}

IntDelayTime_t intDelayType() {
    return currentIntDelay;
}
// SEPARATE PRIVATE AND PUBLIC FUNCS
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

static void intTypeReadUpdateMode() {
   intModeReading = intMode.read();

    if ( intModeReading >= POTENTIOMETER_MIN_INT_LONG_LEVEL ) {
        currentIntDelay = INT_LONG;
    } else if ( intModeReading < POTENTIOMETER_MIN_INT_LONG_LEVEL &&
                intModeReading >= POTENTIOMETER_MIN_INT_MED_LEVEL ) {
        currentIntDelay = INT_MED;
    } else {
        currentIntDelay = INT_SHORT;
    }
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
        rotateWiper(WIPER_LO_DELAY_MS);
    } else if (currentWiperMode == WIPER_OFF) {
        servo.write(DUTY_MIN); // MOVES RLLY FAST LIKE FASTER THAN HI
        return;
    }
}

static void intDelayWiper() {
    switch ( currentIntDelay ) {
        case INT_LONG:
            intDelayDisplayUpdateWiper(WIPER_INT_LONG_DELAY_MS, 25);
            break;
        
        case INT_MED:
            intDelayDisplayUpdateWiper(WIPER_INT_MED_DELAY_MS, 25);
            break;

        case INT_SHORT:
            intDelayDisplayUpdateWiper(WIPER_INT_SHORT_DELAY_MS, 10);
            break;
    }

}

static void intDelayDisplayUpdateWiper(int delayTime, int iterations) {
    for (int i = 0; i < iterations; i++) {
        delay(delayTime / iterations);
        wiperReadUpdateMode();
        intTypeReadUpdateMode();
        userDisplayUpdate();
    }
}


static void rotateWiper(float wiperDelay) {
    static int accumulatedWiperTime = 0;
    accumulatedWiperTime = accumulatedWiperTime + 10; // how long is actually taking before we're counting up to MOTOR_UPDATE_TIME_MS. should only be 20 ms
    if (accumulatedWiperTime >= MOTOR_UPDATE_TIME_MS) {
        accumulatedWiperTime = 0;

        switch ( currentWiperDirection ) {
            case WIPER_MIN_TO_MAX:
                for (int i = 0; i < 10; i++) {
                    servo.write(DUTY_MIN + ((DUTY_MAX - DUTY_MIN) / 10) * i);
                    delay(wiperDelay);
                    ignitionSubsystemUpdate();
                }

                currentWiperDirection = WIPER_MAX_TO_MIN;
                break;

            case WIPER_MAX_TO_MIN:
                for (int i = 0; i < 10; i++){
                    servo.write(DUTY_MAX - ((DUTY_MAX - DUTY_MIN) / 10) * i);
                    delay(wiperDelay);
                    ignitionSubsystemUpdate();
                }
                if (currentWiperMode == WIPER_INT) {
                    intDelayWiper();
                }

                currentWiperDirection = WIPER_MIN_TO_MAX;
                break;

        }

    }
}

void windshieldWiperStop() {
    currentWiperMode = WIPER_OFF;
    userDisplayUpdate();
}