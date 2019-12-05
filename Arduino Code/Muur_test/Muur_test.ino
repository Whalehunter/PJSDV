#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#define I2C_SDL   D1
#define I2C_SDA   D2
#define RGB       D5
#define NUMPIXELS 3

void RGBstrip();
void AanUitLCD(int i);
int leesinput(int i);
Adafruit_NeoPixel pixels(NUMPIXELS, RGB, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels


void setup(void) {
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  Wire.begin();//Start wire
  Serial.begin(115200);//Set serial Baud
}

void loop(void) {

  RGBstrip();
  AanUitLCD(1);//aan
  delay(DELAYVAL);
  AanUitLCD(0);//uit
  delay(DELAYVAL);
  int ldr = leesinput(1); //1:uitlezen van LDR
  int pot = leesinput(2); //2:uitlezen van potmeter

  Serial.print("LDR waarde: ");
  Serial.println(ldr);  
  delay(DELAYVAL); 
  Serial.print("Potmeter: ");
  Serial.println(pot);   
  delay(DELAYVAL);
}

void RGBstrip(){
  pixels.clear(); // Set all pixel colors to 'off'
  pixels.show();  // Send the updated pixel colors to the hardware
  delay(500);
  for(int i=0; i<NUMPIXELS; i++) { //Elke pixel aanzetten
    pixels.setPixelColor(i, pixels.Color(0, 150, 0)); // Set the pixel to green
    pixels.show();   // Send the updated pixel colors to the hardware
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
  Wire.write(byte(0x10));         
  Wire.endTransmission();
  }
  else if(i==0){
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x01));          
  Wire.write(byte(0x00));         
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
