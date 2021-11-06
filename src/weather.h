#ifndef __WEATHER_H_
#define __WEATHER_H_
#include <Arduino.h>

struct Weather {
  uint8_t wind_speed = 0;
  uint16_t wind_direction = 0;
  int8_t temp_degC = 0;
  String description;
};


#endif
