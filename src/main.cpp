#include <Arduino.h>
#include <MouseTo.h>

#define Enable 10 // Enable running
//#define TopRight 2  // Pin 1 brown touch connector
#define TopLeftPNP 13 // Pin 2 red
#define TopLeftNPN 4  // Pin 2 red
#define SenseValue A0 // Pin 3 orange
//#define BottomLeft 5 // Pin 5 green
#define BottomRightPNP 6  // Pin 4 yellow
#define BottomRightNPN 7  // Pin 4 yellow

#define SETTLE_TIME (unsigned long)10 // 10ms

#define ILEGAL_X_VALLUE 50  // if x value is less than 50 its ilegal
#define MIN_DIFFERENCE 2
#define MAX_DIFFERENSE 10
#define CLICK_MAX_VALUES 10

#define MAX_X 824
#define MAX_Y 918
#define DISPLAY_X 1024    // 
#define DISPLAY_Y 768

float ScaleFactorX, ScaleFactorY;
int SavedX_Value, ClickX_Value, ClickY_Value;
int LastX_Value = -1;
int LastY_Value = -1;
int V_CounterX = 0;
bool FistValue = true;
bool IdleFlag;
bool Click = false;

unsigned long PanelTime, temp, ClickDelay;
int Xpos, Ypos;
unsigned int sum;


void setup() {
  pinMode(Enable, INPUT_PULLUP);
  pinMode(TopLeftNPN, OUTPUT);
  //pinMode(TopRight, OUTPUT);
  //pinMode(BottomLeft, OUTPUT);
  pinMode(BottomRightPNP, OUTPUT);
  pinMode(BottomRightNPN, OUTPUT);

  //digitalWrite(TopRight; OUTPUT);
  //digitalWrite(BottomLeft, LOW);

  Mouse.begin();
  MouseTo.setCorrectionFactor(0.9433);

  ScaleFactorX = (float)DISPLAY_X / (float)MAX_X;
  ScaleFactorY = (float)DISPLAY_Y / (float)MAX_Y;

  Serial.begin(115200);
  Serial.println("Start");
  
}

void loop() {
   while(digitalRead(Enable)){  // Running program is allowed
     // Setup X reading
    digitalWrite(TopLeftPNP, LOW);
    digitalWrite(TopLeftNPN, LOW);
    digitalWrite(BottomRightPNP, HIGH);
    digitalWrite(BottomRightNPN, HIGH);

    PanelTime = millis();
    // Wait for panel to settle
    do{
       temp = millis() - PanelTime;
      }while (temp < SETTLE_TIME);

    // Read X position
    Xpos = (int)(ScaleFactorX * (float)(analogRead(SenseValue)-75));

    // Setup Y reading
    digitalWrite(TopLeftPNP, HIGH);
    digitalWrite(TopLeftNPN, HIGH);
    digitalWrite(BottomRightPNP, LOW);
    digitalWrite(BottomRightNPN, LOW); 

    PanelTime = millis();
    // Wait for panel to settle
    do{
       temp = millis() - PanelTime;
      }while (temp < SETTLE_TIME);

    // Read Y position
    Ypos = (int)(ScaleFactorY * (float)(analogRead(SenseValue)-75));
 
    if(Xpos < ILEGAL_X_VALLUE){
      IdleFlag = true;
    }else{
      if(IdleFlag){ // This is the first legal value
        IdleFlag = false;
        V_CounterX = 1;
      }else if(!Click){ // Check values
        if(Xpos - LastX_Value > MIN_DIFFERENCE && abs(Xpos -LastX_Value) < MAX_DIFFERENSE){ // Finger is pressed and still
          V_CounterX++;

        }else{  // Finger is lifted
          if(V_CounterX >=1 && V_CounterX <= 6){
            Serial.print(V_CounterX); Serial.print('-');Serial.println(LastX_Value);
          }

        }
      }else{  // Handle Click
 
      }
    }
    LastX_Value = Xpos;
    LastY_Value = Ypos;
  } // end of - Running program is allowed
}