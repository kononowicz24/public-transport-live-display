#ifndef _STOPSTABLEGENERATOR_H_
#define _STOPSTABLEGENERATOR_H_
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "VFDInterface.hpp"
#include "passwords.h"
#include "debugInterface.hpp"

boolean showStopDelayList(const char *host, int httpsPort, String stopId)
{
    boolean printedLines[] = {false, false, false};
    WiFiClientSecure httpsClient; //Declare object of class WiFiClient

    Serial.println(debug("STOPS")+host);

    Serial.println(String(debug("STOPS"))+"Using fingerprint '"+FINGERPRINT_CKAN);
    httpsClient.setFingerprint(FINGERPRINT_CKAN);
    httpsClient.setTimeout(15000); // 15 Seconds
    //delay(1000);

    Serial.println(debug("STOPS")+"HTTPS Connecting");
    int r = 0; //retry counter
    while ((!httpsClient.connect(host, httpsPort)) && (r < 30))
    {
        delay(100);
        Serial.print(".");
        r++;
    }
    if (r == 30)
    {
        Serial.println(debug("STOPS")+"Connection failed");
        return false;
    }
    else
    {
        Serial.println(debug("STOPS")+"Connected to web");
    }

    // We now create a URI for the request
    String url = "/delays";
    url += "?stopId=";
    url += stopId;

    Serial.print(debug("STOPS")+"requesting URL: ");
    Serial.println(host + url);

    httpsClient.print(String("GET ") + url + " HTTP/1.1\r\n" +
                      "Host: " + host + "\r\n" +
                      "Connection: close\r\n\r\n");

    Serial.println(debug("STOPS")+"request sent");

    unsigned long timeout = millis();
    while (httpsClient.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println(debug("STOPS")+">>> Client Timeout !");
        httpsClient.stop();
        return false;
      }
    }

  const size_t bufferSize = JSON_ARRAY_SIZE(5) + JSON_OBJECT_SIZE(3) + 5*JSON_OBJECT_SIZE(12) + 1120;
  DynamicJsonBuffer jsonBuffer(bufferSize);

  while(httpsClient.available()){
      JsonObject& root = jsonBuffer.parseObject(httpsClient);

      for (int i=0; i<2; i++) {
        JsonObject& delay0 = root["delay"][i];
        int routeId = delay0["routeId"];
        int delayInSeconds = delay0["delayInSeconds"];
        String estimatedTime = delay0["estimatedTime"];
        String theoreticalTime = delay0["theoreticalTime"];
        String relation = delay0["headsign"];
        String delStr = "";
        if (routeId!=0) {
          clearLine(i);
          setCursor(0,i);
          Serial.print(debug("STOPS")+(String)routeId);
          Serial1.print((String)routeId);
          delStr = (String)delayInSeconds;
          setCursor(4,i);
          String relation1= toCP852(relation);
          Serial.print(relation1);
          Serial1.print(relation1);
          setCursor(13-delStr.length(),i);
          Serial.print((String)" "+ delStr+" ");
          Serial1.print((String)" "+ delStr+" ");
          setCursor(15,i);
          Serial.println((String)estimatedTime);
          Serial1.print((String)estimatedTime);
          printedLines[i] = true;
        }
      }
  }
  for (int i=0; i<3; i++) {
    if (!printedLines[i]) clearLine(i);
  }
  httpsClient.stop();
  return true;
}

#endif
