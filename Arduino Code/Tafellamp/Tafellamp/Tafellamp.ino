#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>

#define I2C_SDL   D1
#define I2C_SDA   D2
#define RGB       D5
#define NUMPIXELS 1

Adafruit_NeoPixel pixels(NUMPIXELS, RGB, NEO_GRB + NEO_KHZ800);

int port = 8883;
const char *ssid = "piiWAP";
const char *password = "aanwezig2";
const char* host = "192.168.4.1";

String line = "";

int leesPIR(); 
void ledRGB(int i);

void setup() {
  Wire.begin();//Start wire
  Serial.begin(115200);//Set serial Baud
  pixels.begin();
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    delay(500);
  }
  Serial.print("connected to ");
  Serial.println(ssid);
}

void loop() {
  WiFiClient client;
  Serial.printf("\n[Connecting to %s ... ", host);
  if (client.connect(host, port))
  {
    line = client.readStringUntil('\r');
    if (line=="ID?"){
      client.println(String("y"));
    }
    line = client.readStringUntil('\r');
    if (line=="OK"){
      Serial.println("Verified");
      while (client.connected() || client.available()){
        line = client.readStringUntil('\r');
        Serial.println(line);
        if (line == "getStatus"){
          int PIR = leesPIR();
          client.print(String(PIR));          
          line = "";
        }
        else if (line == "disco"){
          ledRGB(2);
          line = "";
        }
        else if (line == "uit"){
          ledRGB(0);
          line = "";
        }
      }
    }
    else{
      Serial.print("Failed verification.");
    }
    Serial.println("Failed connection.");
    client.stop();
  }
}

int leesPIR(){
  Wire.beginTransmission(0x38); 
  Wire.write(byte(0x00));
  Wire.endTransmission();  
  Wire.requestFrom(0x38, 1);          
  int reading = Wire.read();
  return (0x01 & reading);
}

void ledRGB(int i){
  if (i==0){      //aan
    pixels.clear(); // Set all pixel colors to 'off'
    pixels.show();  // Send the updated pixel colors to the hardware
  }
  else if (i==1){ //uit
    pixels.setPixelColor(0, pixels.Color(255, 255, 255)); // Set the pixel to green
    pixels.show();   // Send the updated pixel colors to the hardware
  }
  else if(i==2){  //disco
   for(int a=0; a<NUMPIXELS; a++) {
      pixels.setPixelColor(i,0,0,255);
      pixels.show();   // Send the updated pixel colors to the hardware
    }    
    delay(500);
    for(int a=0; a<NUMPIXELS; a++) {
      pixels.setPixelColor(i,0,255,0);
      pixels.show();   // Send the updated pixel colors to the hardware
    }    
    delay(500);
    for(int a=0; a<NUMPIXELS; a++) {
      pixels.setPixelColor(i,255,0,0); 
      pixels.show();   // Send the updated pixel colors to the hardware
    }
    delay(500);
  }
}
