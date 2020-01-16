#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ctype.h>

#define I2C_SDL   D1
#define I2C_SDA   D2
#define RGB       D5
#define NUMPIXELS 3

#define PORT      8883
#define SSID      "piiWAP"
#define PASS      "aanwezig2"
#define HOST      "192.168.4.1"

#define BUFSIZE  100

/* Leest LDR en POT-meter waardes op */
void getAnalog(unsigned int &ldr, unsigned int &pot) {
  Wire.beginTransmission(0x36);
  Wire.write(byte(0xA2));
  Wire.write(byte(0x03));
  Wire.endTransmission();

  // Read analog 10-bit inputs 0&1
  Wire.requestFrom(0x36, 4);
  ldr = ((Wire.read()&0x03)<<8)|Wire.read();
  pot = ((Wire.read()&0x03)<<8)|Wire.read();
}

/* Kleurwaarden ophalen van een pixel */
Adafruit_NeoPixel pixels(NUMPIXELS, RGB, NEO_GRB + NEO_KHZ800);
void px(int n, int &r, int &g, int &b ) {
  uint32_t pixel = pixels.getPixelColor(n);
  r = ((pixel>>16) & 0xFF);
  g = ((pixel>>8) & 0xFF);
  b = (pixel & 0xFF);
}

/* Simpelere functie voor het ophalen van berichten over WiFi */
String getLine(WiFiClient c) {
  return c.readStringUntil('\r');
}

/* Set or reset LCD */
void setLCD(int i=0){
  static int hex = 0x00;
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x03));
  Wire.write(byte(0x0F));
  Wire.endTransmission();

  if (i) {
    Wire.beginTransmission(0x38);
    Wire.write(byte(0x01));
    Wire.write(byte(hex |= 0x10));
    Wire.endTransmission();
  } else {
    Wire.beginTransmission(0x38);
    Wire.write(byte(0x01));
    Wire.write(byte(hex &= !(0x00)));
    Wire.endTransmission();
  }
}


void setup(void) {
  pinMode(D4, OUTPUT);   //Set D4 as Output.
  pinMode(D5, OUTPUT);   //Set D5 as Output.

  pixels.begin();        // Start LEDs
  Wire.begin();          // Start I2C
  Serial.begin(115200);  // Start Serial (+ baudrate)

  Serial.print("Connecting to ");
  Serial.println(SSID);

  WiFi.begin(SSID, PASS);

  /* Wacht tot WIFI verbonden is */
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("connected to ");
  Serial.println(SSID);
}

void loop(void) {
  String line = "";
  WiFiClient client;

  if (client.connect(HOST, PORT)) {
    line = getLine(client);
    if (line == "ID?"){
      client.print(String("m")); /* Identificeert zichzelf als 'm' (Muur) */
    }
    line = getLine(client);
    if (line == "OK") {
      Serial.println("Verified");

      while (client.connected() || client.available()) {
        line = getLine(client);
        Serial.println(line);

        if (line == "getStatus") {
          StaticJsonDocument<300> data;
          for (int i=0;i<NUMPIXELS;i++){
            int r, g, b;
            StaticJsonDocument<300> doc;
            JsonObject obj = doc.to<JsonObject>();
            px(i,r,g,b);
            obj["r"] = r;
            obj["g"] = g;
            obj["b"] = b;
            data[String(i)] = obj;
          }

          unsigned int ldr, pot;
          getAnalog(ldr, pot);
          data["ldr"] = ldr;
          data["pot"] = pot;

          char buffer[255];

          serializeJson(data, buffer);
          Serial.println(buffer);

          client.print(String(buffer));
        } else if (line != ""){
          StaticJsonDocument<BUFSIZE> data;
          deserializeJson(data, line);
          Serial.println(line);

          for (int i=0;i<NUMPIXELS;i++) {
            String nLed = "LED"+String(i);
            pixels.setPixelColor(i, pixels.Color(data[nLed]["R"], data[nLed]["G"], data[nLed]["B"]));
          }
          pixels.show();

          setLCD(data["S"]);
        }
        line = "";
      }
    }
    else{
      Serial.print("Failed verification.");
    }
    Serial.println("Failed connection.");
    client.stop();
  }
}
