//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

//=====[Defines]===============================================================

#define DAYLIGHT_DELAY_MS                       2000                     
#define DUSK_DELAY_MS                           1000
#define READING_INCREMENT_MS                      10
# #define LIGHT_SENSOR_MACRO               000000000 

//=====[Declaration and initialization of public global objects]===============

DigitalIn driverPresent(D2);
DigitalIn passengerPresent(D3);
DigitalIn driverSeatbelt(D4);
DigitalIn passengerSeatbelt(D5);
DigitalIn ignitionButton(BUTTON1);

DigitalOut greenLED(LED1);
DigitalOut blueLED(LED2);
DigitalOut leftBeam(D8);
DigitalOut rightBeam(D9);

DigitalOut sirenPin(PE_10);

AnalogIn headlightMode(A0);
AnalogIn lightSensor(D7)

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
void lightLevel();

//=====[Main function, the program entry point after power on or reset]========

int main()
{
    inputsInit();
    outputsInit();
    while (true) {
        alarmActivationUpdate();
        alarmDeactivationUpdate();
        uartTask();
        delay(TIME_INCREMENT_MS);
    }
}

//=====[Implementations of public functions]===================================

void inputsInit()
{
    driverPresent.mode(PullDown);
    passengerPresent.mode(PullDown);
    driverSeatbelt.mode(PullDown);
    passengerSeatbelt.mode(PullDown);
}

void outputsInit()
{
    greenIndicator = OFF;
    blueIndicator = OFF;
    sirenPin = OFF;
    leftBeam = OFF;
    rightBeam = OFF;
}

void welcomeMessage()
{
    if (driverPresent){uartUsb.write("Welcome to enhanced alarm system model 218-W25\r\n", 48);}
}

void ignitionEnable()
{
    if (driverPresent && driverSeatbelt && passengerPresent && passengerSeatbelt){
        green
    }
}

void ignitionSubsystem()
{
    welcomeMessage();

    if (ignitionButton){
    }

}
