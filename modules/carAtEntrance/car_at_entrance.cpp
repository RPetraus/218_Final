#include "car_at_entrance.h"
#include "arm_book_lib.h"
#include "pc_serial_com.h"  // Added this include for the serial communication functions

// Light sensor configuration
#define NIGHT_LEVEL 40   // Dusk level
#define DAY_LEVEL 30     // Daylight level
#define LIGHT_SENSOR_SAMPLES 10  // Number of samples to average

// Global variables
bool Car_Detect = false;
float lightReadingsArray[LIGHT_SENSOR_SAMPLES];
static int lightSampleIndex = 0;

// Define the analog input for light sensor
AnalogIn lightsens(A0);

void carAtEntranceInit()
{
    // Initialize light readings array to zero
    for (int i = 0; i < LIGHT_SENSOR_SAMPLES; i++) {
        lightReadingsArray[i] = 0.0;
    }
    
    // Send initialization message using the pc_serial_com module function
    pcSerialComStringWrite("Car Detection System Initialized\r\n");
}

bool sensorUpdate() 
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
    float currentLightLevel = (1.0 - (lightReadingsSum / LIGHT_SENSOR_SAMPLES)) * 10000.0;
    
    // Debug: Print the current light level
    char buffer[50];
    sprintf(buffer, "Light level: %.2f\r\n", currentLightLevel);
    pcSerialComStringWrite(buffer);  // Using the pc_serial_com module function
    
    // Check if the light level indicates dusk or darker
    if (currentLightLevel >= NIGHT_LEVEL) {
        Car_Detect = true;  // Set Car_Detect to true when it's dusk or darker
    } else if (currentLightLevel < DAY_LEVEL) {
        Car_Detect = false; // Reset when it's clearly daylight
    }
    
    return Car_Detect;  // Return the state of Car_Detect
}

void carAtEntranceUpdate()
{
    static bool previousDetectionState = false;
    
    // Update sensor and get detection status
    bool currentDetectionState = sensorUpdate();
    
    // Only print when the state changes to avoid flooding the serial output
    if (currentDetectionState != previousDetectionState) {
        if (currentDetectionState) {
            pcSerialComStringWrite("Car Detected\r\n");  // Using the pc_serial_com module function
        } else {
            pcSerialComStringWrite("No Car Detected\r\n");  // Using the pc_serial_com module function
        }
        
        previousDetectionState = currentDetectionState;
    }
}

bool isCarDetected()
{
    return Car_Detect;
}