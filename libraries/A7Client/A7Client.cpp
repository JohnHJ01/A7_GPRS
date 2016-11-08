//A7_Client.cpp

#include "A7Client.h"

#define A7Client_ENABLE_DEBUG
#define A7Client_ENABLE_RESP_DEBUG

SoftwareSerial* serialA7 = NULL;

A7Client::A7Client(uint8_t rx, uint8_t tx, uint32_t baudRate){
	serialA7 = new SoftwareSerial(rx, tx);
	serialA7->begin(baudRate);
}

bool A7Client::init(){
	
}

bool A7Client::checkResponse(const char* resp, unsigned int timeout, unsigned int chartimeout)
{
  int len = strlen(resp);
  int sum = 0;
  unsigned long timerStart, prevChar;    //prevChar is the time when the previous Char has been read.
  timerStart = millis();
  prevChar = 0;
  while (1) {
    if (serialA7->available()) {
      char c = serialA7->read();
      //debug purpose
	  #ifdef A7Client_ENABLE_RESP_DEBUG
      Serial.print(c);
	  #endif
      ////
      prevChar = millis();
      sum = (c == resp[sum]) ? sum + 1 : 0;
      if (sum == len)break;
    }
    if ((unsigned long) (millis() - timerStart) > timeout * 1000UL) {
		Serial.println(F("Timeout"));
      return false;
    }
    //If interchar Timeout => return FALSE. So we can return sooner from this function.
    if (((unsigned long) (millis() - prevChar) > chartimeout) && (prevChar != 0)) {
		Serial.println(F("InterChar Timeout"));
      return false;
    }
  }
  serialA7->flush();
  return true;
}

void A7Client::changeBaud(){
	while (1) {
    Serial.println(F("Trying to change baud rate at 115200"));//change baud rate at 115200
    serialA7->write("AT+IPR=9600\r\n"); //need to change baud rate at every startup, default is 115200kbps
    if (checkResponse("OK\r\n", DEFAULT_TIMEOUT, DEFAULT_INTERCHAR_TIMEOUT )) {
      //we got a response!
      serialA7->end();
      serialA7->begin(9600);
      break;
    }
    else {
      //try changing the baud to 9600...
      serialA7->end();
      serialA7->begin(9600);
      Serial.println(F("Trying to check baud rate at 9600"));
      serialA7->write("AT\r\n");
      if (checkResponse("OK\r\n", DEFAULT_TIMEOUT, DEFAULT_INTERCHAR_TIMEOUT )) {
        //we got a response!
        break;
      } else {
        serialA7->write("AT\r\n");
        if (checkResponse("OK\r\n", DEFAULT_TIMEOUT, DEFAULT_INTERCHAR_TIMEOUT )) {
          break;
        } else {
          serialA7->flush();
          serialA7->write("AT\r\n");
          if (checkResponse("OK\r\n", DEFAULT_TIMEOUT, DEFAULT_INTERCHAR_TIMEOUT )) {
            break;
          }
        }
      }
      //If there is no response, change it back to 115200 baud and repeat...
      serialA7->end();
      serialA7->begin(115200);
    }

  }
  Serial.println(F("Baud changed to 9600"));
}

int A7Client::connect(IPAddress ip, uint16_t port){
	char host[16] = {0};
	snprintf(host, 15, "%d.%d.%d.%d", ip[3], ip[2], ip[1], ip[0]);
	return connect(host, port);
	}

int A7Client::connect(const char *host, uint16_t port){
	serialA7->write("AT+CIPSTART=\"TCP\",\"");
	serialA7->write(host);
	serialA7->write("\",");
	serialA7->print(port);	//Too lazy to solve this uint16_t into uint8_t problem
	serialA7->write("\r\n");
	Serial.println(F("Opening TCP via AT"));
	if(checkResponse("CONNECT OK\r\n", DEFAULT_TIMEOUT, DEFAULT_INTERCHAR_TIMEOUT )){
		serialA7->flush();
		A7_DEBUG("Connected");
		return true;
	}else {
		A7_DEBUG("Connect failed");
		return false;
	}
}

void A7Client::A7_DEBUG(char *msg){
	#ifdef A7Client_ENABLE_DEBUG
	Serial.print(F("Debug: "));
	Serial.println(msg);	
	#endif
}

size_t A7Client::write(uint8_t b){	
	return write(&b, 1);
}

size_t A7Client::write(char *buf){
	if (buf == NULL) return 0;
    return write((const uint8_t *)buf, strlen(buf));
}

size_t A7Client::write(const uint8_t *buf, size_t size){
	serialA7->write("AT+CIPSEND\r\n");
	if(checkResponse(">", DEFAULT_TIMEOUT, DEFAULT_INTERCHAR_TIMEOUT )){
		A7_DEBUG(">shown, sending bytes");
		serialA7->write(buf, size);
		//serialA7->write(0x0D);
		//serialA7->write(0x0A);
		//serialA7->write(0x0D);
		//serialA7->write(0x0A);
		serialA7->write(0x1A); //required on A7 module to signal the end of command
		if(checkResponse("OK\r\n", DEFAULT_TIMEOUT, DEFAULT_INTERCHAR_TIMEOUT )){
			A7_DEBUG("Send succeeded");
			return size;
		}			
	}	
}

size_t A7Client::writeSerial(uint8_t b){	//send command directly thorugh serial
	return writeSerial(&b, 1);
}

size_t A7Client::writeSerial(char *buf){
	if (buf == NULL) return 0;
    return writeSerial((const uint8_t *)buf, strlen(buf));
}

size_t A7Client::writeSerial(const uint8_t *buf, size_t size){
	return serialA7->write(buf, size);	
}


int A7Client::available(){
	return serialA7->available();
}

int A7Client::read(){
	return serialA7->read();
}

int A7Client::read(uint8_t *buf, size_t size){
	return size;
}


void A7Client::flush(){
	serialA7->flush();
}

int A7Client::peek(){
	serialA7->peek();
}

void A7Client::stop(){
	if (connected()){
		serialA7->write("AT+CIPCLOSE\r\n");
		if(checkResponse("OK\r\n", DEFAULT_TIMEOUT, DEFAULT_INTERCHAR_TIMEOUT )){
			A7_DEBUG("Close succeeded");
		}else{
			A7_DEBUG("Close failed");
		}
	}else{
		A7_DEBUG("Nothing to be closed");
	}
	
}

uint8_t A7Client::connected(){
	serialA7->write("AT+CIPSTATUS\r\n");
	if(checkResponse("CONNECT OK  \r\n", DEFAULT_TIMEOUT, DEFAULT_INTERCHAR_TIMEOUT )){
		serialA7->flush();
		A7_DEBUG("Connected");
		return 1;		
	}else{
		A7_DEBUG("Not connected");
		return 0;
	}
	
}

A7Client::operator bool(){
	return (connected()==1);
	
}