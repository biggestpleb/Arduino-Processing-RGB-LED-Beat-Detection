/* This sketch is designed to take HSI/RGB inputs and output RGB PWM
 * Commands are in the form [Letter][number]
 * e.g to set hue to 200 the input is "h200", you can chain commands, e.g. "h200 s0.4 i0.8"
 * Commands can be input manually using the serial monitor or from another program
 * 
 * Additional commands: ? - random hue, u - undo last change
 */

#include<math.h>

int rgbPin[3] = {11, 10, 9}; //the pins for Red, Green and Blue respectively.
int rgbInput[3] = {0};
int rgbVal[3] = {0};
float hsiVal[3] = {0};
int prevVal[3] = {0};
int mode = 0;

unsigned long startTime,stopTime, time;
float fadeRate = 0.1;

void setup() {
  
  pinMode(rgbPin[0], OUTPUT);
  pinMode(rgbPin[1], OUTPUT);
  pinMode(rgbPin[2], OUTPUT);

  //inititial val
  hsiVal[0] = 31.0;
  hsiVal[1] = 0.400;
  hsiVal[2] = 0.600;
  hsi2rgb(hsiVal[0],hsiVal[1],hsiVal[2],rgbInput);
  setVal(rgbInput[0], rgbInput[1], rgbInput[2]);
  rgbWrite();
  printVals();
  Serial.begin(57600);
  while(!Serial);
  Serial.setTimeout(100);
  Serial.println(F("Initialised"));
  time = millis();
  
}

void loop() {
  byte updateFlag = 0;
  byte hsiFlag = 0;
  while (Serial.available() > 0) {
    //startTime = micros();
    char inputChar = Serial.read();
    if(!updateFlag){
      delayMicroseconds(100);
      rgbInput[0] = -1;
      rgbInput[1] = -1;
      rgbInput[2] = -1;
      startTime = micros();
    }

    if (inputChar == 'r') {
      rgbInput[0] = Serial.parseInt();
    } else if (inputChar == 'g') {
      rgbInput[1] = Serial.parseInt();
    } else if (inputChar == 'b') {
      rgbInput[2] = Serial.parseInt();
    } else if (inputChar == 'u') {
      undo();
      Serial.println("Undo");
    } else if (inputChar == 'h') {
      hsiVal[0] = Serial.parseFloat();
      hsiFlag = 1;
    } else if (inputChar == 's') {
      hsiVal[1] = Serial.parseFloat();
      hsiFlag = 1;
    } else if (inputChar == 'i') {
      hsiVal[2] = Serial.parseFloat();
      hsiFlag = 1;
    } else if (inputChar == '?') {
      hsiVal[0] = ((int)hsiVal[0]+random(60,241))%360;
      hsiFlag = 1;
    } else if (inputChar == 'f') {
      fadeRate = Serial.parseFloat();
      fadeRate = constrain(fadeRate,0,359);
      if(mode == 1){
        mode = 0;
      } else {
        mode = 1; 
      }
    } else if (inputChar == 'e') {
      int scaleFactor = Serial.parseInt();
      scaleFactor = constrain(scaleFactor,0,255);
      int largestVal = findLargest();
      rgbInput[0] = (int)(((unsigned int)rgbVal[0]*scaleFactor)/largestVal);
      rgbInput[1] = (int)(((unsigned int)rgbVal[1]*scaleFactor)/largestVal);
      rgbInput[2] = (int)(((unsigned int)rgbVal[2]*scaleFactor)/largestVal);
      Serial.print(F("Scaled to "));
      Serial.println(scaleFactor);
    } else if (inputChar == '/') {
      rgbInput[0] = 0;
      rgbInput[1] = 0;
      rgbInput[2] = 0;
      mode = 0;
    }
    updateFlag = 1;
    delayMicroseconds(100);
  }

  if (updateFlag) {
    if(hsiFlag){
      hsi2rgb(hsiVal[0],hsiVal[1],hsiVal[2],rgbInput); 
    }
    setVal(rgbInput[0], rgbInput[1], rgbInput[2]);
    printVals();
    rgbWrite();
    stopTime = micros();
    Serial.print(stopTime - startTime);
    Serial.println("us");
  }
  
  if((mode==1)&&(millis()-time>10)){
    hsiVal[0] += fadeRate;// /(float)sqrt(((int)hsiVal+60)%60);
    if(hsiVal[0]>360){
      hsiVal[0] -= 360; 
    }
    hsiWrite();
    time = millis();
  }
  
}

