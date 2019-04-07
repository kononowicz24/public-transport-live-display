#ifndef _STOPSTABLEGENERATOR_H_
#define _STOPSTABLEGENERATOR_H_
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "VFDInterface.hpp"

boolean showStopDelayList(const char* host, int httpPort, String stopId) {
    boolean printedLines[] = {false, false, false};
    WiFiClient client;
    //const int httpPort = 88;
    if (!client.connect(host, httpPort)) {
      Serial1.println("connection failed");
      return false;
    }
    // We now create a URI for the request
    String url = "/delays";
    url += "?stopId=";
    url += stopId;

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

  const size_t bufferSize = JSON_ARRAY_SIZE(5) + JSON_OBJECT_SIZE(3) + 5*JSON_OBJECT_SIZE(12) + 1120;
  DynamicJsonBuffer jsonBuffer(bufferSize);

  while(client.available()){
      JsonObject& root = jsonBuffer.parseObject(client);

      for (int i=0; i<3; i++) {
        JsonObject& delay0 = root["delay"][i];
        int routeId = delay0["routeId"];
        int delayInSeconds = delay0["delayInSeconds"];
        String estimatedTime = delay0["estimatedTime"];
        String theoreticalTime = delay0["theoreticalTime"];
        String relation = delay0["headsign"];
        String delStr = "";
        if (routeId!=0) {
          clearLine(i);
          //Serial.println(routeId);
          //Serial.println(delayInSeconds);
          //Serial.println(estimatedTime);
          //prevDel = (String)delay0_routeId+" ("+delay0_delayInSeconds+") "+delay0_estimatedTime;
          setCursor(0,i);
          Serial1.print((String)routeId);
          delStr = (String)delayInSeconds;

          setCursor(4,i);
          String relation1= toCP852(relation);
          Serial1.print(relation1);
          //Serial1.print("(");
          setCursor(13-delStr.length(),i);
          Serial1.print((String)" "+ delStr+" ");
          setCursor(15,i);
          Serial1.print((String)estimatedTime);
          printedLines[i] = true;
        }
      }
  }
  for (int i=0; i<3; i++) {
    if (!printedLines[i]) clearLine(i);
  }
  client.stop();
  return true;
}


#endif
