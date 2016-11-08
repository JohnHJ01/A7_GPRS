#include <SoftwareSerial.h>
//#include<PubSubClient.h>
#include<MQTTClient.h>
//#include <MQTTTest.h>
#include<A7Client.h>

A7Client a7Client (7, 8); // RX, TX on Uno, connected to Tx, Rx of A7 Module
MQTTClient client;

char server[] = "test.mosquitto.org";
//uint8_t message[]={0x0F,0x0C,0x02,0x41,0x42,0x00,0x48,0x65,0x6C,0x6C,0x6F,0x57,0x6F,0x72,0x6C,0x64};
//char buffer[512];


void setup() {
  Serial.begin(9600);
  a7Client.changeBaud();//Baud rate for A7.serial is now at 9600 bps
  Serial.println(F("Response okay! :) Module is alive!"));
  a7Client.stop();
  Serial.println(freeRam());
  Serial.println(client.begin(server, a7Client));
  Serial.println(freeRam());
  int counter = 0;
  int numberOfTrial = 3;
  Serial.println(F("Start of nightmare"));
  Serial.println(client.connect("arduino"));
  Serial.println(freeRam());
  Serial.println(client.publish("AB", "world"));


//  while (!client.connected() && (numberOfTrial > 0)) {
//    Serial.print(client.connect("arduino"));
//    Serial.print(", counter = ");
//    Serial.println(counter++);
//    Serial.println(freeRam());
//    numberOfTrial--;
//  }
  Serial.println(F("Out"));
  a7Client.flush();
  Serial.flush();


}

void loop() {
  if (a7Client.available())
    Serial.write(a7Client.read());

  if (Serial.available())
    a7Client.writeSerial(Serial.read());

}
void messageReceived(String topic, String payload, char * bytes, unsigned int length) {
  Serial.print("incoming: ");
  Serial.print(topic);
  Serial.print(" - ");
  Serial.print(payload);
  Serial.println();
}

int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}



