//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

#include "ignitionSubsystem.h"

//=====[Defines]===============================================================

#define DAYLIGHT_DELAY_MS                       2000                     
#define DUSK_DELAY_MS                           1000
#define READING_INCREMENT_MS                      10
//#define LIGHT_SENSOR_MACRO               000000000 

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
AnalogIn lightSensor(A0);

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

void ignitionSubsystem();
void welcomeMessage();
void engineStart();
void errorMessage();
void ignitionEnable();

void headlightSubsystem();
void beamMode();
//void lightLevel();

//=====[Main function, the program entry point after power on or reset]========

int main()
{
    
    while (true) {
    }
}

//=====[Implementations of public functions]===================================
