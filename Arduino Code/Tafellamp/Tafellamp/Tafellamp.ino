#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#define I2C_SDL   D1
#define I2C_SDA   D2
#define RGB       D5
#define NUMPIXELS 1

Adafruit_NeoPixel pixels(NUMPIXELS, RGB, NEO_GRB + NEO_KHZ800);

int leesPIR(); 
void RGBstrip();

void setup() {
  Wire.begin();//Start wire
  Serial.begin(115200);//Set serial Baud
}

void loop() {
  int sensor = leesPIR();
  Serial.print("PIR waarde: ");
  Serial.println(sensor);
  RGBstrip();
}

int leesPIR(){
  Wire.beginTransmission(0x38); 
  Wire.write(byte(0x00));
  Wire.endTransmission();  
  Wire.requestFrom(0x38, 1);          
  int reading = Wire.read();
  return reading;
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
