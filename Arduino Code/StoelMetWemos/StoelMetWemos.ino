#include <Wire.h>
#include <ESP8266WiFi.h>

#define I2C_SDL    D1
#define I2C_SDA    D2

int port = 8883;
const char *ssid = "KAAS";
const char *password = "aanwezig2";
const char* host = "192.168.5.1";

void trilSensorAanUit(int i);
void lampAanUit(int i);
int leesPbUit();
int leesDsUit();

String line = "";
int pb = 0;
int ds = 0;
int hex = 0x00;
  
void setup(void) {
  pinMode(D4, OUTPUT);  //Set D4 as Output.
  pinMode(D5, OUTPUT);  //Set D5 as Output.
  
  Wire.begin();  //Start wire.
  Serial.begin(115200);  //Set serial Baud.
/*
  WiFi.mode(WIFI_STA);
  WiFi.hostname("wemos");
  WiFi.begin(ssid, password);

  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  Serial.print("Port: ");
  Serial.println(port);

  WiFiClient client = server.available();
  
  Serial.printf("\n[Connecting to %s ... ", host);
  if (client.connect(host, port))
  {
    Serial.println("connected]");
    line = client.readStringUntil('\r');
    if (line == "ID?"){
      client.print(String("ei"));
    }
    line = client.readStringUntil('\r');
    if (line == "OK"){
      client.print(String("ei"));
    }
  }*/
}



void loop(void) {
  WiFiClient client = server.available();
  
  digitalWrite(D5, HIGH);  //LED D4 ON.
  delay(500); 
  
  lampAanUit(1);
  trilSensorAanUit(0);
  pb = leesPbUit();
  ds = leesDsUit();

  Serial.println(pb);
  Serial.println(ds);
  Serial.println();
  
  if (client.connect(host, port)){
    line = client.readStringUntil('\r');
    while (line == "getStatus"){  
      while (client.connected() || client.available()){
        pb = leesPbUit();
        ds = leesDsUit();
        client.print(String(pb));
        client.print(String(ds));
      }
      client.stop();
    }
  }
  digitalWrite(D5, LOW);  //LED D4 OFF.
  delay(500);
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
    Wire.write(byte(hex &= !(0x20)));            
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
    Wire.write(byte(hex &= !(0x10)));            
    Wire.endTransmission(); 
  }
}

int leesPbUit(){
  Wire.beginTransmission(0x38); 
  Wire.write(byte(0x00));      
  Wire.endTransmission();
  Wire.requestFrom(0x38, 1);  //request data from 0x38   
  unsigned int inputs = Wire.read();//Write value on address of inputs.
  return inputs&0x01;
}

int leesDsUit(){
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
