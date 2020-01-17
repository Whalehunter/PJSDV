#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#define I2C_SDL    D1
#define I2C_SDA    D2

int port = 8883;
const char *ssid = "piiWAP";
const char *password = "aanwezig2";
const char* host = "192.168.4.1";

void trilSensorAanUit(int i); //0: uit, 1: aan
void lampAanUit(int i); //0: uit, 1: aan 
int leesPbUit();  //0: niet ingedrukt, 1: ingedrukt
int leesDsUit();  //0: niks, 1: ligt wat op de sensor

String line = "";
int pb = 0;
int ds = 0;
int hex = 0x00;
  
void setup(void) {
  trilSensorAanUit(0);
  lampAanUit(0);
  
  Wire.begin();  //Start wire.
  Serial.begin(115200);  //Set serial Baud.
  Serial.printf("Connecting to", ssid);
  WiFi.begin(ssid, password);

  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    delay(500);
  }
  Serial.printf("Connected to ", ssid);
}



void loop(void) {
  WiFiClient client;
  
  pb = leesPbUit();
  ds = leesDsUit();

  Serial.println(pb);
  Serial.println(ds);
  Serial.println();

  trilSensorAanUit(0);
  lampAanUit(0);
  
  Serial.printf("\n[Connecting to %s ... ", host);
  if (client.connect(host, port)){
    Serial.println("connected]");
    line = client.readStringUntil('\r');
    if (line == "ID?"){
      client.print(String("z"));
    }
    line = client.readStringUntil('\r');
    if (line == "OK"){
      //client.print(String("z"));
    }
    while (client.connected() || client.available()){  
      line = client.readStringUntil('\r');
      Serial.println(line);
      if (line == "getStatus"){
        StaticJsonDocument<100> data;
        data["drukknop"] = leesPbUit();
        data["drukSensor"] = leesDsUit();
     
        char buffer[100];

        serializeJson(data, buffer);
        Serial.println(buffer);
        
        client.print(String(buffer));          
        line = "";
      }
      else if (line == "ledAan"){
        lampAanUit(1);
      }
      else if (line == "ledUit"){
        lampAanUit(0);
      }
      else if (line == "trilAan"){
        trilSensorAanUit(1);
      }
      else if (line == "trilUit"){
        trilSensorAanUit(0);
      }
      else {
        Serial.println("Invalid input");
      }
    }
  Serial.println("Failed connection.");
  client.stop();
  }
}

void trilSensorAanUit(int i){
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x03));           
  Wire.write(byte(0x0F));            
  Wire.endTransmission();

  if (i == 1){
    Wire.beginTransmission(0x38);
    Wire.write(byte(0x01));           
    Wire.write(byte(hex |= 0x20));            
    Wire.endTransmission();
  }
  else {
    Wire.beginTransmission(0x38);
    Wire.write(byte(0x01));           
    Wire.write(byte(hex &= ~(0x20)));            
    Wire.endTransmission(); 
  }
}

void lampAanUit(int i){
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x03));           
  Wire.write(byte(0x0F));            
  Wire.endTransmission();

  if (i == 1){
    Wire.beginTransmission(0x38);
    Wire.write(byte(0x01));           
    Wire.write(byte(hex |= 0x10));            
    Wire.endTransmission();
  }
  else {
    Wire.beginTransmission(0x38);
    Wire.write(byte(0x01));           
    Wire.write(byte(hex &= ~(0x10)));            
    Wire.endTransmission(); 
  }
}

int leesPbUit(){
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x03));          
  Wire.write(byte(0x0F));         
  Wire.endTransmission();
  
  Wire.beginTransmission(0x38); 
  Wire.write(byte(0x00));      
  Wire.endTransmission();
  Wire.requestFrom(0x38, 1);  //request data from 0x38   
  unsigned int inputs = Wire.read();//Write value on address of inputs.
  if (inputs&0x01 == 1){
    return 1;
  }
  else {
    return 0;
  }
}

int leesDsUit(){
  Wire.beginTransmission(0x36);
  Wire.write(byte(0xA2));          
  Wire.write(byte(0x03));  
  Wire.endTransmission(); 

  Wire.requestFrom(0x36, 4);  //request data from 0x36   
  unsigned int anin0 = Wire.read()&0x03;  
  anin0=anin0<<8;
  anin0 = anin0|Wire.read();
  if (anin0 >= 100){
    return 1;
  }
  else {
    return 0;
  }
}
