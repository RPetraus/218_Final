//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

#include "siren.h"

#include "smart_car_system.h"

//=====[Declaration of private defines]========================================

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

DigitalOut sirenPin(PE_10);

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

static bool sirenState = OFF;

//=====[Declarations (prototypes) of private functions]========================

//=====[Implementations of public functions]===================================

/**
* Initializes the siren to not output
*/
void sirenInit()
{
    sirenPin = ON;
}
 

/**
* Reads the current state of the siren
* Returns the current state of the siren
*/
bool sirenStateRead()
{
    return sirenState;
}


/**
* Sets the state of the siren 
* Parameter: The desired state of the siren
*/
void sirenStateWrite( bool state )
{
    sirenState = state;
}

/**
* Updates the siren's output based on its state.
*/
void sirenUpdate() {
    if ( sirenState == ON ) {
        sirenPin = ON;
    } else {
        sirenPin = OFF;
    }
}

//=====[Implementations of private functions]==================================

