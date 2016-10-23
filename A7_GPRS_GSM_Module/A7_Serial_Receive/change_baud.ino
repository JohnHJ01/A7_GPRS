void change_baud(){
   while (1) {
    Serial.println("Trying to change baud rate at 115200");//change baud rate at 115200
    A7Serial.write("at+ipr=9600\r\n"); //need to change baud rate at every startup, default is 115200kbps
    if (wait_for_resp("OK\r\n", DEFAULT_TIMEOUT, DEFAULT_INTERCHAR_TIMEOUT )) {
      //we got a response!
      A7Serial.end();
      A7Serial.begin(9600);
      break;
    }
    else {
      //try changing the baud to 9600...
      A7Serial.end();
      A7Serial.begin(9600);
      Serial.println("Trying to check baud rate at 9600");
      A7Serial.write("at\r\n");
      if (wait_for_resp("OK\r\n", DEFAULT_TIMEOUT, DEFAULT_INTERCHAR_TIMEOUT )) {
        //we got a response!
        break;
      } else {
        A7Serial.write("at\r\n");
        if (wait_for_resp("OK\r\n", DEFAULT_TIMEOUT, DEFAULT_INTERCHAR_TIMEOUT )) {
          break;
        } else {
          A7Serial.flush();
          A7Serial.write("at\r\n");
          if (wait_for_resp("OK\r\n", DEFAULT_TIMEOUT, DEFAULT_INTERCHAR_TIMEOUT )) {
            break;
          }
        }
      }
      //If there is no response, change it back to 115200 baud and repeat...
      A7Serial.end();
      A7Serial.begin(115200);
    }

  }
  Serial.println("Baud changed to 9600");
}

