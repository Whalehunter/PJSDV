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
  digitalWrite(D5, HIGH);  //LED D4 ON.
  delay(500); 
  
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

  if (inputs&0x01 == 1){  //If button is pushed.
    Wire.beginTransmission(0x38);
    Wire.write(byte(0x01));           
    Wire.write(byte(0x20)); //0x20 om het trill element aan te zetten. 0x10 om het LED aan te zetten.         
    Wire.endTransmission();
  }
  else{
    Wire.beginTransmission(0x38);
    Wire.write(byte(0x01));           
    Wire.write(byte(0x00)); //0x20 om het trill element aan te zetten. 0x10 om het LED aan te zetten.         
    Wire.endTransmission();
  }

  Wire.requestFrom(0x36, 4);  //request data from 0x36   
  unsigned int anin0 = Wire.read()&0x03;  
  anin0=anin0<<8;
  anin0 = anin0|Wire.read();
  Serial.println(anin0);

  if (anin0 >= 100){  //If pressure is higher then 100.
    Wire.beginTransmission(0x38);
    Wire.write(byte(0x01));           
    Wire.write(byte(0x10)); //0x20 om het trill element aan te zetten. 0x10 om het LED aan te zetten.         
    Wire.endTransmission();
  }
  else if (inputs&0x01 != 1){
    Wire.beginTransmission(0x38);
    Wire.write(byte(0x01));           
    Wire.write(byte(0x00)); //0x20 om het trill element aan te zetten. 0x10 om het LED aan te zetten.         
    Wire.endTransmission();
  }
  
  digitalWrite(D5, LOW);  //LED D4 OFF.
  delay(500);  
}













