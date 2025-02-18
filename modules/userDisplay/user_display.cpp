//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

#include "user_display.h"
#include "display.h"
#include "ignition_subsystem.h"
#include "smart_car_system.h"
#include "windshield_wiper_subsystem.h"

//=====[Declaration of private defines]========================================

#define DISPLAY_REFRESH_TIME_MS 1000
#define POTENTIOMETER_MIN_WIPER_HI_LEVEL     0.75
#define POTENTIOMETER_MIN_WIPER_LO_LEVEL     0.50
#define POTENTIOMETER_MIN_WIPER_INT_LEVEL     0.25

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

//=====[Declarations (prototypes) of private functions]========================

//=====[Implementations of public functions]===================================

void userDisplayInit()
{
    displayInit();
    engineRunning = true;///////////////////asdglkhsakdfrik get rid of this when integrating the ignition back
    displayCharPositionWrite ( 0,0 );
    displayStringWrite( "Wiper Mode:" );
}

void userDisplayUpdate()
{
    static int accumulatedDisplayTime = 0;
    
    if (accumulatedDisplayTime >= DISPLAY_REFRESH_TIME_MS) {

        accumulatedDisplayTime = 0;

        //wiperUpdateMode();

        if (engineRunning) {
                displayCharPositionWrite(11, 0);
                switch ( windshieldWiperMode() ) {
                    case WIPER_HI:
                        displayStringWrite("HI ");
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
            }
    } else {
        accumulatedDisplayTime = accumulatedDisplayTime + TIME_INCREMENT_MS;
    } 
}

//=====[Implementations of private functions]==================================




