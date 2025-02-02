# Automatic Headlight Control System  

Ryan Petrauskas and Leo Weisberger  

The Automatic Headlight Control System in Project 2 enhances driving safety by automating headlight activation based on external conditions. It integrates a driver seat occupancy sensor, an ignition push button, a blue indicator LED on the NUCLEO board, a potentiometer for headlight mode selection, a light sensor, and two LEDs representing the low beam lamps. The ignition system ensures the engine can only start when the driver and passenger are seated and buckled. Once started, the engine remains on even if the driver exits. Pressing the ignition button again while the engine is running turns it off. Headlights operate in three modes controlled by the potentiometer: OFF when below 0.33, ON when above 0.66, and AUTO between 0.33 and 0.66. In AUTO mode, headlights turn on after one second in darkness and off after two seconds in brightness, maintaining their state otherwise.  

# System Components  

| Component                   | Description |
|-----------------------------|-------------|
| **Driver Seat Sensor**      | Push button |
| **Passenger Seat Sensor**   | Push button |
| **Driver Seatbelt Sensor**  | Toggleswitch |
| **Ignition Button**         | Push Button on NUCLEO board |
| **Indicator LEDs**          | Blue and Green LEDs on NUCLEO board |
| **Headlight Mode Selector** | Potentiometer |
| **Light Sensor**            | Light Dependent Resistor (LDR) |
| **Headlights**              | Two LEDs |

# Headlight Modes  

| Mode  | Potentiometer Range | Behavior |
|-------|----------------------|----------|
| **OFF**  | < 0.33  | Headlights remain off |
| **AUTO** | 0.33 - 0.66 | Turns headlights on/off based on ambient light |
| **ON**   | > 0.66  | Headlights remain on |

# Light Sensor Modes  

| Condition | Behavior |
|-----------|----------|
| **Dark (≤ 0.5)** | Turn headlights on after 1 second. |
| **Transition (between 0.5 and 0.8)** | No change in headlight state. |
| **Bright (> 0.8)** | Turn headlights off after a 2 seconds. |

# Testing Results  

| Subsystem           | Specification                                                                    | Results  | 
|-------------------------|------------------------------------------------------------------------------|----------|
| **Ignition System**     | Enable engine start when both seats are occupied and seatbelts fastened      |  Pass    |
|                         | Display appropriate error messages when ignition is inhibited                |  Pass    |
|                         | Keep the engine running even if seat belts are unfastened or occupants leave |  Pass    |
|                         | Stop the engine when the ignition button is pressed and released             |  Pass    |
| **Headlight System**    | ON mode turns both headlights on                                             |  Pass    |
|                         | OFF mode turns both headlights off                                           |  Pass    |
|                         | AUTO mode - Headlights turn off after 2s in daylight                         |  Pass    |
|                         | AUTO mode - Headlights turn on after 1s in dusk                              |  Pass    |
|                         | AUTO mode - Headlights maintain state in intermediate light                  |  Pass    |
|                         | Headlights turn off when engine stops                                        |  Pass    |




