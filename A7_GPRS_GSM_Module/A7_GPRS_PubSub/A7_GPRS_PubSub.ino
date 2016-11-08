#include <SoftwareSerial.h>
#include<PubSubClient.h>
#include<A7Client.h>

A7Client a7Client (7, 8); // RX, TX on Uno, connected to Tx, Rx of A7 Module
PubSubClient client(a7Client);

char server[] = "iot.eclipse.org";
uint8_t message[]={0x0F,0x0C,0x02,0x41,0x42,0x00,0x48,0x65,0x6C,0x6C,0x6F,0x57,0x6F,0x72,0x6C,0x64};
//char buffer[512];

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  Serial.println("Entered this reconnect() loop!");
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClientabcd5569")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("AB", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  a7Client.changeBaud();//Baud rate for A7.serial is now at 9600 bps
  Serial.println("Response okay! :) Module is alive!");
  //  a7Client.connect("mujkotel.wz.cz", 80);
  //  if (a7Client.connected()) {
  //    a7Client.write("GET /index.html HTTP/1.1\r\nHost: mujkotel.wz.cz\r\n\r\n");
  //  }
  client.setServer(server, 1883);
  client.setCallback(callback);
  Serial.println(client.connect("abhjtid"));
  Serial.println(F("Out"));
  client.publish("AB", "hello world");
//  a7Client.write(message, 16);
  
//  while (!client.connected()) {
//    Serial.println("Reconnect!");
//    Serial.println("Attempting MQTT connection...");
//    Serial.println(client.connect("arduinoClient"));   
//    client.publish("outTopic", "hello world"); 
//    client.subscribe("inTopic");
    // Attempt to connect
//    if (client.connect("arduinoClient")) {
//      Serial.println("connected");
//      // Once connected, publish an announcement...
//      client.publish("outTopic", "hello world");
//      // ... and resubscribe
//      client.subscribe("inTopic");
//    }else {
//      Serial.print("failed, rc=");
//      Serial.print(client.state());
//      Serial.println(" try again in 5 seconds");
//      // Wait 5 seconds before retrying
//      delay(1500);
//      }
//  }
}

void loop() {
  if (a7Client.available())
    Serial.write(a7Client.read());

  if (Serial.available())
    a7Client.writeSerial(Serial.read());

}


