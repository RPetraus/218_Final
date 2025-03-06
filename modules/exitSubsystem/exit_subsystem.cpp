//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

#include "exit_subsystem.h"

#include "move_gate.h"
#include "car_at_entrance.h"
#include "pc_serial_com.h"

//=====[Declaration of private defines]========================================

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

DigitalIn exitButtonPin(PG_1);

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

static bool gateIsOpen = false;
static bool previousButtonState = false;

//=====[Declarations (prototypes) of private functions]=========================

//=====[Implementations of public functions]===================================

void exitSubsystemInit()
{
    gateIsOpen = false;
    pcSerialComStringWrite("Exit Button System Initialized\r\n");
}

void exitSubsysemUpdate()
{
    // Read current button state (assuming active low button - pressed when LOW)
    bool currentButtonState = !exitButtonPin.read();
    
    // Check for button press (transition from not pressed to pressed)
    if (currentButtonState && !previousButtonState) {
        if (!gateIsOpen) {
            
            pcSerialComStringWrite("Exit Button Pressed - Opening Gate\r\n");
            openGate();
            gateIsOpen = true;
        }
    }
    
    // If gate is open and car has left (Car_Detect is false), close the gate
    if (gateIsOpen && !carIsDetected()) {
        pcSerialComStringWrite("Car has exited - Closing Gate\r\n");
        closeGate();
        gateIsOpen = false;
    }
    
    // Update previous button state
    previousButtonState = currentButtonState;
}

//=====[Implementations of private functions]==================================