//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

//=====[Defines]===============================================================

#define DAYLIGHT_DELAY_MS                       2000                     
#define DUSK_DELAY_MS                           1000
#define READING_INCREMENT_MS                      10
# #define LIGHT_SENSOR_MACRO               000000000 

//=====[Declaration of public data types]======================================

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
    while (true) {
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
        greenLED = ON;
    }
    else{
        greenLED = OFF;
    }
}

void errorMessage()
{
    uartUsb.write("Ignition Inhibited\r\n", 20);
    

    if(!driverPresent){
        uartUsb.write("Driver seat not occupied.\r\n", 27);
    } 
    if(!driverSeatbelt){
        uartUsb.write("Driver seatbelt not fastened.\r\n", 31);
    }
    if(!passengerPresent){
        uartUsb.write("Passenger seat not occupied.\r\n", 30);
    }
    if(!passengerSeatbelt){
        uartUsb.write("Passenger seatbelt not fastened.\r\n", 34);
    }
}

void ignitionSubsystem()
{
    welcomeMessage();
    ignitionEnable();

    if (ignitionButton && greenLED){
        uartUsb.write("Engine started.\r\n", 17);
        greenLED = OFF;
        blueLED = ON;
    }
    else if (ignitionBuuton && !greenLED){
        errorMessage();
    }
}
