#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>

#define I2C_SDL   D1
#define I2C_SDA   D2
#define RGB       D5
#define NUMPIXELS 3

int port = 8883;
const char *ssid = "piiWAP";
const char *password = "aanwezig2";
const char* host = "192.168.4.1";

void RGBstrip(int i); //0 is uit, 1 is aan, 2 is disco mode
void RGBbrightness(int i);
void RGBdisco();
void AanUitLCD(int i);
int leesinput(int i); //1 is LDR waarde, 2 is POTmeter
int hex = 0x00;

Adafruit_NeoPixel pixels(NUMPIXELS, RGB, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels


void setup(void) {
  pinMode(D4, OUTPUT);  //Set D4 as Output.
  pinMode(D5, OUTPUT);  //Set D5 as Output.
  
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.show();
  Wire.begin();//Start wire
  Serial.begin(115200);//Set serial Baud
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

void loop(void) {
WiFiClient client;
  Serial.printf("\n[Connecting to %s ... ", host);
  if (client.connect(host, port))
  {
    line = client.readStringUntil('\r');
    if (line=="ID?"){
      client.println(String("y")); //eigenlijk Muur
    }
    line = client.readStringUntil('\r');
    if (line=="OK"){
      Serial.println("Verified");
      while (client.connected() || client.available()){
        line = client.readStringUntil('\r');
        Serial.println(line);
        if (line == "getStatus"){
          int LDR = leesinput(1);
          int POT = leesinput(2);
          //client.print(String(LDR));
          client.print(String(POT));          
          line = "";
        }
        else if (line == "LCDAan"){
          AanUitLCD(1);
          line = "";
        }
        else if (line == "LCDUit"){
          AanUitLCD(0);
          line = "";
        }
        if ((line >= "A" & <= "Z" )|(line >= "a" & <= "z"){
          int temp = toInt(line);
          if (temp >= 0 & temp <=1024){
            RGBbrightness(temp);
          }
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

void RGBbrightness(int i){
  pixels.setBrightness(i/4);
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
