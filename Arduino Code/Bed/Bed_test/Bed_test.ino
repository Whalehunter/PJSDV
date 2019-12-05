#include <Wire.h>

#define I2C_SDL   D1
#define I2C_SDA   D2

void LedAanUit(int i);
int leesDruksensor();


void setup() {
  Serial.begin(115200);//Set serial Baud
  Wire.begin();
}

void loop() {
  int knopwaarde;
  LedAanUit (1);
  delay (500);
  LedAanUit (0);
  delay(500);
    //Config MAX11647
  //Inside loop for debugging purpose (hot plugging wemos module into i/o board). 
  
  knopwaarde = leesDruksensor();
  Serial.print("Knop ");
  Serial.println(knopwaarde);
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
