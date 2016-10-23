#include <SoftwareSerial.h>
//#include<PubSubClient.h>
#include<A7Client.h>

//SoftwareSerial A7Serial(7, 8); // RX, TX on Uno, connected to Tx, Rx of A7 Module
A7Client a7Client (7, 8); // RX, TX on Uno, connected to Tx, Rx of A7 Module

char buffer[512];

void setup() {
  Serial.begin(9600);

  a7Client.changeBaud();//Baud rate for A7.serial is now at 9600 bps
  //Just for fun check
  //  while (!a7Client.checkResponse("OK\r\n", DEFAULT_TIMEOUT, DEFAULT_INTERCHAR_TIMEOUT )) {
  //    Serial.println("Trying to check for response");
  //    a7Client.write("AT\r\n"); //checking for response
  //  }
  Serial.println("Response okay! :) Module is alive!");
  a7Client.connect("mujkotel.wz.cz", 80);
  Serial.println("Connection opened, checking via connected()");
    if (a7Client.connected()) {      
      Serial.println("connected!");
    } else {
      Serial.println("not connected!");
    }
  
  Serial.println("Trying to close the connection");
  a7Client.stop();
  Serial.println("Connection closed, checking via connected()");
    if (a7Client.connected()) {      
      Serial.println("Still conected!");
    } else {
      Serial.println("not conected!");
    }
//  if(a7Client.connected()){
//    a7Client.write("GET /index.html HTTP/1.1\r\nHost: mujkotel.wz.cz\r\n\r\n");
//  }
}

void loop() {
  if (a7Client.available())
    Serial.write(a7Client.read());

  if (Serial.available())
    a7Client.writeSerial(Serial.read());

}


