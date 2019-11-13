#ifndef _VFDINTERFACE_H_
#define _VFDINTERFACE_H_
#include "HardwareSerial.h"
#include "Arduino.h"

void setCursor(int x, int y);
void clear();
void clearLine(int line);
void demo();
void setCodePage();
String toCP852(String string);

const String CP852[] = {String((char)164), String((char)143), String((char)168), //todo refactor
                        String((char)157), String((char)227), String((char)224), //maybe store in SPIFFS?
                        String((char)151), String((char)141), String((char)189),
                        String((char)165), String((char)134), String((char)169),
                        String((char)136), String((char)228), String((char)162),
                        String((char)152), String((char)171), String((char)190)};
const String UTF8[] = {String((char)0xc4)+String((char)0x84) ,String((char)0xc4)+String((char)0x86), String((char)0xc4)+String((char)0x98)
                ,String((char)0xc5)+String((char)0x81) ,String((char)0xc5)+String((char)0x83), String((char)0xc3)+String((char)0x93)
                ,String((char)0xc5)+String((char)0x9a) ,String((char)0xc5)+String((char)0xb9), String((char)0xc5)+String((char)0xbb)
                ,String((char)0xc4)+String((char)0x85) ,String((char)0xc4)+String((char)0x87), String((char)0xc4)+String((char)0x99)
                ,String((char)0xc5)+String((char)0x82) ,String((char)0xc5)+String((char)0x84), String((char)0xc3)+String((char)0xb3)
                ,String((char)0xc5)+String((char)0x9b) ,String((char)0xc5)+String((char)0xba), String((char)0xc5)+String((char)0xbc)};

#endif
