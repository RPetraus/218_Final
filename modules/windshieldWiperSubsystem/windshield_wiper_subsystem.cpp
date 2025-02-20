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
#define WIPER_HI_DELAY_MS                    28
#define WIPER_LO_DELAY_MS                    37
#define WIPER_INT_SHORT_DELAY_MS             1200
#define WIPER_INT_MED_DELAY_MS               2750
#define WIPER_INT_LONG_DELAY_MS              4750
#define DUTY_MIN                             0.04
#define DUTY_MAX                             0.0735
#define PERIOD                               0.02
#define MOTOR_UPDATE_TIME_MS                 20

//=====[Declaration of private data types]=====================================

typedef enum {
   WIPER_MIN_TO_MAX,
   WIPER_MAX_TO_MIN
} WiperDirection_t;

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
WiperDirection_t currentWiperDirection = WIPER_MIN_TO_MAX;

//=====[Declarations (prototypes) of private functions]=========================
static void wiperReadUpdateMode();
static void windshieldWiperRunWipers();
static void intTypeReadUpdateMode();
static void rotateWiper(float wiperDelay);
static void intDelayWiper();
static void intDelayDisplayUpdateWiper(int delayTime, int iterations);


//=====[Implementations of public functions]===================================

/**
* Initializes the windshield wiper system by setting up user display and 
* positional servo motor
*/
void windshieldWiperInit() {
    userDisplayInit();
    servo.period(PERIOD);
    servo.write(DUTY_MIN);
}


/**
* Updates the windshield wiper system if the engine is on. If it is off, the wipers
* remain off.
*/
void windshieldWiperUpdate() {
    if ( engineRunning ) {
        windshieldWiperRunWipers();
        wiperReadUpdateMode();
        intTypeReadUpdateMode();
    } else if (!engineRunning) {
        servo.write(DUTY_MIN);
    }
}

/**
* Returns the current wiper mode
*/
WiperMode_t windshieldWiperMode() {
    return currentWiperMode;
}


/**
* Returns the current intermittent delay type
*/
IntDelayTime_t intDelayType() {
    return currentIntDelay;
}

void windshieldWiperStop() {
    currentWiperMode = WIPER_OFF;
    userDisplayUpdate();
}

//=====[Implementations of private functions]==================================
// Updates the wiper mode based on potentiometer value
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

//Updates the intermittent delay type based on the potentiometer value
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

//Runs the wipers according to the current wiper mode
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
        windshieldWiperStop();
        servo.write(DUTY_MIN); // MOVES RLLY FAST LIKE FASTER THAN HI
        return;
    }
}

//Applies the delay for intermittent wiper mode
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

//Updates the display while waiting for int delay to pass
static void intDelayDisplayUpdateWiper(int delayTime, int iterations) {
    for (int i = 0; i < iterations; i++) {
        delay(delayTime / iterations);
        ignitionSubsystemUpdate();
        wiperReadUpdateMode();
        intTypeReadUpdateMode();
        userDisplayUpdate(); 
    }

}

//Rotates the wiper based on the position of the servo motor and duty cycle
static void rotateWiper(float wiperDelay) {
    static int accumulatedWiperTime = 0;
    accumulatedWiperTime = accumulatedWiperTime + 10; // how long is actually taking before we're counting up to MOTOR_UPDATE_TIME_MS. should only be 20 ms
    if (accumulatedWiperTime >= MOTOR_UPDATE_TIME_MS) {
        accumulatedWiperTime = 0;

        switch ( currentWiperDirection ) {
            case WIPER_MIN_TO_MAX:
                for (int i = 0; i < 10; i++) {
                    servo.write(DUTY_MIN + ((DUTY_MAX - DUTY_MIN) / 10) * i);
                   // pcSerialComStringWrite("Start.\r\n");
                    delay(wiperDelay);
                    //pcSerialComStringWrite("end.\r\n");
                    ignitionSubsystemUpdate();
                }

                currentWiperDirection = WIPER_MAX_TO_MIN;
                break;

            case WIPER_MAX_TO_MIN:
                for (int i = 0; i < 10; i++){
                    servo.write(DUTY_MAX - ((DUTY_MAX - DUTY_MIN) / 10) * i);
                    //pcSerialComStringWrite("start 2\r\n");
                    delay(wiperDelay);
                    //pcSerialComStringWrite("wnd 2.\r\n");
                    ignitionSubsystemUpdate();
                }
                if (currentWiperMode == WIPER_INT) {
                    //pcSerialComStringWrite("Start.\r\n");
                    intDelayWiper();
                }

                currentWiperDirection = WIPER_MIN_TO_MAX;
                break;

        }

    }
}