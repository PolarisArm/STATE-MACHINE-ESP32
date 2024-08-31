#include <Arduino.h>

class Main;
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


void FIRSTSTATE::nextState(Main *mc)
{
  digitalWrite(mc->getLED_R(),LOW);
  mc->changeState(mc->getSecondState());
  mc->getSecondState()->display(mc->getLED_Y());
}

void SECONDSTATE::nextState(Main *mc)
{  
  digitalWrite(mc->getLED_Y(),LOW);
  mc->changeState(mc->getThirdState());
  mc->getThirdState()->display(mc->getLED_G());
  mc->ThirdStateForBlink()->blinkingLedSetup(mc->getLED_BL(),mc->getLEDBLK_INTERVAL());
}

void THIRDSTATE::nextState(Main *mc)
{
  digitalWrite(mc->getLED_G(),LOW);
  mc->ThirdStateForBlink()->blinkingLedDisable();

  mc->changeState(mc->getfirstState());
  mc->getfirstState()->display(mc->getLED_R());


  delete blink;
  blink = nullptr;
}

void FIRSTSTATE::previousState(Main *mc)
{  
  digitalWrite(mc->getLED_R(),LOW);
  mc->changeState(mc->getThirdState());
  mc->getThirdState()->display(mc->getLED_G());
  mc->ThirdStateForBlink()->blinkingLedSetup(mc->getLED_BL(),mc->getLEDBLK_INTERVAL());
}

void SECONDSTATE::previousState(Main *mc)
{
  digitalWrite(mc->getLED_Y(),LOW);

  mc->changeState(mc->getfirstState());
  mc->getfirstState()->display(mc->getLED_R());

}

void THIRDSTATE::previousState(Main *mc)
{
  digitalWrite(mc->getLED_G(),LOW);
  mc->ThirdStateForBlink()->blinkingLedDisable();
  mc->changeState(mc->getSecondState());
  mc->getSecondState()->display(mc->getLED_Y());

  delete blink;
  blink = nullptr;
}



int RED = 2;
int YELLOW = 4;
int GREEN = 16;

int BLINK = 17;
int UP = 13;
int DW = 14;
Main* mc;

void setup() {
  // put your setup code here, to run once:
   Serial.begin(115200);
   while (!Serial) {
    ; // Wait for serial port to connect. Needed for native USB port only
  }
  mc = new Main(RED,YELLOW,GREEN,BLINK,UP,DW);
  mc->setLEDBLK_INTERVAL(150);
  
}

void loop() {
  // put your main code here, to run repeatedly:
    mc->update();

}

