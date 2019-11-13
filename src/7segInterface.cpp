#include "7segInterface.hpp"
#include <Wire.h>
//#include <cmath>
#include "Arduino.h"

void digitShow(int a, int i) {
  Wire.beginTransmission(0x20);
  Wire.write(0x12); //goto 0x12 register - portA, 0x13 portB;

  Wire.write(digits[(int)(a/pow(10,i))%10]);
  Wire.write(segments[i]);
  int error = Wire.endTransmission();
  //Serial.println("test1");
  //Serial.println(digits[(int)(a/pow(10,i))%10]);
  //Serial.println(segments[i]);
}
