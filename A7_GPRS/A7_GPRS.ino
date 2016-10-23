#include <GPRS_Shield_Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>
SoftwareSerial A7Serial(7,8); // RX, TX on Uno
//connect to TX, RX of A7

void setup() {
  Serial.begin(9600);
  A7Serial.begin(115200);
  Serial.println("Start");
}

void loop() {
 if (A7Serial.available())

    Serial.write(A7Serial.read());

  if (Serial.available())

    A7Serial.write(Serial.read());

}
