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
    displayCharPositionWrite ( 0,0 );
    displayStringWrite( "Wiper Mode:OFF" );
    displayCharPositionWrite ( 0,1 );
    displayStringWrite( "                 ");
}

void userDisplayUpdate()
{
    if (engineRunning) {
        displayCharPositionWrite(11, 0);
        switch ( windshieldWiperMode() ) {
            case WIPER_HI:
                displayStringWrite("HI ");
                displayCharPositionWrite( 0,1 );
                displayStringWrite("                 ");
                break;

            case WIPER_LO:
                displayStringWrite("LO ");
                displayCharPositionWrite( 0,1 );
                displayStringWrite("                 ");
                break;

            case WIPER_INT: 
                displayStringWrite("INT");
                displayCharPositionWrite( 0,1 );
                displayStringWrite("Delay Type:");
                displayCharPositionWrite( 11,1 );
                switch ( intDelayType() ) {
                    case INT_SHORT:
                        displayStringWrite("SHORT");
                        break;
                    case INT_MED:
                        displayStringWrite("MED  ");
                        break;
                    case INT_LONG:
                        displayStringWrite("LONG ");
                        break;
                }
                break;

            case WIPER_OFF:
                displayStringWrite("OFF");
                displayCharPositionWrite( 0,1 );
                displayStringWrite("                 ");
                break;
        }
    }
}

//=====[Implementations of private functions]==================================