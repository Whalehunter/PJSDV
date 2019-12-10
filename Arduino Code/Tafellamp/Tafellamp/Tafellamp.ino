#include <Wire.h>

#define I2C_SDL   D1
#define I2C_SDA   D2

int leesPIR(); 

void setup() {
  Wire.begin();//Start wire
  Serial.begin(115200);//Set serial Baud
}

void loop() {
  int sensor = leesPIR();
  Serial.print("PIR waarde: ");
  Serial.println(sensor);
}

int leesPIR(){
  Wire.beginTransmission(0x38); 
  Wire.write(byte(0x00));
  Wire.endTransmission();  
  Wire.requestFrom(0x38, 1);          
  int reading = Wire.read();
  return reading;
}
