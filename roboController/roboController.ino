#include <Arduino.h>;
#include <SoftwareSerial.h>;
#include <string.h>



// Bluetooth pins
const byte rxPin = 10;
const byte txPin = 11;
SoftwareSerial BTSerial(rxPin, txPin); //RX TX

#define mLeft1 2
#define mLeft2 4
#define mLeftPwm 3

#define mRight1 5
#define mRight2 7
#define mRightPwm 6

#define hornPin 8

void run(int a, int b, int speed, int pwm);

void setup() {
  // put your setup code here, to run once:
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  // Serial.begin(38400);
  // BTSerial.begin(38400); //AT Command Debug mode

  for(int i=2; i<9; i++){
    pinMode(i, OUTPUT);
  }
  // pinMode(9, OUTPUT);

  Serial.begin(9600);
  BTSerial.begin(9600);
}

String receivedData = "";
float x_value = 0;
float y_value = 0;
bool isValueX = false;
bool isNegative = false;
bool isBoostClicked = false;
String isBoostOn = "";
int boostValue = 40;

bool isHornClicked = false;
String isHornOn = "";
bool activateHorn = false;

// bool isForward = false;
int maxSpeed = 180;
int speed = int(y_value);
int m1Speed = 0; // Left Motor
int m2Speed = 0; // Right Motor

void loop() {
  while (BTSerial.available() > 0) {
    char dataChar = BTSerial.read();
    // Serial.println(dataChar);
    if (dataChar == 'x') {
      isValueX = true;
      receivedData = ""; 
      isNegative = false; 
    } else if (dataChar == 'y') {
      isValueX = false;
      receivedData = ""; 
      isNegative = false; 
    } else if (dataChar == '-') {
      isNegative = true; 
      // isForward = (!isNegative)?true:false;
    } else if ( dataChar == 'b'){   
      isBoostClicked = true;
      Serial.println(isBoostClicked);
    } else if (isBoostClicked) {
      isBoostOn = dataChar;
      // Serial.println(isBoostOn);
      isBoostClicked = false;
    } else if (dataChar == 'h'){   
      isHornClicked = true;
      Serial.println(isHornClicked);
    } else if (isHornClicked) {
      isHornOn = dataChar;
      // Serial.println(isBoostOn);
      isHornClicked = false;
    } 
    else if (dataChar == '\\') {
      // Process the received data
      float value = receivedData.toFloat();
      value = min(value, 1.00);
      value = isNegative ? -value : value; 
      if (isValueX) {
        x_value = value;       
        // Serial.print("X: ");
        // Serial.println(x_value);
      } else {
        y_value = value;
        // Serial.print("Y: ");
        // Serial.println(y_value);
      }
      receivedData = "";
      isNegative = false; 
    } else {
      receivedData += dataChar;
    }
    speed = int(y_value*maxSpeed);
    if(isBoostOn == "t" && speed > 0){
      speed += boostValue;
    }else if(isBoostOn == "f" && speed > boostValue){
      speed -= boostValue;
    }


    if(isHornOn == "t"){
      activateHorn = true;
    }else if (isHornOn == "f") {
      activateHorn = false;
    }

    m1Speed = adjustSpeed(speed, speed*(1-x_value));
    m2Speed = adjustSpeed(speed, speed*(1+x_value));
  }
  // Serial.print("X: ");
  // Serial.println(x_value);

  // Serial.print("Y: ");
  // Serial.println(y_value);


  // Serial.print("Left Motor Speed: ");
  // Serial.println(m1Speed);

  // Serial.print("Right Motor Speed: ");
  // Serial.println(m2Speed);

  // if(isHornOn == "t"){
  //   digitalWrite(hornPin, 1);
  // }else if (isHornOn == "f") {
  //   digitalWrite(hornPin, 0);
  // }
  digitalWrite(hornPin, activateHorn);

  run(mLeft1, mLeft2, m1Speed, mLeftPwm);
  run(mRight1, mRight2, m2Speed, mRightPwm);
}

int adjustSpeed(int speed, int modified_speed){
  if(speed<0){
    return max(speed, modified_speed);
  }
  return min(speed, modified_speed);
}

void run(int a, int b, int speed, int pwm) {
  bool direction;
  if(speed<0){
    speed *= -1;
    direction = true;
  }
  else{
    direction = false;
  }
  digitalWrite(a, direction);
  digitalWrite(b, !direction);
  analogWrite(pwm, speed);
}
