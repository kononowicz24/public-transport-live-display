#ifndef _STOPSTABLEGENERATOR_H_
#define _STOPSTABLEGENERATOR_H_
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "VFDInterface.hpp"

void showStopDelayList(const char* host, int httpPort, String stopId);

#endif
