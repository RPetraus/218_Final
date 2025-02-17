//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

#include "user_display.h"
#include "display.h"
#include "ignition_subsystem.h"
#include "smart_car_system.h"

//=====[Declaration of private defines]========================================

#define DISPLAY_REFRESH_TIME_MS 1000
#define POTENTIOMETER_MIN_WIPER_HI_LEVEL     0.75
#define POTENTIOMETER_MIN_WIPER_LO_LEVEL     0.50
#define POTENTIOMETER_MIN_WIPER_INT_LEVEL     0.25



//=====[Declaration of private data types]=====================================

AnalogIn wiperMode(A1);

//=====[Declaration and initialization of public global objects]===============

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

typedef enum {
   WIPER_HI,
   WIPER_LO,
   WIPER_INT,
   WIPER_OFF
} WiperMode_t;


WiperMode_t currentWiperMode = WIPER_OFF;

float wiperModeReading = 0.0;


//=====[Declarations (prototypes) of private functions]========================
static void wiperUpdateMode();

//=====[Implementations of public functions]===================================

void userDisplayInit()
{
    displayInit();
    engineRunning = true;///////////////////asdglkhsakdfrik
    displayCharPositionWrite ( 0,0 );
    displayStringWrite( "Wiper Mode: OFF" );
}

void userDisplayUpdate()
{
    static int accumulatedDisplayTime = 0;
    
    if (accumulatedDisplayTime >= DISPLAY_REFRESH_TIME_MS) {

        accumulatedDisplayTime = 0;

        if (engineRunning) {
            static WiperMode_t lastWiperMode = WIPER_OFF;  // Track the last wiper mode
            if (currentWiperMode != lastWiperMode) {  // Only update the display if the mode changed
                displayCharPositionWrite(0, 13); // Set the position to overwrite the previous string
                switch (currentWiperMode) {
                    case WIPER_HI:
                        displayStringWrite("HI "); // if issue persists just clear and redisplay all text
                        break;

                    case WIPER_LO:
                        displayStringWrite("LO ");
                        break;

                    case WIPER_INT: 
                        displayStringWrite("INT");
                        break;

                    case WIPER_OFF:
                        displayStringWrite("OFF");
                        break;
                }
                lastWiperMode = currentWiperMode;  // Update the last wiper mode
            }
            wiperUpdateMode(); // Update the wiper mode based on the potentiometer reading
        }

    } else {
        accumulatedDisplayTime = accumulatedDisplayTime + TIME_INCREMENT_MS;
    } 
}


/*
void userDisplayUpdate()
{
    static int accumulatedDisplayTime = 0;
    
    if( accumulatedDisplayTime >=
        DISPLAY_REFRESH_TIME_MS ) {

        accumulatedDisplayTime = 0;

        if ( engineRunning ) {
            static WiperMode_t lastWiperMode = WIPER_HI;
            if (currentWiperMode != lastWiperMode) {
                switch ( currentWiperMode ) {
                    case WIPER_HI:
                        displayCharPositionWrite ( 0,13 ); //changed here
                        displayStringWrite( "HI " );
                        break;

                    case WIPER_LO:
                        displayCharPositionWrite ( 0,13 ); //changed here
                        displayStringWrite( "LO " );
                        break;

                    case WIPER_INT: 
                        displayCharPositionWrite ( 0,13 ); //changed here
                        displayStringWrite( "INT" );
                        break;

                    case WIPER_OFF:
                        displayCharPositionWrite ( 0,13 ); //changed here
                        displayStringWrite( "OFF" );
                        break;
                        
                }
            lastWiperMode = currentWiperMode;
            }
            wiperUpdateMode();
        }

    } else {
        accumulatedDisplayTime =
            accumulatedDisplayTime + TIME_INCREMENT_MS;        
    } 
}
*/





//=====[Implementations of private functions]==================================



static void wiperUpdateMode() {
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



