//=====[Libraries]=============================================================

#include "arm_book_lib.h"
#include "mbed.h"

#include "smart_car_system.h"

//=====[Defines]===============================================================

//=====[Declaration of public data types]======================================

//=====[Declaration and initialization of public global objects]===============

//=====[Declaration and initialization of public global variables]=============

//=====[Declarations (prototypes) of public functions]=========================

int main()
{
    smartCarSystemInit();
    while (true) {
        smartCarSystemUpdate();
    }
}

//=====[Implementations of public functions]===================================
