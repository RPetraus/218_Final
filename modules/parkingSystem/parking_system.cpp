//=====[Libraries]=============================================================

#include "arm_book_lib.h"

#include "parking_system.h"







#include "user_display.h"

#include "sirens.h"
#include "code.h"

//=====[Declaration of private defines]========================================

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

//=====[Declarations (prototypes) of private functions]========================

//=====[Implementations of public functions]===================================

/**
* Initializes the smart car system by initializing the subsystems
*/
void parkingSystemInit()
{
    sirensInit();

    userDisplayInit();

    resetCode();
}


/**
* Updates the smart car system by updating its subsystems
*/
void parkingSystemUpdate()
{
    
    
    delay(TIME_INCREMENT_MS);
}

//=====[Implementations of private functions]==================================