#include <ESP8266WiFi.h>

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
#include "Adafruit_SI1145.h"

#include "i2cscan.hpp"

const char* ssid     = SSID;
const char* password = PASSWORD;

const char* stopId = "1331";//todo: make it selectable

#define STOPSREFRESHMILLIS 20000 //each 20sec
#define WEATHERREFRESHMILLIS 1200000 //each 20min

uint8_t leds_config = 0;

long long stopsLastMillis = -1*STOPSREFRESHMILLIS; //so that both weather and
long long weatherLastMillis = -1*WEATHERREFRESHMILLIS; //stops refresh immediately
/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.
 */

LedControl_HW_SPI segment = LedControl_HW_SPI(); 
TCA9534 expander;
SHT31 sht;
Adafruit_SI1145 uv;

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

  uv.begin(0x60);

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
     Serial1.println("T: "+(String)(sht.getTemperature())+" H: "+sht.getHumidity()+" tick: "+ leds_config);
     Serial.println("LIGHT: "+(String)(uv.readVisible()-256)+" IR: "+ (uv.readIR()-256)+" PROX: "+(uv.readProx()-256)+" UV: "+uv.readUV());
  }
  if (millis()>weatherLastMillis+WEATHERREFRESHMILLIS) {
    Serial.println(debug()+" Trying to refresh weather");
     if (showWeatherline("api.openweathermap.org", 80, "7531002")) { //todo maybe change city?
       weatherLastMillis = millis();
       Serial.println(debug()+" Weather up to date");
     }
     weatherLastMillis+=200000; //wait for 200s to connect again
  }

}
