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
                           | 
                           |              
                      +----+-----+                             +-----+-----+
                      | First     |                            | Second    |
                      |  State    |<-------------------------->|  State    |
                      +----+------+                            +------+----+
                           |                                        |
                           |                                        v
                           |                                 +------+------+
                           |                                 |   Third     |
                           |                                 |   State     |
                           |                                 +------+------+
                           |                                        |
                           |                                        v
                           +<---------------------------------->+----+-----+
                                                               | BLINKING  |
                                                               |(LED Blink)|
                                                               +-----------+
```
***State Transitions***
**First State:** Turns on the RED LED. Can transition to the Second <br> State.
**Second State:** Turns on the YELLOW LED. Can transition to the Third State. <br>
**Third State:** Turns on the GREEN LED and starts blinking the BLINK LED. Can transition back to <br> the First State or Second State.
## Overview
### The state machine is controlled using two buttons:

__UP Button:__ Moves to the next state. <br>
__DOWN Button:__ Moves to the previous state. <br>

### The behavior of each state is as follows:
<br>
__First State:__ Turns on the RED LED. <br>
__Second State:__ Turns on the YELLOW LED. <br>
__Third State:__ Turns on the GREEN LED and blinks another LED (BLINK). <br>
<br>

__Components__ <br>

Main Class: Manages the state machine and transitions between states. <br>
Base Class: Abstract base class for defining state behavior. <br>
FirstState, SecondState, ThirdState Classes: Implement the specific logic for each state. <br>
BLINKING Class: Handles the LED blinking functionality in the third state. <br>

__Code Explanation__
__Main Class__
```C++
class Main{
  private:
      Base *activeState;
      FIRSTSTATE *firstSate = new FIRSTSTATE;
      SECONDSTATE *secondState = new SECONDSTATE;
      THIRDSTATE *thirdState = new THIRDSTATE;
    
      int _LED_R;
      int _LED_Y;
      int _LED_G;
      int _LED_BL;

      int _UP;
      int _DW;

      unsigned long currentMillis, previousMillis = 0;
      unsigned long interval = 500;
      unsigned long _blkInterval = 250;
  public:
      Main(int R, int Y, int G ,int BL, int UP , int DW) : _LED_R(R),_LED_Y(Y),_LED_G(G),
                                                   _LED_BL(BL), _UP(UP), _DW(DW)
      {
        Serial.println("FIRST MAIN IS CREATED");
        pinMode(_LED_R,OUTPUT);
        pinMode(_LED_Y,OUTPUT);
        pinMode(_LED_G,OUTPUT);
        pinMode(_LED_BL,OUTPUT);

        pinMode(_UP,INPUT_PULLUP);
        pinMode(_DW,INPUT_PULLUP);

        activeState = firstSate;
        activeState->display(_LED_R);
      }

      void update()
      {
        currentMillis = millis();

        if(digitalRead(_UP) == LOW)
        {
          if(currentMillis - previousMillis >= interval)
          {
            if(digitalRead(_UP) == LOW)
            {
              activeState->nextState(this);
            }
            previousMillis = currentMillis;
          }
        }

         if(digitalRead(_DW) == LOW)
        {
          if(currentMillis - previousMillis >= interval)
          {
            if(digitalRead(_DW) == LOW)
            {
              activeState->previousState(this);
            }
            previousMillis = currentMillis;
          }
        }
    if (activeState == thirdState && thirdState->blink) {
        thirdState->blink->blinking();
      }
      }

      void setLEDBLK_INTERVAL(unsigned long blkInterval) {  _blkInterval = blkInterval; }

      unsigned long getLEDBLK_INTERVAL() { return _blkInterval;  }

      void changeState(Base *state) { activeState = state; }

      Base* getfirstState(){ return firstSate; }
      
      Base* getSecondState() { return secondState; }
      
      Base* getThirdState() { return thirdState; }

      THIRDSTATE *ThirdStateForBlink() { return thirdState;  }

      int getLED_R() { return _LED_R;  }

      int getLED_Y() {  return _LED_Y; }

      int getLED_G() {  return _LED_G; }

      int getLED_BL() { return _LED_BL; }

      ~Main()
      {
        delete firstSate;
        delete secondState;
        delete thirdState;
        Serial.println("Main is being deleted");
      }

};

```
The Main class handles:

Initializing LEDs and buttons. <br>
Managing the current state and transitioning between states using the UP and DOWN buttons.<br>
Updating the state machine through the update() method.<br>

__State Classes__ <br>

```C++
class Base{

  public:
    virtual void display(int LED) = 0;
    virtual void nextState(Main *mc) = 0;
    virtual void previousState(Main *mc) = 0;

