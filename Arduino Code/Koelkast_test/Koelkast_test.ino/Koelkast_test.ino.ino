#include <Wire.h>

#define I2C_SDL    D1
#define I2C_SDA    D2
  
void setup(void) {
  pinMode(D4, OUTPUT);  //Set D5 as Output.
  pinMode(D5, OUTPUT);  //Set D5 as Output.
  
  Wire.begin();  //Start wire.
  Serial.begin(115200);  //Set serial Baud.
}

void loop(void) {
  digitalWrite(D4, HIGH);  //LED D4 ON.
  delay(500); 

  analogWrite(D5, 255);  //
  
  Wire.beginTransmission(0x38); 
  Wire.write(byte(0x00));      
  Wire.endTransmission();
  Wire.requestFrom(0x38, 1);  //request data from 0x38   
  unsigned int inputs = Wire.read();//Write value on address of inputs.
  Serial.print("Digital in: "); 
  Serial.println(inputs&0x01); 

  Wire.beginTransmission(0x38);
  Wire.write(byte(0x03));          
  Wire.write(byte(0x0F));         
  Wire.endTransmission();

  Wire.beginTransmission(0x38);
  Wire.write(byte(0x01));           
  Wire.write(byte(0x10)); //Turn on fan.         
  Wire.endTransmission();

  Wire.beginTransmission(0x36);
  Wire.write(byte(0xA2));          
  Wire.write(byte(0x03));  
  Wire.endTransmission(); 
  
  //Read analog 10bit inputs 0&1
  Wire.requestFrom(0x36, 4);   
  unsigned int anin0 = Wire.read()&0x03;  
  anin0=anin0<<8;
  anin0 = anin0|Wire.read();  
  unsigned int anin1 = Wire.read()&0x03;  
  anin1=anin1<<8;
  anin1 = anin1|Wire.read(); 
  
  Serial.println(anin0);
  Serial.println(anin1);
  
  digitalWrite(D4, LOW);  //LED D4 OFF.
  delay(500);  
}













