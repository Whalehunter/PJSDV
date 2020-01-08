#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#define I2C_SDL   D1
#define I2C_SDA   D2

int port = 8883;
const char *ssid = "piiWAP";
const char *password = "aanwezig2";
const char* host = "192.168.4.1";


void LedAanUit(int i); //0: uit, 1: aan
int leesDruksensor(); //0: niks, 1: ligt wat op de sensor
int leesKnop(); //0: niet ingedrukt, 1: ingedrukt

String line = "";
int state = 0;      // the current state of the output pin
int reading;           // the current reading from the input pin
int previous = 0;    // the previous reading from the input pin
int hex = 0x00;

void setup() {
  pinMode(D4, OUTPUT);  //Set D4 as Output.
  pinMode(D5, OUTPUT);  //Set D5 as Output.
  
  Wire.begin();

  Serial.begin(115200);
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
          StaticJsonDocument<100> data;
          data["knop"] = leesKnop();
          data["drukSensor"] = leesDruksensor();

         // Serial.println(data);

          char buffer[100];

          serializeJson(data, buffer);
          Serial.println(buffer);
          
          client.print(String(buffer));          
          line = "";
        }
        else if (line == "lampAan"){
          LedAanUit(1);
          line = "";
        }
        else if (line == "lampUit"){
          LedAanUit(0);
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

void LedAanUit(int i) {
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x03));
  Wire.write(byte(0x0F));
  Wire.endTransmission();

  if (i == 1) {
    Wire.beginTransmission(0x38);
    Wire.write(byte(0x01));
    Wire.write(byte(hex |= (0x10)));
    Wire.endTransmission();
  }
  else if (i == 0) {
    Wire.beginTransmission(0x38);
    Wire.write(byte(0x01));
    Wire.write(byte(hex &= !(0x10)));
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
  return anin0;
//  if (anin0 > 100){
//    knop = 1;
//  }
//  else {
//    knop = 0;
//  }
//  return knop;
}

int leesKnop(){
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x03));
  Wire.write(byte(0x0F));
  Wire.endTransmission();
  
  Wire.beginTransmission(0x38); 
  Wire.write(byte(0x00));
  Wire.endTransmission();  
  Wire.requestFrom(0x38, 1);          
  reading = Wire.read();
  return (0x01&reading);
//  if ((reading&0x01 == 1) && previous == 0) {
//    if (state == 1){
//      state = 0;  
//      Serial.println("state to 0");
//    }
//    else {
//      state = 1;
//      Serial.println("state to 1");
//    }
//  }
//  previous = reading&0x01;
//  return state;
}
