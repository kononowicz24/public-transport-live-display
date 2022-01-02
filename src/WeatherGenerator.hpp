#ifndef _WEATHER_GENERATOR_H_
#define _WEATHER_GENERATOR_H_
#include "VFDInterface.hpp"
#include "Arduino.h"

#include "passwords.h"

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include "weather.h"
#include <cmath>

const String caridnals[] = { "N", "NNE", "NE", "ENE", "E", "ESE", "SE", "SSE", "S", "SSW", "SW", "WSW", "W", "WNW", "NW", "NNW", "N" };

Weather weather;
//https://api.openweathermap.org/data/2.5/weather?id=7531002&APPID=

String degreesToCardinalDetailed(int16_t degrees)
{
    degrees *= 10;
    return caridnals[(int)round((degrees % 3600) / 225.0f)];
}

boolean showWeatherline(const char* host, int httpPort, String cityId) {
  WiFiClient client;
  //const int httpPort = 88;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return false;
  }
  // We now create a URI for the request
  String url = "/data/2.5/weather";
  url += "?id=";
  url += cityId;
  url += "&lang=pl&APPID=";
  url += WEATHERAPIKEY_k24;

  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n"); //czy to generuje errory?
  client.print(String("GET ") + url + " HTTP/1.1\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return false;
    }
  }
  float main_temp;
  float main_wind_speed;
  uint16_t main_wind_direction;
  const char* main_description;
  const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 2*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(12) + 400;
  DynamicJsonBuffer jsonBuffer(capacity);
  while (client.available()) {

    JsonObject& root = jsonBuffer.parseObject(client);
    //JsonObject& weather_0 = root["weather"][0];
    JsonObject& main = root["main"];

    main_temp = main["temp"]; // 284.7
    weather.temp_degC = (int)( main_temp - 273.15);

    main_wind_speed = root["wind"]["speed"]; // 10.3
    weather.wind_speed = (int)(main_wind_speed*3.6);

    main_wind_direction = root["wind"]["deg"]; // 10.3
    weather.wind_direction = (int)(main_wind_direction);

    main_description = root["weather"][0]["description"];
    weather.description = String(main_description);
  }
  client.stop();

  clearLine(3);
  clearLine(4);
  setCursor(0,2);
  Serial1.print((String)""+(String)weather.temp_degC+(char)248+"C "+(String)weather.wind_speed+"km/h "+degreesToCardinalDetailed(weather.wind_direction)); // ascii - 248 
  setCursor(0,3);
  Serial1.print((String)""+(String)toCP852(weather.description)); // ascii - 248 
  //Serial.print((String)""+(String)temp+(char)176+"C "+(String)wind+"km/h "+(String)description); //win - 176, ciekawe co linuks ehhh

  return true;
}

boolean showWeather() {
  if (showWeatherline("api.openweathermap.org", 80, "7531002")) { //todo maybe change city?
    //if (VFD_on) {
      VFD_showWeather(weather);
    //}
    return true;
  } else {
    return false;
  }
}

#endif
