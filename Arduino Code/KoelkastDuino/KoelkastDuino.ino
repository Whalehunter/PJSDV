#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#define I2C_SDL    D1
#define I2C_SDA    D2

int port = 8883;
const char *ssid = "piiWAP";
const char *password = "aanwezig2";
const char* host = "192.168.4.1";

int deurOpenDicht();
void turnOnOffFan(int i);
int readNTC1(int i);
void turnOnPeltier();

String line = "";
int NTC1 = 0;
int NTC2 = 0;
int ds = 0;
int hex = 0x00;
  
void setup(void) {
  pinMode(D5, OUTPUT);  //Set D5 as Output.
  
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
  
  turnOnPeltier(0);
  turnOnOffFan(1);

  Serial.printf("\n[Connecting to %s ... ", host);
  if (client.connect(host, port)){
    Serial.println("connected]");
    line = client.readStringUntil('\r');
    if (line == "ID?"){
      client.print(String("k"));
    }
    line = client.readStringUntil('\r');
    if (line == "OK"){
      //client.print(String("k"));
    }
    while (client.connected() || client.available()){  
      line = client.readStringUntil('\r');
      Serial.println(line);
      if (line == "getStatus"){
        StaticJsonDocument<100> data;
        data["deur"] = deurOpenDicht();
        data["NTC1"] = readNTC1(1);
        data["NTC2"] = readNTC1(0);
     
        char buffer[100];

        serializeJson(data, buffer);
        Serial.println(buffer);
        
        client.print(String(buffer));          
        line = "";
      }
      else if (line == "fanAan"){
        turnOnOffFan(1);
      }
      else if (line == "fanUit"){
        turnOnOffFan(0);
      }
      else if (line == "peltierAan"){
        turnOnPeltier(1);
      }
      else if (line == "peltierUit"){
        turnOnPeltier(0);
      }
      else {
        Serial.println("Invalid input");
      }
      float C;
      C = 1.00 / (1.00 / 298.15 + 1.00 / 3380.00*(log (1024.00 / (float) 480 - 1.00))) - 273.15;
      Serial.print(C);
    }
  Serial.println("Failed connection.");
  client.stop();
  }
}

int deurOpenDicht(){
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x03));          
  Wire.write(byte(0x0F));         
  Wire.endTransmission();
  
  Wire.beginTransmission(0x38); 
  Wire.write(byte(0x00));      
  Wire.endTransmission();
  Wire.requestFrom(0x38, 1);  //request data from 0x38   
  unsigned int inputs = Wire.read();//Write value on address of inputs.
  Serial.print("Digital in: "); 
  Serial.println(inputs&0x01);
  if (inputs&0x01 == 1){
    return 1;
  }
  else {
    return 0;
  } 
}

void turnOnOffFan(int i){
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

int readNTC1(int i){
  Wire.requestFrom(0x36, 4);   
  unsigned int anin0 = Wire.read()&0x03;  
  anin0=anin0<<8;
  anin0 = anin0|Wire.read();  
  unsigned int anin1 = Wire.read()&0x03;  
  anin1=anin1<<8;
  anin1 = anin1|Wire.read(); 
  Serial.print("analog in 0: ");
  Serial.println(anin0);   
  Serial.print("analog in 1: ");
  Serial.println(anin1);   
  Serial.println("");
  if (i){
    return anin0;
  }
  else {
    return anin1;
  }
}

void turnOnPeltier(int i){
  if (i == 1){
    digitalWrite(D5, HIGH);
    analogWrite(D5, 255);
  }
  else {
    digitalWrite(D5, LOW);
  }
  
}





