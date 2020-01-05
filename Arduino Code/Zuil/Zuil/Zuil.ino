#include <Wire.h>
#include <ESP8266WiFi.h>

#define I2C_SDL   D1
#define I2C_SDA   D2

int port = 8883;
const char *ssid = "piiWAP";
const char *password = "aanwezig2";
const char* host = "192.168.4.1";

int leesKnop(); //0: niet ingedrukt, 1: ingedrukt
void BuzzerAanUit(int i);
void brandalarm();
void LedAanUit(int i);
int leesinput(int i);//1 is anin0, 2 is anin1


String line = "";
int state = 0;      // the current state of the output pin
int reading;           // the current reading from the input pin
int previous = 0;    // the previous reading from the input pin
int hex = 0x00;

void setup() {
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

void loop() {
  WiFiClient client;
  Serial.printf("\n[Connecting to %s ... ", host);
  if (client.connect(host, port))
  {
    line = client.readStringUntil('\r');
    if (line=="ID?"){
      client.println(String("y")); //eigenlijk "Zuil"
    }
    line = client.readStringUntil('\r');
    if (line=="OK"){
      Serial.println("Verified");
      while (client.connected() || client.available()){
        line = client.readStringUntil('\r');
        Serial.println(line);
        if (line == "getStatus"){
          int drukknop = leesKnop();
          int input = leesinput(1);
          client.print(String(drukknop));
          //client.print(String(input));
          line = "0"; 
        }
        else if (line == "buzzerAan"){
          BuzzerAanUit(1);
          line = "0";
        }
        else if (line == "buzzerUit"){
          BuzzerAanUit(0);
          line = "0";
        }
        else if (line == "ledAan"){
          LedAanUit(1);
          line = "0";
        }
        else if (line == "ledUit"){
          LedAanUit(0);
          line = "0";
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
void brandalarm(){
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
