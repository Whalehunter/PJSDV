#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#define I2C_SDL   D1
#define I2C_SDA   D2

int port = 8883;
const char *ssid = "piiWAP";
const char *password = "aanwezig2";
const char* host = "192.168.4.1";

int leesKnop(); //0: niet ingedrukt, 1: ingedrukt
void BuzzerAanUit(int i);
void deurBel();
void LedAanUit(int i);
int leesInput(int i);//1 is anin0, 2 is anin1


String line = "";
int state = 0;      // the current state of the output pin
int reading;           // the current reading from the input pin
int previous = 0;    // the previous reading from the input pin
int hex = 0x00;
int nood;
int brand;

void setup() {
  Wire.begin();//Start wire
  Serial.begin(115200);//Set serial Baud

    BuzzerAanUit(0);

  Serial.printf("Connecting");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    delay(500);
  }
//  Serial.print("connected to ", ssid);
}

void loop() {
  WiFiClient client;
 // Serial.printf("\n[Connecting to %s ... ", host);
  if (client.connect(host, port))
  {
    line = client.readStringUntil('\r');
    if (line=="ID?"){
      client.print(String("f")); //eigenlijk "Zuil"
    }
    line = client.readStringUntil('\r');
    if (line=="OK"){
      Serial.println("Verified");
      while (client.connected() || client.available()){
        line = client.readStringUntil('\r');
        
        if (line == "getStatus"){
          StaticJsonDocument<100> data;
          data["knopValue"] = leesKnop();
          data["sensorValue"] = leesInput(1);

          char buffer[100];

          serializeJson(data, buffer);
          client.print(String(buffer));

          line = ""; 
        }
        else if (line == "deurBelAan" || line == "noodAlarm"){
          BuzzerAanUit(1);
          line = "";
        }
        else if (line == "noodAlarmAan") {
          BuzzerAanUit(1);
          nood = 1;
        }
        else if ((line == "deurBelUit" || line == "brandAlarmUit" || line == "noodAlarmUit") && !nood && !brand){
          BuzzerAanUit(0);
          line = "";
        }
        else if (line == "ledAan"){
          LedAanUit(1);
          line = "";
        }
        else if (line == "ledUit"){
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

int leesKnop(){
  Wire.beginTransmission(0x38); 
  Wire.write(byte(0x00));
  Wire.endTransmission();  
  Wire.requestFrom(0x38, 1);          
  reading = Wire.read();
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
  if (reading&0x01 == 1){
    return 1;
  }
  return 0;
}

void BuzzerAanUit(int i) {
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x03));
  Wire.write(byte(0x0F));
  Wire.endTransmission();

  if (i == 1) {
    Wire.beginTransmission(0x38);
    Wire.write(byte(0x01));
    Wire.write(byte(hex |= 0x10));
    Wire.endTransmission();
  }
  else if (i == 0) {
    Wire.beginTransmission(0x38);
    Wire.write(byte(0x01));
    Wire.write(byte(hex &= !(0x10)));
    Wire.endTransmission();
  }
}
void deurBel(){
  BuzzerAanUit(1);
  delay(500);
  BuzzerAanUit(0);
  delay(250);
}
void LedAanUit(int i) {
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x00));
  Wire.write(byte(0x0F));
  Wire.endTransmission();

  if (i == 1) {
    Wire.beginTransmission(0x38);
    Wire.write(byte(0x01));
    Wire.write(byte(hex |= 0x20));
    Wire.endTransmission();
  }
  else if (i == 0) {
    Wire.beginTransmission(0x38);
    Wire.write(byte(0x01));
    Wire.write(byte(hex &= (0x20)));
    Wire.endTransmission();
  }
}
int leesInput(int i){
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
