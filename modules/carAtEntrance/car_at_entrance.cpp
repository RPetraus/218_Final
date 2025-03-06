#include "car_at_entrance.h"
#include "arm_book_lib.h"

#include "pc_serial_com.h"  // Added this include for the serial communication functions

//=====[Declaration of private defines]========================================

#define NIGHT_LEVEL 26  
#define DAY_LEVEL 17     
#define LIGHT_SENSOR_SAMPLES 10 

// Global variables
bool Car_Detect = false;

//=====[Declaration of private data types]=====================================

float lightReadingsArray[LIGHT_SENSOR_SAMPLES];
static int lightSampleIndex = 0;

//=====[Declaration and initialization of public global objects]===============

AnalogIn lightsens(A0);

//=====[Declarations (prototypes) of private functions]========================

static bool sensorUpdate();

//=====[Implementations of public functions]===================================

void carAtEntranceInit()
{
    // Initialize light readings array to zero
    for (int i = 0; i < LIGHT_SENSOR_SAMPLES; i++) {
        lightReadingsArray[i] = 0.0;
    }
    
    // Send initialization message using the pc_serial_com module function
    pcSerialComStringWrite("Car Detection System Initialized\r\n");
}

void carAtEntranceUpdate()
{
    static bool previousDetectionState = false;
    bool currentDetectionState = sensorUpdate();
    if (currentDetectionState != previousDetectionState) {
        if (currentDetectionState) {
            pcSerialComStringWrite("Car Detected\r\n");
        } else {
            pcSerialComStringWrite("No Car Detected\r\n");  
        }
        
        previousDetectionState = currentDetectionState;
    }
}
bool carIsDetected()
{
    return Car_Detect;
}

//=====[Implementations of public functions]===================================

static bool sensorUpdate() 
{
    // Add new reading to array
    lightReadingsArray[lightSampleIndex] = lightsens.read();
    lightSampleIndex++;
    
    // Reset index if we reach the end of array
    if (lightSampleIndex >= LIGHT_SENSOR_SAMPLES) {
        lightSampleIndex = 0;
    }
    
    // Calculate average
    float lightReadingsSum = 0.0;
    for (int i = 0; i < LIGHT_SENSOR_SAMPLES; i++) {
        lightReadingsSum += lightReadingsArray[i];
    }
    
    // Scale to 0-100 (higher number means darker)
    float currentLightLevel = (1.0 - (lightReadingsSum / LIGHT_SENSOR_SAMPLES)) * 100.0;
    
    // Debug: Print the current light level
    char buffer[50];
    sprintf(buffer, "Light level: %.2f\r\n", currentLightLevel);
    pcSerialComStringWrite(buffer);  
    
    if (currentLightLevel >= NIGHT_LEVEL) {
        Car_Detect = true;  
    } else if (currentLightLevel < DAY_LEVEL) {
        Car_Detect = false; 
    }
    
    return Car_Detect;  // Return the state of Car_Detect
}