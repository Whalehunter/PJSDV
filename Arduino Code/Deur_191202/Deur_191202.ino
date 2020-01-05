#include <Wire.h>
#include <ESP8266WiFi.h>
#include <Servo.h>

#define I2C_SDL    D1
#define I2C_SDA    D2

int port = 8883;
const char *ssid = "piiWAP";
const char *password = "aanwezig2";
const char* host = "192.168.4.1";

Servo myservo;
int servoPos = 0;

void binnenLampAanUit(int i);
void buitenLampAanUit(int i);
int leesBuitenUit();
int leesBinnenUit();
void deurOpen();
void deurDicht();

String line = "";
int pb = 0;
int ds = 0;
int hex = 0x00;

void setup(void) {
  pinMode(D5, OUTPUT);  //Set D5 as Output.
  myservo.attach(D5);
  
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

  buitenLampAanUit(1);
  binnenLampAanUit(1);
  pb = leesBuitenUit();
  ds = leesBinnenUit();
  if (pb == 1){
    deurOpen();
  }
  if (ds == 1){
    deurDicht();
  }

  Serial.printf("\n[Connecting to %s ... ", host);
  if (client.connect(host, port)){
    Serial.println("connected]");
    line = client.readStringUntil('\r');
    if (line == "ID?"){
      client.print(String("a"));
    }
    line = client.readStringUntil('\r');
    if (line == "OK"){
      client.print(String("a"));
    }
    while (client.connected() || client.available()){  
      line = client.readStringUntil('\r');
      if (line == "getStatus"){
        pb = leesBuitenUit();
        ds = leesBinnenUit();
        client.print(String(pb));
        client.print(String(ds));
        line = "";
      }
    }
  Serial.println("Failed connection.");
  client.stop();
  }
}

void buitenLampAanUit(int i){
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
    Wire.write(byte(hex &= !(0x10)));            
    Wire.endTransmission(); 
  }
}

void binnenLampAanUit(int i){
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
    Wire.write(byte(hex &= !(0x20)));            
    Wire.endTransmission(); 
  }
}

int leesBuitenUit(){
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

int leesBinnenUit(){
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
  Serial.println(inputs&0x02);
  
  if (inputs&0x02 == 2){
    return 1;
  }
  else {
    return 0;
  }
}

void deurOpen(){
  for(servoPos; servoPos<=180; servoPos+=1){  //Go through 0 / 1024.
    myservo.write(servoPos);
    delay(15);  //Wait for 0.1s.
  }
}

void deurDicht(){
  for(servoPos; servoPos>=90; servoPos-=1){  //Go through 1024 / 0.
    myservo.write(servoPos);
    delay(15);  //Wait for 0.1s.
  }
}











