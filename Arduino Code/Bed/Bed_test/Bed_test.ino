#include <Wire.h>

#define I2C_SDL   D1
#define I2C_SDA   D2

void LedAanUit(int i);
int leesDruksensor();
int leesKnop();

unsigned int outputs=0;
void setup() {
  Serial.begin(115200);//Set serial Baud
  Wire.begin();
}

void loop() {
  int drukknopwaarde = leesDruksensor();
  Serial.print("Drukknop ");
  Serial.println(drukknopwaarde);
  int knopwaarde = leesKnop();
  Serial.print("Knop ");
  Serial.println(knopwaarde);
  
  LedAanUit (1);
  delay (500);
  LedAanUit (0);
  delay(500);
}

void LedAanUit(int i) {
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x03));
  Wire.write(byte(0x0F));
  Wire.endTransmission();

  if (i == 1) {
    Wire.beginTransmission(0x38);
    Wire.write(byte(0x01));
    Wire.write(byte(0x10));
    Wire.endTransmission();
  }
  else if (i == 0) {
    Wire.beginTransmission(0x38);
    Wire.write(byte(0x01));
    Wire.write(byte(0x00));
    Wire.endTransmission();
  }
}

int leesDruksensor(){
  int knop;
  Wire.beginTransmission(0x36);
  Wire.write(byte(0xA2));          
  Wire.write(byte(0x03));  
  Wire.endTransmission(); 

  //Read analog 10bit inputs 0&1
  Wire.requestFrom(0x36, 4);   
  unsigned int anin0 = Wire.read()&0x03;  
  anin0=anin0<<8;
  if (anin0 > 100){
    knop = 1;
  }
  else {
    knop = 0;
  }
  return knop;
}

int leesKnop(){
  //Config PCA9554
  //Inside loop for debugging purpose (hot plugging wemos module into i/o board). 
  //IO0-IO3 as input, IO4-IO7 as output.
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x03));          
  Wire.write(byte(0x0F));         
  Wire.endTransmission();
  
  //Set PCA9554 outputs (IO44-IO7)
  Wire.beginTransmission(0x38); 
  Wire.write(byte(0x01));            
  Wire.write(byte(outputs<<4));            
  Wire.endTransmission(); 
  Serial.print("Digital out: ");
  Serial.println(outputs&0x0F);   
  outputs++;
  
  //Read PCA9554 outputs (IO40-IO3)
  Wire.beginTransmission(0x38); 
  Wire.write(byte(0x00));      
  Wire.endTransmission();
  Wire.requestFrom(0x38, 1);   
  unsigned int inputs = Wire.read();  
  Serial.print("Digital in: ");
  Serial.println(inputs&0x0F);  
  if (inputs == 15){
    return 1;
  }else if(inputs == 14){
    return 0;
  }
}