void rgbWrite() {
  for (int i = 0; i < 3; i++) {
    rgbVal[i] = constrain(rgbVal[i], 0, 255);
    analogWrite(rgbPin[i], rgbVal[i]);
  }
}

void printVals() {
  Serial.print("H=");
  Serial.print(hsiVal[0]);
  Serial.print(" S=");
  Serial.print(hsiVal[1]);
  Serial.print(" I=");
  Serial.println(hsiVal[2]);
  
  Serial.print("r=");
  Serial.print(rgbVal[0]);
  Serial.print(" g=");
  Serial.print(rgbVal[1]);
  Serial.print(" b=");
  Serial.println(rgbVal[2]);
}

void undo() {
  rgbVal[0] = prevVal[0];
  rgbVal[1] = prevVal[1];
  rgbVal[2] = prevVal[2];
}

void hsiWrite(){
  hsi2rgb(hsiVal[0],hsiVal[1],hsiVal[2],rgbVal); 
  rgbWrite();
}

void setVal(int rVal, int gVal, int bVal) {
  //if there are valid inputs, save state
  if ((rVal != -1) || (gVal != -1) || (bVal != -1)) {
    prevVal[0] = rgbVal[0];
    prevVal[1] = rgbVal[1];
    prevVal[2] = rgbVal[2];
  }
  if ((rVal > -1)) {
    rgbVal[0] = rVal;
  }
  if ((gVal > -1)) {
    rgbVal[1] = gVal;
  }
  if ((bVal > -1)) {
    rgbVal[2] = bVal;
  }
}

void writeVal(int rVal, int gVal, int bVal) {
  setVal(rVal, gVal, bVal);
  rgbWrite();
}

int findLargest(){
  int largest = rgbVal[0];
  if(rgbVal[1]>largest){
    largest = rgbVal[1];
  } 
  if(rgbVal[2]>largest){
    largest = rgbVal[2];
  }
  return largest;
}

//convert HSI color space to RGB
void hsi2rgb(float H, float S, float I, int* rgb) {
  const int rMax = 360;
  const int gMax = 270;
  const int bMax = 240;
  
  int r, g, b;
  H = fmod(H,360); // cycle H around to 0-360 degrees
  
  H = 3.14159*H/(float)180; // Convert to radians.
  S = S>0?(S<1?S:1):0; // clamp S and I to interval [0,1]
  I = I>0?(I<1?I:1):0;
    
  // Math! Thanks in part to Kyle Miller.
  if(H < 2.09439) {
    r = rMax*I/3*(1+S*cos(H)/cos(1.047196667-H));
    g = gMax*I/3*(1+S*(1-cos(H)/cos(1.047196667-H)));
    b = bMax*I/3*(1-S);
  } else if(H < 4.188787) {
    H = H - 2.09439;
    g = gMax*I/3*(1+S*cos(H)/cos(1.047196667-H));
    b = bMax*I/3*(1+S*(1-cos(H)/cos(1.047196667-H)));
    r = rMax*I/3*(1-S);
  } else {
    H = H - 4.188787;
    b = bMax*I/3*(1+S*cos(H)/cos(1.047196667-H));
    r = rMax*I/3*(1+S*(1-cos(H)/cos(1.047196667-H)));
    g = gMax*I/3*(1-S);
  }
  rgb[0]=r;
  rgb[1]=g;
  rgb[2]=b;
}
