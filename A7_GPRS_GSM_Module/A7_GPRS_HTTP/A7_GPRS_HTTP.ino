#include <SoftwareSerial.h>
#include<A7Client.h>

A7Client a7Client (7, 8); // RX, TX on Uno, connected to Tx, Rx of A7 Module

char server[] = "waterqualitysite.azurewebsites.net";
//char message[] = "GET /log.php?t=2016-10-25,21:16:00&te=30.0&do=7.05&pH=5.66&tu=7&la=5.1492&ln=100.492 HTTP/1.1\r\nHost: waterqualitysite.azurewebsites.net\r\nConnection: close\r\n\r\n";
char msg[256] = "GET /log.php?";

void setup() {
  Serial.begin(9600);
  Serial.println(freeRam());
  a7Client.changeBaud();//Baud rate for A7.serial is now at 9600 bps
  Serial.println(freeRam());
  Serial.println(F("Response okay! :) Module is alive!"));
  a7Client.connect(server, 80);
  Serial.println(freeRam());
  strcat(msg, "t=2016-10-25,21:16:00");
  strcat(msg, "&");
  strcat(msg, "te=30.0");
  strcat(msg, "&");
  strcat(msg, "do=7.05");
  strcat(msg, "&");
  strcat(msg, "pH=5.66");
  strcat(msg, "&");
  strcat(msg, "tu=7");
  strcat(msg, "&");
  strcat(msg, "la=5.1492");
  strcat(msg, "&");
  strcat(msg, "ln=100.492");
  strcat(msg, " HTTP/1.1\r\nHost: ");
  strcat(msg, server);
  strcat(msg, "\r\nConnection: close\r\n\r\n");

  Serial.println(msg);
  a7Client.write(msg);
  Serial.println(freeRam());
}


void loop() {
  if (a7Client.available())
    Serial.write(a7Client.read());

  if (Serial.available())
    a7Client.writeSerial(Serial.read());
}

int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

