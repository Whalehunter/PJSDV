#include <Wire.h>

#define I2C_SDL    D1
#define I2C_SDA    D2
  
void setup(void) {
  pinMode(D4, OUTPUT);  //Set D5 as Output.
  pinMode(D5, OUTPUT);  //Set D5 as Output.
  
  Wire.begin();  //Start wire.
  Serial.begin(115200);  //Set serial Baud.

  int i = 0;  //Define i for for loops.
}

void loop(void) {
  int i = 0;
  digitalWrite(D4, HIGH);  //LED D4 ON.
  for(i; i<1024; i+=10){  //Go through 0 / 1024.
    analogWrite(D5, i);  //Deur Output pin D5, <1-1024>.
    Serial.println(i);  //Serial print value of i.
    delay(100);  //Wait for 0.1s.
  }
  
  digitalWrite(D4, LOW);  //LED D4 OFF.
  for(i; i>0; i-=10){  //Go through 1024 / 0.
    analogWrite(D5, i);  //Deur Output pin D5, <1-1024>.
    Serial.println(i);  //Serial print value of i.
    delay(100);  //Wait for 0.1s.
  }
  /*
  Wire.beginTransmission(0x38);  //I2C Address.
  Wire.write(byte(0x03));  //Set outputs.            
  Wire.write(byte(0xFF));  //Turn on LEDs.          
  Wire.endTransmission();  //End I2C transmission.*/
}
