#include <ESP8266WiFi.h>

#include <ArduinoJson.h>

#include "VFDInterface.hpp"
#include "StopsTableGenerator.hpp"
#include "WeatherGenerator.hpp"
#include "debugInterface.hpp"

#include "passwords.h"

const char* ssid     = SSID;
const char* password = PASSWORD;

const char* stopId = "1331";//todo: make it selectable

#define STOPSREFRESHMILLIS 20000
#define WEATHERREFRESHMILLIS 1200000

long long stopsLastMillis = -20000;
long long weatherLastMillis = -1200000;

void setup() {
  Serial.begin(115200);
  delay(10);
  // We start by connecting to a WiFi network
  Serial1.begin(9600); //this one setups communication with the vfd display
  clear();
  setCursor(0,0);
  Serial1.println("Connecting to ");
  Serial1.println(ssid);
  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  setCursor(0,2);
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
  //delay(1000);
}

int value = 0;

void loop() {
  //if (value>0) delay(20000); else delay(3000);//FIXME
  //++value;

  //Serial.print(debug()+"connecting to ");
  //Serial.println(host);
  if (millis()>stopsLastMillis+STOPSREFRESHMILLIS) {
    Serial.println(debug()+" Trying to refresh delays");
     if (showStopDelayList("87.98.237.99", 88, stopId)) {
       stopsLastMillis = millis();
       Serial.println(debug()+" Delays up to date");
     }
     else stopsLastMillis+=2000;//wait for 20s to connect again
  }
  //todo maybe other cities
  if (millis()>weatherLastMillis+WEATHERREFRESHMILLIS) {
    Serial.println(debug()+" Trying to refresh weather");
     if (showWeatherline("api.openweathermap.org", 80, "7531002")) { //todo maybe change city?
       weatherLastMillis = millis();
       Serial.println(debug()+" Weather up to date");
     }
     else weatherLastMillis+=20000; //wait for 20s to connect again
  }
  //todo weather only once in 20min
  //Serial.println(debug()+"closing connection");
}
