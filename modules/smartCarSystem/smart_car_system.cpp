//=====[Libraries]=============================================================

#include "arm_book_lib.h"
#include "mbed.h"

#include "ignition_subsystem.h"
#include "smart_car_system.h"
#include "user_display.h"

//=====[Defines]===============================================================

//=====[Declaration of public data types]======================================

//=====[Declaration and initialization of public global objects]===============

//=====[Declaration and initialization of public global variables]=============

//=====[Declarations (prototypes) of public functions]=========================

//=====[Implementations of public functions]===================================

void smartCarSystemInit()
{
    //ignitionSubsystemInit();
    // windshieldWiperSubsystemInit();
    userDisplayInit();
}


void smartCarSystemUpdate()
{
    //ignitionSubsystemUpdate();
    userDisplayUpdate();
    delay(TIME_INCREMENT_MS);
}