# Automatic Headlight Control System  

Ryan Petrauskas and Livy Bottomley

The Windshield Wiper Control System in Project 3 enhances driving convenience and safety by creating windshield wiper functions in various driving conditions. The system ensures that the engine is only started when both the driver and passenger seats are occupied, and both seatbelts are fastened. The wipers can operate in four modes: high-speed, low-speed, intermittent, and off. In the intermittent mode, the user can select from three different delay times: short, medium, or long. The wipers will only function if the engine is running, and they will stop when the engine is turned off. At high-speed, the servo moves at 40 rpm and 30 rpm at low-speed. If the engine or wipers are turned off, the wipers will move back to their original position.

In this project, we chose to use a position servo motor to simulate the windshield wiper motor. The reason for this choice is that position servo motors allow precise control over the wiper's position. This is ideal for accurately simulating the movement of the wipers from one end of the windshield to the other. Additionally, position servos can hold a specific position until instructed to move again, which is an important trait for the movement of windshield wipers.


# System Components  

| Component                     | Description                                                         |
|-------------------------------|---------------------------------------------------------------------|
| **Driver Seat Sensor**        | Push button to detect if the driver seat is occupied                |
| **Passenger Seat Sensor**     | Push button to detect if the passenger seat is occupied             |
| **Driver Seatbelt Sensor**    | Toggle switch to check if the driver's seatbelt is fastened         |
| **Passenger Seatbelt Sensor** | Toggle switch to check if the driver's seatbelt is fastened         |
| **Ignition Button**           | Push Button on NUCLEO board to turn engine on or off                |
| **Indicator LEDs**            | Blue LED to show engine on and Green LED to show ignition enabled   |
| **Wiper Mode Selector**       | Potentiometer that selects wiper modes: HI, LO, INT, OFF            |
| **Int Delay Selector**        | Potentiometer that selects delay time in int mode: SHORT, MED, LONG |
| **LCD Display**               | Displays the current wiper mode and delay time chosen by user       |
| **Windshield Wiper Motor**    | Positional Servo that simulates windshield wiper movement           |
| **Siren**                     | Sounds when try to start the engine when system is not ready        |
| **Windshield Wiper**          | Pencil                                                              |

# Wiper Modes  

| Mode    | Potentiometer Range | Behavior                                                  |
|---------|---------------------|-----------------------------------------------------------|
| **HI**  | >= 0.75             | Wipers move at high speed                                 |
| **LO**  | 0.50 - 0.75         | Wipers move at low speed                                  |
| **INT** | 0.25 - 0.50         | Wipers move at low speed, with given delay between sweeps |
| **OFF** | < 0.25              | Wipers do not move and remain stationary                  |

# Intermittent Delay Time Modes  

| Mode       | Potentiometer Range | Behavior                            |
|------------|---------------------|-------------------------------------|
| **LONG**   | >= 0.66             | 8-second delay between wiper sweeps |
| **MEDIUM** | 0.66 - 0.33         | 6-second delay between wiper sweeps |
| **SHORT**  | < 0.33              | 3-second delay between wiper sweeps |

# Testing Results  

| Subsystem                   | Specification                                                                  | Results  | 
|-----------------------------|--------------------------------------------------------------------------------|----------|
| **Ignition System**         | Enable engine start when both seats are occupied and seatbelts fastened        |  Pass    |
|                             | Welcome driver when seated                                                     |  Pass    |
|                             | Display appropriate error messages when ignition is inhibited                  |  Pass    |
|                             | Allows for multiple attempts to start engine                                   |  Pass    |
|                             | Keep the engine running even if seat belts are unfastened or occupants leave   |  Pass    |
|                             | Stop the engine when the ignition button is pressed and released               |  Pass    |
| **Windshield Wiper System** | Wipers run in HI, LO, INT, or OFF when engine is running                       |  Pass    |
|                             | In INT mode, delay time is correct based on chosen mode (SHORT, MED, LONG)     |  Pass    |
|                             | Selected mode is displayed, including selection of delay time                  |  Pass    |
|                             | Wipers turn off if engine turns off                                            |  Pass    |
|                             | Wipers complete cycle and return to 0 degrees if engine is turned off          |  Pass    |
|                             | Wipers complete cycle and return to 0 degrees if wipers are turned off         |  Pass    |
|                             | Wipers remain stationary if engine is turned off while hesitating in INT mode  |  Pass    |
|                             | Wipers remain stationary if wipers are turned off while hesitating in INT mode |  Pass    |




