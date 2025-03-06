/*
#include "exit_subsystem.h"
#include "move_gate.h"  // For openGate and closeGate functions
#include "car_at_entrance.h"  // For the Car_Detect status via isCarDetected()
#include "pc_serial_com.h"  // For debug messages
#include "arm_book_lib.h"
#include "mbed.h"


DigitalIn exitButtonPin(PG_1);  // Adjust pin as needed for your hardware

// State variables
static bool gateIsOpen = false;
static bool previousButtonState = false;

void exitButtonInit()
{
    // Initialize the exit button module
    gateIsOpen = false;
    pcSerialComStringWrite("Exit Button System Initialized\r\n");
}

void exitButtonUpdate()
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
    if (gateIsOpen && !isCarDetected()) {
        pcSerialComStringWrite("Car has exited - Closing Gate\r\n");
        closeGate();
        gateIsOpen = false;
    }
    
    // Update previous button state
    previousButtonState = currentButtonState;
}
*/