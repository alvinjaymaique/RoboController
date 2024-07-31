#include <Arduino.h>;
#include <SoftwareSerial.h>;
#include <string.h>

#define ledPin 4

const byte rxPin = 10;
const byte txPin = 11;
SoftwareSerial BTSerial(rxPin, txPin); //RX TX

void setup() {
  // put your setup code here, to run once:
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  // Serial.begin(38400);
  // BTSerial.begin(38400); //AT Command Debug mode

  Serial.begin(9600);
  BTSerial.begin(9600);
}


// String receivedData = "";
// float x_value = 0;
// float y_value = 0;

// bool isValueX = false;
// int logic = 0;
// long prevMillis = 0;
// void loop() {
//   // put your main code here, to run repeatedly:
//   bool isNegative = false;
  

//   while(BTSerial.available() > 0){
//     char dataChar = BTSerial.read();
//     // isValueX = (dataChar=='x')?true:(dataChar=='y')?false:isValueX; 
//     if(dataChar=='x'){
//       isValueX = true;
//       receivedData = "";
//     }else if(dataChar=='y'){
//       isValueX = false;
//     }

//     Serial.println(dataChar);

//     if(dataChar != '\\'){
//       if (dataChar != '-' && dataChar !='x' && dataChar != 'y') {
//         receivedData += dataChar;
//       }else if(dataChar == '-'){
//         isNegative = true;
//         Serial.println("The value is negative");
//       }
      
//     }else if(dataChar == '\\'){
//       if(isValueX){
//         x_value = receivedData.toFloat();
//         x_value = (isNegative)?x_value*-1:x_value;
//         Serial.print("X: ");
//         Serial.print(isNegative);
//         Serial.println(x_value);
//       }else if(!isValueX){
//         y_value = receivedData.toFloat();
//         y_value = (isNegative)?y_value*-1:y_value;
//         Serial.print("Y: ");
//         Serial.println(y_value);
//         BTSerial.flush();
//       }
      
//       receivedData = "";
//       // isValueX = false;
//       isNegative = false;
//       break;
//     }
//   }
//   // Serial.print("X: ");
//   // Serial.println(x_value);
// }



String receivedData = "";
float x_value = 0;
float y_value = 0;
bool isValueX = false;
bool isNegative = false;
int maxSpeed = 200;
int speed = int(y_value);
int m1Speed = 0; // Left Motor
int m2Speed = 0; // Right Motor

void loop() {
  while (BTSerial.available() > 0) {
    char dataChar = BTSerial.read();

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
    } else if (dataChar == '\\') {
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
    m1Speed = min(speed, speed*(1-x_value));
    m2Speed = min(speed, speed*(1+x_value));
  }
  // Serial.print("X: ");
  // Serial.println(x_value);

  // Serial.print("Y: ");
  // Serial.println(y_value);


  Serial.print("Left Motor Speed: ");
  Serial.println(m1Speed);

  Serial.print("Right Motor Speed: ");
  Serial.println(m2Speed);

}