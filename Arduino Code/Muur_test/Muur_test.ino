#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#define I2C_SDL   D1
#define I2C_SDA   D2
#define RGB       D5
#define NUMPIXELS 3

void RGBstrip(int i); //0 is uit, 1 is aan, 2 is disco mode
void RGBdisco();
void AanUitLCD(int i);
int leesinput(int i);
int hex = 0x00;

Adafruit_NeoPixel pixels(NUMPIXELS, RGB, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels


void setup(void) {
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.show();
  Wire.begin();//Start wire
  Serial.begin(115200);//Set serial Baud
}

void loop(void) {

  RGBstrip(2);
  int pot = leesinput(2); //2:uitlezen van potmeter
  int ldr = leesinput(1); //1:uitlezen van LDR
  pixels.setBrightness(pot/4);
//  delay(DELAYVAL);
//  RGBstrip(0);
//  delay(DELAYVAL);
//  AanUitLCD(1);//aan
//  delay(DELAYVAL);
//  AanUitLCD(0);//uit
//  delay(DELAYVAL);
//  int ldr = leesinput(1); //1:uitlezen van LDR
//  int pot = leesinput(2); //2:uitlezen van potmeter
//
//  Serial.print("LDR waarde: ");
//  Serial.println(ldr);  
//  delay(DELAYVAL);
//  Serial.print("Potmeter: ");
//  Serial.println(pot);   
//  delay(DELAYVAL);
}

void RGBstrip(int i){
  if (i==0){
    pixels.clear();
    pixels.show();
  }
  else if (i==1){
    for(int a=0; a<NUMPIXELS; a++) { //Elke pixel aanzetten
      pixels.setPixelColor(i,255,0,0);
      pixels.show();   // Send the updated pixel colors to the hardware
    }
  }
  else if (i==3){
    for(int a=0; a<NUMPIXELS; a++) { //Elke pixel aanzetten
      pixels.setPixelColor(i,0,0,255);
      pixels.show();   // Send the updated pixel colors to the hardware
    }    
    delay(500);
    for(int a=0; a<NUMPIXELS; a++) { //Elke pixel aanzetten
      pixels.setPixelColor(i,0,255,0);
      pixels.show();   // Send the updated pixel colors to the hardware
    }    
    delay(500);
    for(int a=0; a<NUMPIXELS; a++) { //Elke pixel aanzetten
      pixels.setPixelColor(i,255,0,0); 
      pixels.show();   // Send the updated pixel colors to the hardware
    }
    delay(500);
  }
}

void AanUitLCD(int i){
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x03));          
  Wire.write(byte(0x0F));         
  Wire.endTransmission();
  
  if (i==1){
    Wire.beginTransmission(0x38);
    Wire.write(byte(0x01));          
    Wire.write(byte(hex |= 0x10));         
    Wire.endTransmission();
  }
  else {
    Wire.beginTransmission(0x38);
    Wire.write(byte(0x01));          
    Wire.write(byte(hex &= !(0x00)));         
    Wire.endTransmission();
  }
}

int leesinput(int i){
  //Config MAX11647
  //Inside loop for debugging purpose (hot plugging wemos module into i/o board). 
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
  
  if (i==1){
    return anin0;
  }else if (i==2){
    return anin1;
  }else{
    return 0;
  }
}
