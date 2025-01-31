//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

#include "ignition_subsystem.h"

//=====[Defines]===============================================================



//=====[Declaration of public data types]======================================

//=====[Declaration and initialization of public global objects]===============

/*
DigitalIn driverPresent(D2);
DigitalIn passengerPresent(D3);
DigitalIn driverSeatbelt(D4);
DigitalIn passengerSeatbelt(D5);
DigitalIn ignitionButton(BUTTON1);

DigitalOut greenLED(LED1);
DigitalOut blueLED(LED2);
*/
DigitalOut leftBeam(D8);
DigitalOut rightBeam(D9);

DigitalOut sirenPin(PE_10);

AnalogIn headlightMode(A1);
//AnalogIn lightSensor(A0);

UnbufferedSerial uartUsb(USBTX, USBRX, 115200);

//=====[Declaration and initialization of public global variables]=============

bool isDaylight; // currently lacking initializations of global variables
bool isDusk;

float lightLevel;
float daylightLevel;
float duskLevel;

//=====[Declarations (prototypes) of public functions]=========================

void inputsInit();
void outputsInit();

void headlightSubsystem();
void beamMode();
//void lightLevel();

//=====[Main function, the program entry point after power on or reset]========

int main()
{
    inputsInit();
    outputsInitIgnition();
    while (true) {
        ignitionUpdate();
    }
}

//=====[Implementations of public functions]===================================