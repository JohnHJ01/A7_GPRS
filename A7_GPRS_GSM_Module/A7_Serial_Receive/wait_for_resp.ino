boolean wait_for_resp(const char* resp, unsigned int timeout, unsigned int chartimeout)
{
  int len = strlen(resp);
  int sum = 0;
  unsigned long timerStart, prevChar;    //prevChar is the time when the previous Char has been read.
  timerStart = millis();
  prevChar = 0;
  while (1) {
    if (A7Serial.available()) {
      char c = A7Serial.read();
      //debug purpose
      Serial.print(c);
      ////
      prevChar = millis();
      sum = (c == resp[sum]) ? sum + 1 : 0;
      if (sum == len)break;
    }
    if ((unsigned long) (millis() - timerStart) > timeout * 1000UL) {
      return false;
    }
    //If interchar Timeout => return FALSE. So we can return sooner from this function.
    if (((unsigned long) (millis() - prevChar) > chartimeout) && (prevChar != 0)) {
      return false;
    }

  }
  return true;
}

