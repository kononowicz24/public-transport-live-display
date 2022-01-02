#ifndef _DEBUGINTERFACE_H_
#define _DEBUGINTERFACE_H_
#include <Arduino.h>

String precUptimePrint(long a) {
  long sec = a/1000;
  long dec = a%1000;
  return String(sec)+"."+(dec<100?"0":"")+(dec<10?"0":"")+(dec);
}

String debug() {
  return String("[")+precUptimePrint(millis())+"] : ";
}

String debug(String key) {
  return String("[")+precUptimePrint(millis())+"]["+key+"] : ";
}

#endif
