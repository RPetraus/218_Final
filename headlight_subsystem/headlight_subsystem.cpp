//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

#include "headlight_subsystem.h"
#include "ignition_subsystem.h"

//=====[Defines]===============================================================

#define DAYLIGHT_DELAY_MS                       2000                     
#define DUSK_DELAY_MS                           1000
#define READING_INCREMENT_MS                      10
#define LDR_NUMBER_OF_AVG_SAMPLES    10
 

//=====[Declaration of public data types]======================================

typedef enum{
    HL_ON,
    HL_AUTO,
    HL_OFF
} HLMode_t;

//=====[Declaration and initialization of public global objects]===============

DigitalOut leftBeam(D8);
DigitalOut rightBeam(D9);

AnalogIn headlightMode(A1);
AnalogIn lightSensor(A0);

//=====[Declaration and initialization of public global variables]=============

HLMode_t HLMode;

float onMode = 0.33;
float autoMode = 0.66;

float headlightModeSelector;

float daylightLevel = 0.6;
float duskLevel = 0.3;
float LDR_readings[LDR_NUMBER_OF_AVG_SAMPLES];
float LDRReadingAvg = 0.0;

//=====[Declarations (prototypes) of public functions]=========================

void headlightModeUpdate();
void lightSensorInit();
void lightLevelUpdate();


//=====[Implementations of public functions]===================================

void outputsInitHeadlight()
{
    leftBeam = OFF;
    rightBeam = OFF;
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
    else{
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


void headlightUpdate()
{
    lightSensorInit();
    lightLevelUpdate();
    headlightModeUpdate();

    switch ( HLMode ){
        case HL_ON:
            if ((engineState = ENGINE_ON)){
                leftBeam = ON;
                rightBeam = ON;
            }
            else{
                leftBeam = OFF;
                rightBeam = OFF;
            }
        break;

        case HL_OFF:
            leftBeam = OFF;
            rightBeam = OFF;
        break;

        case HL_AUTO:
            if ((engineState = ENGINE_OFF)){
                if (LDRReadingAvg < duskLevel){
                    delay(DUSK_DELAY_MS);
                    leftBeam = ON;
                    rightBeam = ON;
                }
                else if (LDRReadingAvg > duskLevel && LDRReadingAvg < daylightLevel){
                    leftBeam = leftBeam;
                    rightBeam = rightBeam;
                }
                else if ( LDRReadingAvg > daylightLevel){
                    delay(DAYLIGHT_DELAY_MS);
                    leftBeam = OFF;
                    rightBeam = OFF;
                }
            }
            else{
                leftBeam = OFF;
                rightBeam = OFF;
            }
        break;
    }
}