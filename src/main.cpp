#include <ESP8266WiFi.h>

#include <ArduinoJson.h>
#include <Wire.h>

#include "VFDInterface.hpp"
#include "StopsTableGenerator.hpp"
#include "WeatherGenerator.hpp"
#include "debugInterface.hpp"
#include "7segInterface.hpp"

#include "passwords.h"

const char* ssid     = SSID;
const char* password = PASSWORD;

const char* stopId = "1331";//todo: make it selectable

#define STOPSREFRESHMILLIS 20000 //each 20sec
#define WEATHERREFRESHMILLIS 1200000 //each 20min
#define DISPREFRESHMILLIS 3

long long stopsLastMillis = -1*STOPSREFRESHMILLIS; //so that both weather and
long long weatherLastMillis = -1*WEATHERREFRESHMILLIS; //stops refresh immediately
long long dispLastMillis = -1*DISPREFRESHMILLIS;

int digitIterator = 0;

void setup() {
  Serial.begin(115200); //debug one
  Serial1.begin(9600); //this one setups communication with the vfd display
  Wire.begin(D2, D1);
  Wire.beginTransmission(0x20);
  Wire.write(0x00); //goto 0x00 reg

  Wire.write(0x00);
  Wire.write(0x00); // set as output
  Wire.endTransmission();
  /*setCodePage();
  clear();
  //demo();
  delay(500);
  setCursor(0,0);

  Serial1.println("Connecting to ");
  Serial1.println(ssid);
  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  /*WiFi.mode(WIFI_STA);
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
  //delay(1000);*/
}

void loop() {
  //if (value>0) delay(20000); else delay(3000);//FIXME
  //++value;
  //delay(1);
  //Serial.print(debug()+"connecting to ");
  //Serial.println(host);
  /*if (millis()>stopsLastMillis+STOPSREFRESHMILLIS) {
    Serial.println(debug()+" Trying to refresh delays");
     if (showStopDelayList("87.98.237.99", 88, stopId)) {
       stopsLastMillis = millis();
       Serial.println(debug()+" Delays up to date");
     }
     else stopsLastMillis+=10000;//wait for 10s to connect again
  }
  if (millis()>weatherLastMillis+WEATHERREFRESHMILLIS) {
    Serial.println(debug()+" Trying to refresh weather");
     if (showWeatherline("api.openweathermap.org", 80, "7531002")) { //todo maybe change city?
       weatherLastMillis = millis();
       Serial.println(debug()+" Weather up to date");
     }
     weatherLastMillis+=200000; //wait for 200s to connect again
  }
  */
  if (millis()>dispLastMillis+DISPREFRESHMILLIS) {
    digitShow(1122, digitIterator);
    digitIterator++;
    digitIterator%=4;
    dispLastMillis = millis();
  }

}
