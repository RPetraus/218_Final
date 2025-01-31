//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"


//=====[Defines]===============================================================

#define BUZZER_ON 0
#define BUZZER_OFF 1
#define TIME_INCREMENT_MS 10
#define TIME_DEBOUNCE_MS 30
#define DELAY_LIGHTS_OFF_MS 2000
#define DELAY_LIGHTS_ON_MS 1000

#define DAYLIGHT_DELAY_MS 2000                     
#define DUSK_DELAY_MS 1000
#define READING_INCREMENT_MS 10
#define LDR_NUMBER_OF_AVG_SAMPLES 10

//=====[Declaration of public data types]======================================

typedef enum{
    BUTTON_UP,
    BUTTON_FALLING,
    BUTTON_DOWN,
    BUTTON_RISING
} debouncedIgnitionReleasedStateMachine_t;

typedef enum{
    HL_ON,
    HL_AUTO,
    HL_OFF
} HLMode_t;


//=====[Declaration and initialization of public global objects]===============

DigitalIn driverPresent(D5);
DigitalIn passengerPresent(D4);
DigitalIn driverSeatbelt(D3);
DigitalIn passengerSeatbelt(D2);
DigitalIn ignitionButton(BUTTON1);

DigitalOut greenLED(LED1);
DigitalOut blueLED(LED2);
DigitalOut sirenPin(D6);

DigitalOut leftBeam(D8);
DigitalOut rightBeam(D9);

AnalogIn headlightMode(A1);
AnalogIn lightSensor(A0);

UnbufferedSerial uartUsb(USBTX, USBRX, 115200);

//=====[Declaration and initialization of public global variables]=============

int accumulatedButtonTime = 0;
int accumulatedHeadlightDelayTime_ON = 0;
int accumulatedHeadlightDelayTime_OFF = 0;

bool engineStarted = false;
bool driverWelcomed = false;

debouncedIgnitionReleasedStateMachine_t ignitionButtonState;

HLMode_t HLMode;

float onMode = 0.33;
float autoMode = 0.66;

float headlightModeSelector;

float daylightLevel = 0.6;
float duskLevel = 0.3;
float LDR_readings[LDR_NUMBER_OF_AVG_SAMPLES];
float LDRReadingAvg = 0.0;

//=====[Declarations (prototypes) of public functions]=========================

void inputsInit();
void outputsInitIgnition();

void welcomeMessage();
void errorMessage();

void debounceIgnitionInit();
bool debounceIgnition();

void ignitionEnable();
bool ignitionUpdate();

void outputsInitHeadlight();
void headlightModeUpdate();
void lightSensorInit();
void lightLevelUpdate();
float lightSensorRead();
void headlightUpdate();

int main()
{
    inputsInit();
    outputsInitIgnition();
    outputsInitHeadlight();
    lightSensorInit();
    while (true) {
        ignitionUpdate();
        headlightUpdate();
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

void outputsInitIgnition()
{
    greenLED = OFF;
    blueLED = OFF;
    sirenPin = BUZZER_OFF;
}

void outputsInitHeadlight()
{
    leftBeam = OFF;
    rightBeam = OFF;
}

void debounceIgnitionInit()
{
    if( ignitionButton ) {
        ignitionButtonState = BUTTON_UP;
    } else {
        ignitionButtonState = BUTTON_DOWN;
    }
}


bool debounceIgnition()
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

void ignitionEnable()
{
    if (driverPresent && driverSeatbelt && passengerPresent && passengerSeatbelt){
        greenLED = ON;
    }
    else{
        greenLED = OFF;
    }
}

void welcomeMessage()
{
    if (driverPresent && !driverWelcomed){
        uartUsb.write("Welcome to enhanced alarm system model 218-W25\r\n", 48);
        driverWelcomed = true;
    }
    else if (!driverPresent){
        driverWelcomed = false;
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

bool ignitionUpdate()
{
    welcomeMessage();
    ignitionEnable();
    if (!engineStarted && debounceIgnition()){
        uartUsb.write("Ignition attempted.\r\n", 21);
        if (!greenLED){
            sirenPin = BUZZER_ON;
            errorMessage();
            engineStarted = false;
        }
        else{
            sirenPin = BUZZER_OFF;
            greenLED = OFF;
            blueLED = ON;
            uartUsb.write("Engine started.\r\n", 17);
            engineStarted = true;
        }
    }
    else if (engineStarted && debounceIgnition()){
        uartUsb.write("Engine off.\r\n", 13);
        engineStarted = false;
        blueLED = OFF;
    }
    
    return engineStarted;
}

void headlightModeUpdate()
{
    headlightModeSelector = headlightMode.read();

    if (headlightModeSelector <= onMode){
        HLMode = HL_ON;
    }
    else if (headlightModeSelector > onMode && headlightModeSelector <= autoMode){
        HLMode = HL_AUTO;
    }
    else if (headlightModeSelector > autoMode){
        HLMode = HL_OFF;
    }
}

void lightSensorInit(){
    int x;

    for( x=0; x<LDR_NUMBER_OF_AVG_SAMPLES; x++){
        LDR_readings[x] = 0;
    }
}

void lightLevelUpdate()
{
    static int LDRSampleIndex = 0;
    float LDRReadingsSum;
    int x;

    LDR_readings[LDRSampleIndex] = lightSensor.read();
       LDRSampleIndex++;
    if ( LDRSampleIndex >= LDR_NUMBER_OF_AVG_SAMPLES) {
        LDRSampleIndex = 0;
    }
    
   LDRReadingsSum = 0.0;
    for (x = 0; x < LDR_NUMBER_OF_AVG_SAMPLES; x++) {
        LDRReadingsSum = LDRReadingsSum + LDR_readings[x];
    }
    LDRReadingAvg = LDRReadingsSum / LDR_NUMBER_OF_AVG_SAMPLES;
}

float lightSensorRead(){
    lightLevelUpdate();
    return LDRReadingAvg;
}

void headlightUpdate()
{
    headlightModeUpdate();

    if (engineStarted){
        switch (HLMode){
            case HL_ON:
                leftBeam = ON;
                rightBeam = ON;

            break;

            case HL_OFF:
                leftBeam = OFF;
                rightBeam = OFF;

            break;

            case HL_AUTO:
                
                if (lightSensor.read() <= duskLevel){
                    accumulatedHeadlightDelayTime_ON = accumulatedHeadlightDelayTime_ON + TIME_INCREMENT_MS;
                    if (accumulatedHeadlightDelayTime_OFF >= DELAY_LIGHTS_ON_MS){
                        accumulatedHeadlightDelayTime_OFF = 0;
                        accumulatedHeadlightDelayTime_ON = 0;
                        leftBeam = ON;
                        rightBeam = ON;
                    }

                } else if (lightSensor.read() > duskLevel && lightSensor.read() <= daylightLevel){
                    accumulatedHeadlightDelayTime_ON = 0;
                    accumulatedHeadlightDelayTime_OFF = 0;

                } else if ( lightSensor.read() > daylightLevel){
                    accumulatedHeadlightDelayTime_OFF = accumulatedHeadlightDelayTime_OFF + TIME_INCREMENT_MS;
                    if (accumulatedHeadlightDelayTime_OFF >= DELAY_LIGHTS_OFF_MS){
                        accumulatedHeadlightDelayTime_OFF = 0;
                        accumulatedHeadlightDelayTime_ON = 0;
                        leftBeam = OFF;
                        rightBeam = OFF;
                    }
                }
        }
    } else{
        leftBeam = OFF;
        rightBeam = OFF;
    }
}


