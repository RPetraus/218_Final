//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

#include "ignition_subsystem.h"

//=====[Defines]===============================================================

#define BUZZER_ON 0
#define BUZZER_OFF 1

//=====[Declaration of public data types]======================================

//=====[Declaration and initialization of public global objects]===============

DigitalIn driverPresent(D5);
DigitalIn passengerPresent(D4);
DigitalIn driverSeatbelt(D3);
DigitalIn passengerSeatbelt(D2);
DigitalIn ignitionButton(BUTTON1);

DigitalOut greenLED(LED1);
DigitalOut blueLED(LED2);
DigitalOut sirenPin(D6);

UnbufferedSerial uartUsb(USBTX, USBRX, 115200);

//=====[Declaration and initialization of public global variables]=============

bool driverWelcomed = false;
bool errorReceived = false;


//=====[Declarations (prototypes) of public functions]=========================

void welcomeMessage();
void errorMessage();
void ignitionEnable();

//=====[Implementations of public functions]===================================

void inputsInit()
{
    driverPresent.mode(PullDown);
    passengerPresent.mode(PullDown);
    driverSeatbelt.mode(PullDown);
    passengerSeatbelt.mode(PullDown);
}

void outputsInitIgnition()
{
    greenLED = OFF;
    blueLED = OFF;
    sirenPin = BUZZER_OFF;
}

void engineInit()
{
    engineState = ENGINE_OFF;
}
void welcomeMessage()
{
    if (driverPresent && !driverWelcomed){
        uartUsb.write("Welcome to enhanced alarm system model 218-W25\r\n", 48);
        driverWelcomed = true;
    }
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

void ignitionUpdate()
{
    ignitionEnable();
    welcomeMessage();

    switch( engineState ){
    
    case ENGINE_OFF:
        if (ignitionButton && greenLED){
            uartUsb.write("Engine started.\r\n", 17);
            sirenPin = BUZZER_OFF;
            greenLED = OFF;
            blueLED = ON;
            engineState = ENGINE_ON;

        }
        else if (ignitionButton && !greenLED && !errorReceived){
            sirenPin = BUZZER_ON;
            errorMessage();
            while(ignitionButton){errorReceived = true;} // want to change this
            errorReceived = false;
        }
        break;

    case ENGINE_ON:
        if (ignitionButton){
            while(ignitionButton){} // want to change this
            engineState = ENGINE_OFF;
        }
        break;
    }
}
