//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

#include "headlight_subsystem.h"

//=====[Defines]===============================================================

#define DAYLIGHT_DELAY_MS                       2000                     
#define DUSK_DELAY_MS                           1000
#define READING_INCREMENT_MS                      10
#define LIGHT_SENSOR_MACRO               000000000 

//=====[Declaration of public data types]======================================

DigitalOut leftBeam(D8);
DigitalOut rightBeam(D9);

AnalogIn headlightMode(A1);
AnalogIn lightSensor(A0);

//=====[Declaration and initialization of public global objects]===============

//=====[Declaration and initialization of public global variables]=============

//=====[Declarations (prototypes) of public functions]=========================

//=====[Implementations of public functions]===================================
