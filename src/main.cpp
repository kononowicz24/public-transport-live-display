#include <ESP8266WiFi.h>

#include <ArduinoJson.h>

#include <SoftwareSerial.h>

#include <Wire.h>
//#include <LiquidCrystal_I2C.h>

const char* ssid     = "UPC247602010";
const char* password = "HFLAZPSY";

const char* host = "87.98.237.99";
const char* stopId = "1331";

//LiquidCrystal_I2C Serial1(0x27, 16, 2);
//SoftwareSerial Serial1(D3,D4);

void setCursor(int x, int y) {
  char buf1[5];
  itoa(y+1, buf1, 10);
  char buf2[5];
  itoa(x+1, buf2, 10);
  Serial1.print((String)""+(char)27+"["+buf1+";"+buf2+"H");
}

void clear() {
  Serial1.print((String)""+(char)27+"[2J");
}


void setup() {
  Serial.begin(115200);
  delay(10);


  // We start by connecting to a WiFi network
  Serial1.begin(9600);
  clear();
  setCursor(0,0);
  //Serial1.setBacklight(50);
  //Serial.println();
  //Serial.println();
  Serial1.println("Connecting to ");
  //setCursor(0,1);
  //Serial1.println(ssid);
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
  Serial.println("WiFi connected");
  setCursor(0,1);
  //Serial.println("IP address: ");
  Serial1.print(WiFi.localIP());

}

int value = 0;

void loop() {
  if (value>0) delay(20000); else delay(3000);
  ++value;

  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 88;
  if (!client.connect(host, httpPort)) {
    Serial1.println("connection failed");
    return;
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
      return;
    }
  }

  const size_t bufferSize = JSON_ARRAY_SIZE(5) + JSON_OBJECT_SIZE(3) + 5*JSON_OBJECT_SIZE(12) + 1120;
  DynamicJsonBuffer jsonBuffer(bufferSize);
  //DynamicJsonDocument ;
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
      JsonObject& root = jsonBuffer.parseObject(client);
      String delStr = "";
      for (int i=0; i<3; i++) {
        JsonObject& delay0 = root["delay"][i];
        int routeId = delay0["routeId"];
        int delayInSeconds = delay0["delayInSeconds"];
        String estimatedTime = delay0["estimatedTime"];
        String theoreticalTime = delay0["theoreticalTime"];
        if (routeId!=0) {
          if (i==0) clear();
          Serial.println(routeId);
          Serial.println(delayInSeconds);
          Serial.println(estimatedTime);
          //prevDel = (String)delay0_routeId+" ("+delay0_delayInSeconds+") "+delay0_estimatedTime;
          setCursor(0,i);
          Serial1.print((String)routeId);
          delStr = (String)delayInSeconds;

          setCursor(4,i);
          Serial1.print("(");
          setCursor(9-delStr.length(),i);
          Serial1.print(delStr+")");
          setCursor(11,i);
          Serial1.print((String)estimatedTime);
        }
      }
  }


  Serial.println();
  Serial.println("closing connection");
}
