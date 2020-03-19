#include "Adafruit_MCP23017.h"  // https://github.com/adafruit/Adafruit-MCP23017-Arduino-Library
 
// setup the port expander
Adafruit_MCP23017 mcp0;
volatile long rotation_direction = 0; //CW: 0; CCW:1
volatile long counter = 0;

boolean change=false;        // goes true when a change in the encoder state is detected
int encSelect[2] = {101, 0}; // stores the last encoder used and direction {encNo, 1=CW or 2=CCW}
unsigned long currentTime;
unsigned long loopTime;
 
const int encCount0 = 2;  // number of rotary encoders
// encoder pin connections to MCP23017
//    EncNo { Encoder pinA  GPAx, Encoder pinB  GPAy },
const int encPins0[encCount0][2] = {
  {0,1},   // enc:0 AA GPA0,GPA1 - pins 21/22 on MCP23017
  {14,15}    // enc:1 BB GPA3,GPA4 - pins 24/25 on MCP23017
};  
 
// arrays to store the previous value of the encoders
unsigned char encoders0[encCount0];
 
// read the rotary encoder on pins X and Y, output saved in encSelect[encNo, direct]
unsigned char readEnc(Adafruit_MCP23017 mcpX, const int *pin, unsigned char prev, int encNo) {
 
  unsigned char encA = mcpX.digitalRead(pin[0]);    // Read encoder pins
  unsigned char encB = mcpX.digitalRead(pin[1]);
  if((!encA) && (prev)) { 
    encSelect[0] = encNo;
    if(encB) {
      encSelect[1] = 1;  // clockwise
    }
    else {
      encSelect[1] = 2;  // counter-clockwise
    }
    change=true;
  }
  return encA;
}

 
// setup the encoders as inputs. 
unsigned char encPinsSetup(Adafruit_MCP23017 mcpX, const int *pin) {
 
  mcpX.pinMode(pin[0], INPUT);  // A
  mcpX.pullUp(pin[0], HIGH);    // turn on a 100K pullup internally
  mcpX.pinMode(pin[1], INPUT);  // B
  mcpX.pullUp(pin[1], HIGH); 
}
 
void setup() {  
 
  mcp0.begin(7);    // 0 = i2c address 0x27
 
  // setup the pins using loops, saves coding when you have a lot of encoders and buttons
  for (int n = 0; n < encCount0; n++) {
    encPinsSetup(mcp0, encPins0[n]);
    encoders0[n] = 1;  // default state
  }
 
 
  Serial.begin(115200); 

  currentTime = millis();
  loopTime = currentTime;
}
 
void loop() {
 
  // check the encoders and buttons every 5 millis
  currentTime = millis();
  if(currentTime >= (loopTime + 5)){
 
    for (int n = 0; n < encCount0; n++) {
      encoders0[n] = readEnc(mcp0, encPins0[n], encoders0[n],n);
    }
 
    loopTime = currentTime;  // Updates loopTime
  } 
  
  // when an encoder has been rotated
  if (change == true) {
 
    if (encSelect[0] < 100) {
      Serial.print("Enc: ");
      Serial.print(encSelect[0]);  
      Serial.print(" " );
 
      switch (encSelect[1]) {
        case (1): // clockwise
          // reset counter for each direction change
          if (rotation_direction != 0) {
            counter = 0;
            rotation_direction = 0;
          }
          Serial.print("Direction: CW ");
          Serial.print("Tick: ");
          Serial.println(counter);
          counter++;
          break;
        case (2): // counter-clockwise
          // reset counter for each direction change
          if (rotation_direction != 1) {
            counter = 0;
            rotation_direction = 1;
          }
          Serial.print("Direction: CCW ");
          Serial.print("Tick: ");
          Serial.println(counter);
          counter++;
          break;
      }
      // set the selection to 101 now we have finished doing things. Not 0 as there is an encoder 0.
      encSelect[0] = 101;
    }
    // ready for the next change
    change = false;

    if (counter >= 600) {
      counter=0;
    }
  }
}
