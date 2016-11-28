#include <SoftwareSerial.h>
#include<A7Client.h>
A7Client a7Client (7, 8); // RX, TX on Uno, connected to Tx, Rx of A7 Module

const char server[] = "waterqualitysite.azurewebsites.net";
//char message[] = "GET /log.php?t=2016-10-25,21:16:00&te=30.0&do=7.05&pH=5.66&tu=7&la=5.1492&ln=100.492 HTTP/1.1\r\nHost: waterqualitysite.azurewebsites.net\r\nConnection: close\r\n\r\n";
char msg[200] = "";
// char time[] = "2016-11-18,22:35:01";
char temperature[6] = "";
char pH[6] = "";
char DO[6] = "";
char turbidity[5] = "";
char lat[9] = "";
char lng[9] = "";

const uint8_t pinTemperature  = A0;
const uint8_t pinPH           = A1;
const uint8_t pinDO           = A2;
const uint8_t pinTurbidity    = A3;

struct Data{
  float temperature;
  float pH;
  float DO;
  uint8_t turbidity;
} data;

unsigned long previousMillis = 0; //for looping purpose
long timeInterval = 5000; //update once per 5 seconds

void setup() {
  Serial.begin(9600);
  Serial.println(freeRam());
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  a7Client.changeBaud();//Baud rate for A7.serial is now at 9600 bps
  Serial.println(freeRam());
  Serial.println(F("Response okay! :) Module is alive!"));
  a7Client.startGPS();
  getData(&data);
  printData(&data);
    
  while(!a7Client.readGPS());//force Arduino to complete one GPS at least once =.= 16/11/16      
  if(a7Client.parse(a7Client.lastNMEA())){
    // Serial.print(F("Location: "));
    // Serial.print(a7Client.latitude, 4); Serial.print(a7Client.lat);
    // Serial.print(F(", ")); 
    // Serial.print(a7Client.longitude, 4); Serial.println(a7Client.lon);
    // Serial.print(F("Location (in degrees, works with Google Maps): "));
    // Serial.print(a7Client.latitudeDegrees, 4);
    // Serial.print(F(", ")); 
    // Serial.println(a7Client.longitudeDegrees, 4);    
  }
  convertToChar(&data, a7Client);    
  constructHTTPRequest(msg);  
  a7Client.connect(server, 80);
  sendHTTPRequest(msg);  
}


void loop() {  
  // Serial.println(F("I am down here"));
  
  // if (a7Client.available())
  // Serial.write(a7Client.read());

  // if (Serial.available())
  // a7Client.writeSerial(Serial.read());

//start of millis() code from AdaFruit
  unsigned long currentMillis = millis();
  Serial.println(freeRam());
  if((currentMillis - previousMillis >= timeInterval)){
    previousMillis = currentMillis;
    getData(&data);
    Serial.println(freeRam());
    printData(&data);
    Serial.println(freeRam());
    while(!a7Client.readGPS());//force Arduino to complete one GPS at least once =.= 16/11/16      
    if(a7Client.parse(a7Client.lastNMEA())){
      Serial.println(F("parsed completed"));    
    }
    convertToChar(&data, a7Client);  
    printCharData();
    constructHTTPRequest(msg);    
    a7Client.connect(server, 80);
    sendHTTPRequest(msg);

  }
}



