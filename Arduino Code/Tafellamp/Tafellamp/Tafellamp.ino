#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>

#define I2C_SDL   D1
#define I2C_SDA   D2
#define RGB       D5
#define NUMPIXELS 1
#define PORT      8883
#define SSID      "piiWAP"
#define PASS      "aanwezig2"
#define HOST      "192.168.4.1"
#define BUFSIZE   100

Adafruit_NeoPixel pixels(NUMPIXELS, RGB, NEO_GRB + NEO_KHZ800);

int leesPIR();
void ledRGB();

string getLine(c) {
  return c.readStringUntil('\r');
}

void setup() {
  Wire.begin();//Start wire
  Serial.begin(115200);//Set serial Baud
  pixels.begin();

  WiFi.begin(SSID, PASS);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serail.printf("Connected to ", SSID);
  pixels.setPixelColor(0, pixels.Color(255, 255, 255)); // ff een testje
}

void loop() {
  WiFiClient client;
  String ID = "s";

  if (client.connect(HOST, PORT)) {
    line = getLine(client);
    if (line == "ID?") {
      client.print(ID);
    }
    line = getLine(client);
    if (line == "OK") {
      while (client.connected() || client.available()) {
        line = getLine(client);
        if (line == "getStatus") {
          char buffer[BUFSIZE];
          StaticJsonDocument<BUFSIZE> data;
          uint32_t pixel = pixels.getPixelColor(0);
          data["rood"] = ((pixel>>16) & 0xFF);
          data["groen"] = ((pixel>>8) & 0xFF);
          data["blauw"] = (pixel & 0xFF);
          data["brightness"] = pixels.getBrightness(0);
          data["beweging"] = (sensor&0x01);
          serializeJson(data, buffer);
          client.print(String(buffer));
        } else if (line == "discoAan") {

        } else if (line == "discoUit") {

        } else if (line == "")

        line = "";
      }
    }
  } else {
    Serial.println("Connection failed: verification");
  }

  int sensor = leesPIR();
  Serial.print("Bewegingsensor: ");
  Serial.println(sensor&0x01);
  ledRGB();
  client.stop();
}

int leesPIR(){
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x00));
  Wire.endTransmission();
  Wire.requestFrom(0x38, 1);
  int reading = Wire.read();
  return reading;
}

void ledRGB(){
  pixels.clear(); // Set all pixel colors to 'off'
  pixels.show();  // Send the updated pixel colors to the hardware
  delay(250);
  pixels.setPixelColor(0, pixels.Color(255, 255, 255)); // Set the pixel to green
  pixels.show();   // Send the updated pixel colors to the hardware
  delay(250);
}
