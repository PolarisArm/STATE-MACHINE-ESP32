# STATE-MACHINE-ESP32
## TRYING STATE MACHINE WITH ESP32
# Arduino LED State Machine

This project demonstrates a state machine implementation using an Arduino to control multiple LEDs in different states. The state machine includes three states, with each state controlling a different LED. The third state also features a blinking LED controlled by a timer.

## Block Diagram

```plaintext
                +------------------+
                |      Main        |
                |  (State Manager) |
                +-------+----------+
                        |
                        v
                +-------+----------+
                |     Base State   |
                +--+-----------+---+
                   |           |
       +-----------+           +-------------+
       |                                       |
       v                                       v
  +----+-----+                           +-----+-----+
  | First     |                           | Second    |
  |  State    |-------------------------->|  State    |
  +----+------+                           +------+-----+
       |                                        |
       |                                        v
       |                                 +------+------+
       |                                 |   Third     |
       |                                 |   State     |
       |                                 +------+------+
       |                                        |
       |                                        v
       +---------------------------------->+----+----+
                                           | BLINKING |
                                           |(LED Blink)|
                                           +-----------+
```
***State Transitions***
**First State:** Turns on the RED LED. Can transition to the Second <br> State.
**Second State:** Turns on the YELLOW LED. Can transition to the Third State. <br>
**Third State:** Turns on the GREEN LED and starts blinking the BLINK LED. Can transition back to <br> the First State or Second State.
## Overview
### The state machine is controlled using two buttons:

__UP Button:__ Moves to the next state.
__DOWN Button:__ Moves to the previous state.

### The behavior of each state is as follows:
<br>
__First State:__ Turns on the RED LED.
__Second State:__ Turns on the YELLOW LED.
__Third State:__ Turns on the GREEN LED and blinks another LED (BLINK).
<br>

__Components__

Main Class: Manages the state machine and transitions between states.
Base Class: Abstract base class for defining state behavior.
FirstState, SecondState, ThirdState Classes: Implement the specific logic for each state.
BLINKING Class: Handles the LED blinking functionality in the third state.

Code Explanation
Main Class
The Main class handles:

Initializing LEDs and buttons.
Managing the current state and transitioning between states using the UP and DOWN buttons.
Updating the state machine through the update() method.

State Classes
FirstState: Represents the first state, where the RED LED is turned on.
SecondState: Represents the second state, where the YELLOW LED is turned on.
ThirdState: Represents the third state, where the GREEN LED is turned on and another LED blinks 

using the BLINKING class.
BLINKING Class
The BLINKING class is used to blink an LED at a defined interval using a timer. It keeps track of the current LED state and toggles it on or off based on the elapsed time.

Usage
Setup: Load the code onto an Arduino board.
Operation: Press the UP button to move to the next state, and the DOWN button to move to the previous state. The LEDs will illuminate or blink according to the active state.

Requirements

Hardware:

Arduino board (e.g., Arduino Uno, Nano)
3 Standard LEDs (RED, YELLOW, GREEN)
1 LED for blinking (BLINK)
Resistors for LEDs (e.g., 220Ω)
2 Push buttons for state transitions (UP and DOWN)
Breadboard and jumper wires

Circuit Connections
RED LED: Connected to digital pin 2
YELLOW LED: Connected to digital pin 4
GREEN LED: Connected to digital pin 16
BLINK LED: Connected to digital pin 17
UP Button: Connected to digital pin 13 with pull-up resistor
DOWN Button: Connected to digital pin 14 with pull-up resistor

ESP32 Pin Layout:
-------------------
+5V  >---+         +-----> [RED LED]    ---GND
         |         +-----> [YELLOW LED] ---GND
         |         +-----> [GREEN LED]  ---GND
         |         +-----> [BLINK LED]  ---GND
       [BUTTON UP]  >----+   [BUTTON DOWN] >----+
GND   >---+              GND   >---+          GND
