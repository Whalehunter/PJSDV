#include <Wire.h>
#include <ESP8266WiFi.h>

#define I2C_SDL    D1
#define I2C_SDA    D2

int port = 8883;
WiFiServer server(port);
const char* host = "192.168.4.1";

const char *ssid = "piiWAP";
const char *password = "aanwezig";
  
void setup(void) {
  pinMode(D4, OUTPUT);  //Set D5 as Output.
  pinMode(D5, OUTPUT);  //Set D5 as Output.
  
  Wire.begin();  //Start wire.
  Serial.begin(115200);  //Set serial Baud.

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

    Serial.println("[Sending a request]");
    client.print(String("HALLO"));
  }
}

void loop(void) {
  WiFiClient client = server.available();
  
  digitalWrite(D5, HIGH);  //LED D4 ON.
  delay(500); 
  
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x03));           
  Wire.write(byte(0x0F));            
  Wire.endTransmission();

  Wire.beginTransmission(0x38);
  Wire.write(byte(0x01));           
  Wire.write(byte(0x00));            
  Wire.endTransmission();
  
  if (client.connect(host, port)){
    while (client.connected() || client.available()){
      
      Wire.beginTransmission(0x38); 
      Wire.write(byte(0x00));      
      Wire.endTransmission();
      Wire.requestFrom(0x38, 1);  //request data from 0x38   
      unsigned int inputs = Wire.read();//Write value on address of inputs.
      Serial.print("Digital in: "); 
      Serial.println(inputs&0x01);
      
      Wire.requestFrom(0x36, 4);  //request data from 0x36   
      unsigned int anin0 = Wire.read()&0x03;  
      anin0=anin0<<8;
      anin0 = anin0|Wire.read();
      Serial.println(anin0);

      if (anin0 >= 100){
        client.print(String("STOEL: Druksensor") + anin0);
      }
      if ((inputs&0x01) == 1){
        //client.print(String("STOEL: Knoppie") + (inputs&0x01));
        client.print(String("a"));
        String line = client.readStringUntil('\r');
          if (line == "b"){
            Serial.println("kaas");
            line = "a";
          }
        Serial.println(line);
      }
      delay(1000);
    }
    client.stop();
  }
  
  digitalWrite(D5, LOW);  //LED D4 OFF.
  delay(500);
}













