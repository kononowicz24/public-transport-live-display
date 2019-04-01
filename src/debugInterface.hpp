#ifndef  _DEBUGINTERF
#define _DEBUGINTERFACE_H_
#include <Arduino.h>

String precUptimePrint(long a) {
  return String(a/1000)+"."+(a%1000);
}

String debug() {
  return String("[")+precUptimePrint(millis())+"] : ";
}

#endif