    virtual  ~Base(){
      Serial.println("Base is Deleted ");
    }
};


class FIRSTSTATE : public Base{
      private:
          int LED_INSIDE_CLASS;

      public:
       void previousState(Main *mc) override;
       void nextState(Main *mc) override;
       void display(int LED)
       {
        LED_INSIDE_CLASS = LED;
        digitalWrite(LED,HIGH);
        Serial.print("FIRST STATE : LED IS HIGH LED NAME >> ");
        Serial.println(LED);
       }
};

class SECONDSTATE : public Base{
    private:
      int LED_INSIDE_CLASS;

      public:
       void previousState(Main *mc) override;
       void nextState(Main *mc) override;
       void display(int LED)
       {
        LED_INSIDE_CLASS = LED;
        digitalWrite(LED,HIGH);
        Serial.print("SECOND STATE : LED IS HIGH LED NAME >> ");
        Serial.println(LED);
       }
};


class THIRDSTATE : public Base{
      private:
          int LED_INSIDE_CLASS;
      public:
       
      BLINKING *blink = nullptr;
        
       void previousState(Main *mc) override;
       void nextState(Main *mc) override;
       void display(int LED)
       {
        if(!blink)
        {
        blink = new BLINKING;
        }

        LED_INSIDE_CLASS = LED;
        digitalWrite(LED,HIGH);
        Serial.print("THIRD STATE : LED IS HIGH LED NAME >> ");
        Serial.println(LED);
       }

       void blinkingLedSetup(int _LED_BLK, unsigned long interval)
       {
        if(blink)
        {
          blink->BlinkSetup(_LED_BLK,interval);
        }       
        else
        {
          Serial.println("Blink is not initialized");
        }
       }

        void blinkingLedDisable()
        {
          blink->blinkingDisable();
        }
};
```
FirstState: Represents the first state, where the RED LED is turned on. <br>
SecondState: Represents the second state, where the YELLOW LED is turned on. <br>
ThirdState: Represents the third state, where the GREEN LED is turned on and another LED blinks  <br>

__using the BLINKING class.__<br>
BLINKING Class<br>
```C++
class BLINKING{
    public:
      int _LED_BLK;
      unsigned long _interval;
      unsigned long currentMillis, previousMillis = 0;
      bool ledglow = true;

    public:
      BLINKING()
      {
        Serial.println("BLINKING CLASS HAS BEEN CREATED");
      }

      void BlinkSetup(int LED_BLK,unsigned long interval)
      {
        _LED_BLK = LED_BLK;
        _interval = interval;
        pinMode(_LED_BLK,OUTPUT);
      }

      void blinking()
      {
        currentMillis = millis();
        if(currentMillis - previousMillis >= _interval)
        {
          ledglow = !ledglow;
          previousMillis = currentMillis;
        }
        digitalWrite(_LED_BLK,ledglow);
      }

      void blinkingDisable()
      {
        digitalWrite(_LED_BLK, LOW);
      }
      ~BLINKING(){ }

};
```
The BLINKING class is used to blink an LED at a defined interval using a timer. It keeps track of the current LED state and toggles it on or off based on the elapsed time.<br>

__Usage__<br>
Setup: Load the code onto an Arduino board.<br>
Operation: Press the UP button to move to the next state, and the DOWN button to move to the previous state. The LEDs will illuminate or blink according to the active state.<br>

__Requirements__<br>

__Hardware:__<br>

Arduino board (e.g., Arduino Uno, Nano) <br>
3 Standard LEDs (RED, YELLOW, GREEN) <br>
1 LED for blinking (BLINK) <br>
Resistors for LEDs (e.g., 220Ω) <br>
2 Push buttons for state transitions (UP and DOWN) <br>
Breadboard and jumper wires <br>

Circuit Connections <br>
RED LED: Connected to digital pin 2 <br>
YELLOW LED: Connected to digital pin 4 <br>
GREEN LED: Connected to digital pin 16 <br>
BLINK LED: Connected to digital pin 17 <br>
UP Button: Connected to digital pin 13 with pull-up resistor <br>
DOWN Button: Connected to digital pin 14 with pull-up resistor <br>
```plaintext
ESP32 Pin Layout:
-------------------
+5V  >---+         +-----> [RED LED]    ---GND 
         |         +-----> [YELLOW LED] ---GND 
         |         +-----> [GREEN LED]  ---GND 
         |         +-----> [BLINK LED]  ---GND 
       [BUTTON UP]  >----+   [BUTTON DOWN] >----+ 
GND   >---+              GND   >---+          GND
```
