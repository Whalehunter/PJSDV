#include <ESP8266WiFi.h>
#include <Wire.h>

#define I2C_SDL    D1
#define I2C_SDA    D2

int port = 8888;
WiFiServer server(port);

const char *ssid = "piiWAP";
const char *password = "aanwezig";

void setup()
{
  pinMode(D5, OUTPUT);
  Wire.begin();

  Wire.beginTransmission(0x38);
  Wire.write(byte(0x03));
  Wire.write(byte(0x0F));
  Wire.endTransmission();

  Wire.beginTransmission(0x38);
  Wire.write(byte(0x01));
  Wire.write(byte(0x00));
  Wire.endTransmission();

  Serial.begin(115200);
  Serial.println();

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

}

void loop()
{
  WiFiClient client = server.available();

  if (client) {
    if (client.connected())
    {
      Serial.println("Client Connected");
    }

    while (client.connected()) {
      if (client.available() > 0) {
        String line = client.readStringUntil('\n');
        Serial.println(line);

        if (line == "g") { /* lees draaiknop waarde */
          Wire.requestFrom(0x36, 4);
          unsigned int anin0 = ((Wire.read() & 0x03) << 8) | Wire.read();
          client.print(anin0);
        }

        else if (line == "h") { /* led aan */
          Wire.beginTransmission(0x38);
          Wire.write(byte(0x01));
          Wire.write(byte(1 << 4));
          Wire.endTransmission();
          client.print("Led aan");
        }

        else if (line == "j") { /* led uit */
          Wire.beginTransmission(0x38);
          Wire.write(byte(0x01));
          Wire.write(byte(0 << 4));
          Wire.endTransmission();
          client.print("Led uit");
        }
        else {
          client.print("Wrong input.");
        }
      }
    }
    client.stop();
    Serial.println("Client disconnected");
  }
}
