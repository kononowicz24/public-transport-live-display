#ifndef _WEATHER_GENERATOR_H_
#define _WEATHER_GENERATOR_H_
#include "VFDInterface.hpp"
#include "Arduino.h"
#include "passwords.h"
#include "VFDInterface.hpp"

//https://api.openweathermap.org/data/2.5/weather?id=7531002&APPID=

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
  url += "&APPID=";
  url += WEATHERAPIKEY;

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
  float wind_speed;
  int temp;
  int wind;
  const char* description;
  const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 2*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(12) + 400;
  DynamicJsonBuffer jsonBuffer(capacity);
  while (client.available()) {

    JsonObject& root = jsonBuffer.parseObject(client);
    //JsonObject& weather_0 = root["weather"][0];
    JsonObject& main = root["main"];
    main_temp = main["temp"]; // 284.7
    //Serial.println(main_temp);
    temp = (int)( main_temp - 273.15);
    wind_speed = root["wind"]["speed"]; // 10.3
    wind = (int)(wind_speed*3.6);
    description = root["weather"][0]["description"];
  }
  client.stop();
  clearLine(4);
  setCursor(0,3);
  Serial1.print((String)""+(String)temp+(char)248+"C "+(String)wind+"km/h "+(String)description); // ascii - 248 
  Serial.print((String)""+(String)temp+(char)176+"C "+(String)wind+"km/h "+(String)description); //win - 176, ciekawe co linuks ehhh
  return true;
}

#endif
