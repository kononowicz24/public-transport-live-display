#ifndef _DEBUGINTERFACE_H_
#define _DEBUGINTERFACE_H_
#include <Arduino.h>

String precUptimePrint(long a) {
  return String(a/1000)+"."+(a%1000);
}

String debug() {
  return String("[")+precUptimePrint(millis())+"] : ";
}

String debug(String key) {
  return String("[")+precUptimePrint(millis())+"]["+key+"] : ";
}

#endif
