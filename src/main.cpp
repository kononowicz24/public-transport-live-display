#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "NTPClient.h"

#include <ArduinoJson.h>

#include "VFDInterface.hpp"
#include "StopsTableGenerator.hpp"
#include "WeatherGenerator.hpp"
#include "debugInterface.hpp"

#include "passwords.h"

#include "LedControl_HW_SPI.h"

#include <Wire.h>
#include "SparkFun_TCA9534.h"
#include "SHT31.h"
#include "MAX44009.h"
#include "FastLED.h"

#include "i2cscan.hpp"

const char* ssid     = SSID;
const char* password = PASSWORD;

const char* stopId = "1331";//todo: make it selectable

#define STOPSREFRESHMILLIS 20000 //each 20sec
#define WEATHERREFRESHMILLIS 1200000 //each 20min
#define TIMEREFRESHMILLIS 1000 //each 1sec

uint8_t leds_config = 0;

long long stopsLastMillis = -1*STOPSREFRESHMILLIS; //so that both weather and
long long weatherLastMillis = -1*WEATHERREFRESHMILLIS; //stops refresh immediately
long long timeLastMillis = -1*TIMEREFRESHMILLIS; //stops refresh immediately
int utcOffsetInSeconds = 2 * 60 * 60;
/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.
 */
#define NUM_LEDS 7
#define LED_PIN 16

LedControl_HW_SPI segment = LedControl_HW_SPI(); 
TCA9534 expander;
SHT31 sht;
MAX44009 light;
CRGB led_strip[NUM_LEDS];

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void setup() {
  Serial.begin(115200); //debug one
  Serial1.begin(9600); //this one setups communication with the vfd display
  Wire.begin(4,5);//sda scl
  i2cscan();
  setCodePage();
  clear();
  delay(500);
  setCursor(0,0);

  expander.begin(Wire, 0x3f);
  expander.writeRegister(REGISTER_CONFIGURATION, 0xc0);
  expander.writeRegister(REGISTER_OUTPUT_PORT, 0);

  sht.begin(0x44);
  light.begin();
  FastLED.addLeds<NEOPIXEL, LED_PIN>(led_strip, NUM_LEDS);

  //digitalWrite(15,HIGH);
  pinMode(16,OUTPUT);
  digitalWrite(16,HIGH); //enable TXS0104

  segment.begin(15);
  segment.shutdown(0,false);
  segment.setScanLimit(0,4);
  segment.setIntensity(0,8);
  segment.clearDisplay(0);
  segment.setDigit(0,0,8,false);
  segment.setDigit(0,1,8,false);
  segment.setDigit(0,2,8,false);
  segment.setDigit(0,3,8,false);

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
    Serial.print(".");
  }
  pinMode(16, OUTPUT);
  digitalWrite(16, 1);
  setCursor(0,0);
  Serial.println("WiFi connected");
  Serial.println(debug()+"WiFi connected");
  setCursor(0,1);
  //Serial.println("IP address: ");
  Serial.print(WiFi.localIP());

  timeClient.begin();

}

void loop() {
  //if (value>0) delay(20000); else delay(3000);//FIXME
  //++value;
  //delay(1);
  //Serial.print(debug()+"connecting to ");
  //Serial.println(host);
  if (millis()>stopsLastMillis+STOPSREFRESHMILLIS) {
    /*Serial.println(debug()+" Trying to refresh delays");
     if (showStopDelayList("ckan2.multimediagdansk.pl", 80, stopId)) {
       stopsLastMillis = millis();
       Serial.println(debug()+" Delays up to date");
     }
     else stopsLastMillis+=10000;//wait for 10s to connect again*/
     leds_config++;
     if (leds_config>63) leds_config = 0;
     //expander.writeRegister(REGISTER_OUTPUT_PORT, leds_config);
     //Serial.println("tick "+leds_config);
     stopsLastMillis+=1000;
     sht.read(false);
     Serial.println(debug()+"T: "+(String)(sht.getTemperature())+" H: "+sht.getHumidity()+" tick: "+ leds_config);
     Serial.println(debug()+"Light: "+(String)light.get_lux());
  }
  if (millis()>weatherLastMillis+WEATHERREFRESHMILLIS) {
    Serial.println(debug()+" Trying to refresh weather");
     if (showWeatherline("api.openweathermap.org", 80, "7531002")) { //todo maybe change city?
       weatherLastMillis = millis();
       Serial.println(debug()+" Weather up to date");
     }
     weatherLastMillis+=200000; //wait for 200s to connect again
  }
  if (millis()>timeLastMillis+TIMEREFRESHMILLIS) {
      if (timeClient.update()) {
        int hr = timeClient.getHours();
        int mn = timeClient.getMinutes();
        segment.setDigit(0,0,hr/10,false);
        segment.setDigit(0,1,hr%10,false);
        segment.setDigit(0,2,mn/10,false);
        segment.setDigit(0,3,mn%10,false);
      } else {} // tutaj czerwonym ledem że czas nieaktualny
      timeLastMillis+=1000;
  }

  //rbg

  led_strip[0] = CRGB(255,255,255);
  led_strip[1] = CRGB(0,0,0);
  led_strip[2] = CRGB(0,0,255);
  led_strip[3] = CRGB(0,255,0);
  led_strip[4] = CRGB(255,0,0);
  led_strip[5] = CRGB(255,255,0);
  led_strip[6] = CRGB(255,0,255);
  FastLED.show();
  delay(500);

}
