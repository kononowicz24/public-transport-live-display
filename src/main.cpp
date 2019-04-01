#include <ESP8266WiFi.h>

#include <ArduinoJson.h>

#include "VFDInterface.hpp"
#include "StopsTableGenerator.hpp"
#include "debugInterface.hpp"

#include "passwords.h"

const char* ssid     = SSID;
const char* password = PASSWORD;

const char* host = "87.98.237.99";
const char* stopId = "1331";//todo: make it selectable

void setup() {
  Serial.begin(115200);
  delay(10);
  // We start by connecting to a WiFi network
  Serial1.begin(9600);
  clear();
  setCursor(0,0);
  Serial1.println("Connecting to ");
  Serial1.println(ssid);
  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  //ESP.wdtFeed();
  //delay(10);
  //Serial1.println("WATCHDOG?");
  //ESP.wdtFeed();
  //delay(100);
  setCursor(0,2);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial1.print(".");
  }

  setCursor(0,0);
  Serial1.println("WiFi connected");
  Serial.println(debug()+"WiFi connected");
  setCursor(0,1);
  //Serial.println("IP address: ");
  Serial1.print(WiFi.localIP());

}

int value = 0;

void loop() {
  if (value>0) delay(20000); else delay(3000);
  ++value;

  Serial.print(debug()+"connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  showStopDelayList(host, 88, stopId);
  Serial.println(debug()+"closing connection");
}
