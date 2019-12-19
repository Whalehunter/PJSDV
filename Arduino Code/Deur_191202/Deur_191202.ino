#include <Wire.h>
#include <Servo.h>

#define I2C_SDL    D1
#define I2C_SDA    D2

Servo myservo;
int servoPos = 0;

void binnenLampAanUit(int i);
void buitenLampAanUit(int i);

  
void setup(void) {
  pinMode(D4, OUTPUT);  //Set D5 as Output.
  pinMode(D5, OUTPUT);  //Set D5 as Output.
  
  Wire.begin();  //Start wire.
  Serial.begin(115200);  //Set serial Baud.

  myservo.attach(D5);

  int i = 0;  //Define i for for loops.
}

void loop(void) {
  digitalWrite(D4, HIGH);  //LED D4 ON.
  delay(500);

  Wire.beginTransmission(0x38); 
  Wire.write(byte(0x00));      
  Wire.endTransmission();
  Wire.requestFrom(0x38, 1);  //request data from 0x38   
  unsigned int inputs = Wire.read();//Write value on address of inputs.
  Serial.print("Digital in: "); 
  Serial.println(inputs&0x01);
  Serial.print("Digital in: "); 
  Serial.println(inputs&0x02);

  buitenLampAanUit(1);
  binnenLampAanUit(1);

  
  if ((inputs&0x01) == 1){
    for(servoPos; servoPos<=180; servoPos+=1){  //Go through 0 / 1024.
      myservo.write(servoPos);
      delay(15);  //Wait for 0.1s.
    }
  }
  
  digitalWrite(D4, LOW);  //LED D4 OFF.

  if ((inputs&0x02) == 2){
    for(servoPos; servoPos>=90; servoPos-=1){  //Go through 1024 / 0.
      myservo.write(servoPos);
      delay(15);  //Wait for 0.1s.
    }
  }
  digitalWrite(D4, LOW);  //LED D4 ON.
  delay(500);
}

void buitenLampAanUit(int i){
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x03));           
  Wire.write(byte(0x0F));            
  Wire.endTransmission();

  if (i == 1){
    Wire.beginTransmission(0x38);
    Wire.write(byte(0x01));           
    Wire.write(byte(0x10));            
    Wire.endTransmission();
  }
  else {
    Wire.beginTransmission(0x38);
    Wire.write(byte(0x01));           
    Wire.write(byte(0x00));            
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
    Wire.write(byte(0x20));            
    Wire.endTransmission();
  }
  else {
    Wire.beginTransmission(0x38);
    Wire.write(byte(0x01));           
    Wire.write(byte(0x00));            
    Wire.endTransmission(); 
  }
}
