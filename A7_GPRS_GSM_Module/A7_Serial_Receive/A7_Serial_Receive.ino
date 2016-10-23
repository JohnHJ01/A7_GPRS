#include <SoftwareSerial.h>
SoftwareSerial A7Serial(7, 8); // RX, TX on Uno

#define DEFAULT_TIMEOUT          5   //seconds
#define DEFAULT_INTERCHAR_TIMEOUT 1500   //miliseconds

char buffer[512];


void setup() {
  Serial.begin(9600);
  A7Serial.begin(115200);
  change_baud();//Baud rate for A7.serial is now at 9600 bps
  //Just for fun check
  while (!wait_for_resp("OK\r\n", DEFAULT_TIMEOUT, DEFAULT_INTERCHAR_TIMEOUT )) {
    Serial.println("Trying to check for response");
    A7Serial.write("at\r\n"); //checking for response
  }
  Serial.println("Response okay! :)");
  


}

void loop() {

  //  A7Serial.write("at+cipstatus\r\n");
  //  A7_read_buffer(buffer, sizeof(buffer)-1, DEFAULT_TIMEOUT, DEFAULT_INTERCHAR_TIMEOUT);
  //  Serial.println(buffer);

  if (A7Serial.available())
    Serial.write(A7Serial.read());

  if (Serial.available())
    A7Serial.write(Serial.read());

}


