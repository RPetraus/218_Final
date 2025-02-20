//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

#include "ignition_subsystem.h"
#include "smart_car_system.h"
#include "windshield_wiper_subsystem.h"
#include "siren.h"
#include "pc_serial_com.h"


//=====[Declaration of private defines]========================================

#define TIME_DEBOUNCE_MS 30

//=====[Declaration of private data types]=====================================

typedef enum{
    BUTTON_UP,       //ignition button is not pressed
    BUTTON_FALLING,  //ignition button is being pressed down
    BUTTON_DOWN,     //ignition button is pressed 
    BUTTON_RISING    //ignition button is being released
} debouncedIgnitionReleasedStateMachine_t;

//=====[Declaration and initialization of public global objects]===============

DigitalIn driverPresent(D10);
DigitalIn passengerPresent(D11);
DigitalIn driverSeatbelt(D12);
DigitalIn passengerSeatbelt(D13);
DigitalIn ignitionButton(BUTTON1);

DigitalOut greenLED(LED1);
DigitalOut blueLED(LED2);

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

int accumulatedButtonTime = 0;

bool engineRunning = false;
bool driverWelcomed = false;
bool displayErrors = false;
bool ignitionAttempted = false;

debouncedIgnitionReleasedStateMachine_t ignitionButtonState;
debouncedIgnitionReleasedStateMachine_t ignitionButtonLastState;

//=====[Declaration and initialization of private global variables]============

//=====[Declarations (prototypes) of private functions]=========================

static void inputsInit();
static void outputsInitIgnition();
static void debounceIgnitionInit();

static void welcomeMessage();
static void ignitionEnable();
static bool debounceIgnition();
static void errorMessage();

//=====[Implementations of public functions]===================================
/**
* Initializes the ignition subsystem, including inputs, outputs, and debounce.
*/
void ignitionSubsystemInit()
{
    inputsInit();
    outputsInitIgnition();
    debounceIgnitionInit();
}


/**
* Updates the ignition subsystem. Handls ignition button state and the status of engine.
* Returns true if engine is running, false otherwise.
*/
bool ignitionSubsystemUpdate()
{
    welcomeMessage();
    ignitionEnable();
     
    if (!engineRunning && debounceIgnition() && ignitionAttempted){
        pcSerialComStringWrite("Ignition attempted.\r\n");
        if (!greenLED){
            sirenStateWrite( OFF );
            sirenUpdate();
            errorMessage();
            engineRunning = false;
        }
        else{
            sirenStateWrite( ON );
            sirenUpdate();
            greenLED = OFF;
            blueLED = ON;
            pcSerialComStringWrite("Engine started.\r\n");
            engineRunning = true;
        }
    } else if (engineRunning && debounceIgnition()){
        pcSerialComStringWrite("Engine off.\r\n");

        windshieldWiperStop();

        engineRunning = false;
        blueLED = OFF;
    }
    
    return engineRunning;
}

 
//=====[Implementations of private functions]===================================

//Initializes input pins
static void inputsInit()
{
    driverPresent.mode(PullDown);
    passengerPresent.mode(PullDown);
    driverSeatbelt.mode(PullDown);
    passengerSeatbelt.mode(PullDown);
    sirenInit();
}

//Initializes output pins
static void outputsInitIgnition()
{
    greenLED = OFF;
    blueLED = OFF;
}

//Initializes the ignition button state for debouncing purposes
static void debounceIgnitionInit()
{
    if( ignitionButton ) {
        ignitionButtonState = BUTTON_UP;
    } else {
        ignitionButtonState = BUTTON_DOWN;
    }
}

//Displays a welcome message when the driver is seated
static void welcomeMessage()
{
    if (driverPresent && !driverWelcomed){
        pcSerialComStringWrite("Welcome to enhanced alarm system model 218-W25\r\n");
        driverWelcomed = true;
    }
    else if (!driverPresent){
        driverWelcomed = false;
    }
}

//Enables ignition LED based on seat and seatbelt status
static void ignitionEnable()
{
    if (driverPresent && driverSeatbelt && passengerPresent && passengerSeatbelt){
        greenLED = ON;
    }
    else{
        greenLED = OFF;
    }
}

//Debounces the ignition button (avoids false triggers). Returns true if ignition button released
static bool debounceIgnition()
{
    bool ignitionReleased = false;

    switch( ignitionButtonState ) {
        case BUTTON_UP:
            if( ignitionButton ) {
                ignitionButtonState = BUTTON_FALLING;
                accumulatedButtonTime = 0;
            }
            break;

        case BUTTON_FALLING:
            if( accumulatedButtonTime >= TIME_DEBOUNCE_MS ) {
                if( ignitionButton ) {
                    ignitionButtonState = BUTTON_DOWN;
                    ignitionAttempted = true;
                } else {
                    ignitionButtonState = BUTTON_UP;
                }
            }
            accumulatedButtonTime = accumulatedButtonTime + TIME_INCREMENT_MS;
            break;
        
        case BUTTON_DOWN:
            if (!ignitionButton){
                ignitionButtonState = BUTTON_RISING;
                accumulatedButtonTime = 0;
            }
            break;

        case BUTTON_RISING:
            if (!ignitionButton){
                ignitionButtonState = BUTTON_UP;
                ignitionReleased = true;
            }
            else{
                ignitionButtonState = BUTTON_DOWN;
            }
            accumulatedButtonTime = accumulatedButtonTime + TIME_INCREMENT_MS;
            break;
    }
    return ignitionReleased;
}


//Displays error messages if ignition is inhibited
static void errorMessage()
{

    pcSerialComStringWrite("Ignition Inhibited\r\n");
    
    if(!driverPresent){
        pcSerialComStringWrite("Driver seat not occupied.\r\n");
    } 
    if(!driverSeatbelt){
        pcSerialComStringWrite("Driver seatbelt not fastened.\r\n");
    }
    if(!passengerPresent){
        pcSerialComStringWrite("Passenger seat not occupied.\r\n");
    }
    if(!passengerSeatbelt){
        pcSerialComStringWrite("Passenger seatbelt not fastened.\r\n");
    }
    displayErrors = true;
}