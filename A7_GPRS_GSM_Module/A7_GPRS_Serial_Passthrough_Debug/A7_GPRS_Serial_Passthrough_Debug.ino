#include <SoftwareSerial.h>
SoftwareSerial A7Serial(7, 8); // RX, TX on Uno
//connect to TX, RX of A7

char buffer[512];
const unsigned int MAX_INPUT = 50;


void setup() {
  Serial.begin(115200);
  A7Serial.begin(115200);
  Serial.println("Start");
}

void loop() {
  if (A7Serial.available()) {
//    Serial.write(A7Serial.read());
  processIncomingByte (A7Serial.read ());
  }

  if (Serial.available())
    A7Serial.write(Serial.read());
}

 void process_data (const char * data)
  {
  // for now just display it
  // (but you could compare it to some value, convert to an integer, etc.)
  Serial.println (data);
  }  // end of process_data
  
void processIncomingByte (const byte inByte)
  {
  static char input_line [MAX_INPUT];
  static unsigned int input_pos = 0;

  switch (inByte)
    {

    case '\n':   // end of text
      input_line [input_pos] = 0;  // terminating null byte
      
      // terminator reached! process input_line here ...
      process_data (input_line);
      
      // reset buffer for next time
      input_pos = 0;  
      break;

    case '\r':   // discard carriage return
      break;

    default:
      // keep adding if not full ... allow for terminating null byte
      if (input_pos < (MAX_INPUT - 1))
        input_line [input_pos++] = inByte;
      break;

    }  // end of switch
   
  } // end of processIncomingByte  

 
