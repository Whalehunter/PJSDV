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
  WiFiClient client;
  
  digitalWrite(D5, HIGH);  //LED D4 ON.
  delay(500); 

  Wire.beginTransmission(0x38); 
  Wire.write(byte(0x00));      
  Wire.endTransmission();
  Wire.requestFrom(0x38, 1);  //request data from 0x38   
  unsigned int inputs = Wire.read();//Write value on address of inputs.
  Serial.print("Digital in: "); 
  Serial.println(inputs&0x01);
  
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x03));           
  Wire.write(byte(0x30));            
  Wire.endTransmission();
/*
  if (client) {
    if (client.connected())
    {
      Serial.println("Client Connected");
    }

    while (client.connected()) {
      if (client.available() > 0) {
        String line = client.readStringUntil('\n');
        Serial.println(line);

        if (line == "g") { // lees draaiknop waarde 
          Wire.beginTransmission(0x38);
          Wire.write(byte(0x01));           
          Wire.write(byte(0x20)); //0x20 om het trill element aan te zetten. 0x10 om het LED aan te zetten.         
          Wire.endTransmission();
        }

        else if (line == "h") { // led aan
          Wire.beginTransmission(0x38);
          Wire.write(byte(0x01));           
          Wire.write(byte(0x00)); //0x20 om het trill element aan te zetten. 0x10 om het LED aan te zetten.         
          Wire.endTransmission();
        }

        else {
          client.print("Wrong input.");
        }
      }
    }
    client.stop();
    Serial.println("Client disconnected");
  }*/

  Wire.requestFrom(0x36, 4);  //request data from 0x36   
  unsigned int anin0 = Wire.read()&0x03;  
  anin0=anin0<<8;
  anin0 = anin0|Wire.read();
  Serial.println(anin0);

  if (client.connect(host, 80)){
    while (client.connected() || client.available())
    {
      if (client.available())
      {
        if (anin0 >= 100){  //If pressure is higher then 100.
          client.print(String("Hooger dan 100"));
          Wire.beginTransmission(0x38);
          Wire.write(byte(0x01));           
          Wire.write(byte(0x10)); //0x20 om het trill element aan te zetten. 0x10 om het LED aan te zetten.         
          Wire.endTransmission();
        }
        else if (inputs&0x01 != 1){
          client.print(String("Lager dan 100"));
          Wire.beginTransmission(0x38);
          Wire.write(byte(0x01));           
          Wire.write(byte(0x00)); //0x20 om het trill element aan te zetten. 0x10 om het LED aan te zetten.         
          Wire.endTransmission();
        }
      }
    }
  }
  
  digitalWrite(D5, LOW);  //LED D4 OFF.
  delay(500);
}













