//=====[Libraries]=============================================================

#include "arm_book_lib.h"
#include "mbed.h"

#include "parking_system.h"

//=====[Defines]===============================================================

//=====[Declaration of public data types]======================================

//=====[Declaration and initialization of public global objects]===============

//=====[Declaration and initialization of public global variables]=============

//=====[Declarations (prototypes) of public functions]=========================

int main()
{
    parkingSystemInit();
    while (true) {
        parkingSystemUpdate();
    }
}

//=====[Implementations of public functions]===================================
