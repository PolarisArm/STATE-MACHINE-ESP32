#include <Arduino.h>

class Main;
class BLINKING;
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
       void previousState(Main *mc) override { }
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
        blink = new BLINKING(LED,150);

        }
        LED_INSIDE_CLASS = LED;
        digitalWrite(LED,HIGH);
        Serial.print("THIRD STATE : LED IS HIGH LED NAME >> ");
        Serial.println(LED);
       }
};

class BLINKING{
    public:
      int _LED_BLK;
      unsigned long _interval;
      unsigned long currentMillis, previousMillis = 0;

    public:
      BLINKING(int LED_BLK, unsigned long interval = 250) : _LED_BLK(LED_BLK),_interval(interval)
      {
        Serial.println("BLINKING CLASS HAS BEEN CREATED");
        pinMode(_LED_BLK,OUTPUT);
      }

      void blinking()
      {
        currentMillis = millis();
        bool ledglow = true;
        if(currentMillis - previousMillis >= _interval)
        {
          ledglow != ledglow;
          previousMillis = currentMillis;
        }
        digitalWrite(_LED_BLK,ledglow);
      }

      ~BLINKING(){ }

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
      unsigned long interval = 100;
  public:
      Main(int R, int Y, int G ,int BL, int UP , int DW) : _LED_R(R),_LED_Y(Y),_LED_G(G),
                                                   _LED_BL(BL), _UP(UP), _DW(DW)
      {
        Serial.println("FIRST MAIN IS CREATED");
        pinMode(_LED_R,INPUT);
        pinMode(_LED_Y,INPUT);
        pinMode(_LED_G,INPUT);
        pinMode(_LED_BL,INPUT);

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

      }

      void changeState(Base *state)
      {
        activeState = state;
      }

      Base* getfirstState()
      {
        return firstSate;
      }
       Base* getSecondState()
      {
        return secondState;
      }
       Base* getThirdState()
      {
        return thirdState;
      }

      int getLED_R()
      {
        return _LED_R;
      }

      int getLED_Y()
      {
        return _LED_Y;
      }

      int getLED_G()
      {
        return _LED_G;
      }

      int getLED_BL()
      {
        return _LED_BL;
      }

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
}

void THIRDSTATE::nextState(Main *mc)
{
  digitalWrite(mc->getLED_G(),LOW);
  mc->changeState(mc->getfirstState());
  mc->getfirstState()->display(mc->getLED_R());

  delete blink;
  blink = nullptr;
}



void SECONDSTATE::previousState(Main *mc)
{
  mc->changeState(mc->getfirstState());
}

void THIRDSTATE::previousState(Main *mc)
{
  mc->changeState(mc->getSecondState());
}

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
}

