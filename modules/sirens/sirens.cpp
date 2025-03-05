//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

#include "sirens.h"

//=====[Declaration of private defines]========================================

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

DigitalOut internalSirenPin(PE_10);
DigitalOut externalSirenPin(PE_12);

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

static bool internalSirenState = OFF;
static bool externalSirenState = OFF;

//=====[Declarations (prototypes) of private functions]========================

//=====[Implementations of public functions]===================================

/**
* Initializes the siren to not output
*/
void sirensInit()
{
    internalSirenPin = ON;
    externalSirenPin = ON;
}
 

bool internalSirenStateRead()
{
    return internalSirenState;
}

bool externalSirenStateRead()
{
    return externalSirenState;
}

void internalSirenStateWrite( bool state )
{
    internalSirenState = state;
}

void externalSirenStateWrite( bool state )
{
    externalSirenState = state;
}


void sirensUpdate() {
    if ( internalSirenState ) {
        internalSirenPin = OFF;
    } else {
        internalSirenPin = ON;
    }
    
    if ( externalSirenState ) {
        externalSirenPin = OFF;
    } else {
        externalSirenPin = ON;
    }
}

//=====[Implementations of private functions]==================================

