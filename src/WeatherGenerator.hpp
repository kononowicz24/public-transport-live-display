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
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return false;
    }
  }

  const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 2*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(12) + 400;
  DynamicJsonBuffer jsonBuffer(capacity);
  while (client.available()) {
    JsonObject& root = jsonBuffer.parseObject(client);

    //float coord_lon = root["coord"]["lon"]; // 18.69
    //float coord_lat = root["coord"]["lat"]; // 54.36

    JsonObject& weather_0 = root["weather"][0];
    //int weather_0_id = weather_0["id"]; // 800
    const char* weather_0_main = weather_0["main"]; // "Clear"
    const char* weather_0_description = weather_0["description"]; // "clear sky"
    //const char* weather_0_icon = weather_0["icon"]; // "01d"

    //const char* base = root["base"]; // "stations"

    JsonObject& main = root["main"];
    float main_temp = main["temp"]; // 284.7
    int temp = (int)( main_temp - 273.15);
    //int main_pressure = main["pressure"]; // 1018
    //int main_humidity = main["humidity"]; // 23
    //float main_temp_min = main["temp_min"]; // 283.15
    //float main_temp_max = main["temp_max"]; // 285.93

    //int visibility = root["visibility"]; // 10000

    float wind_speed = root["wind"]["speed"]; // 10.3
    int wind = (int)(wind_speed*3.6);

    setCursor(5,3);
    Serial1.print((String)""+(String)temp+"dC "+(String)wind+"km/h "+(String)weather_0_main);
  }
  return true;
}

#endif
