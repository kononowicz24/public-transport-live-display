#include "VFDInterface.hpp"
//#include "WeatherGenerator.hpp"
#include <cstdlib>

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

void clearLine(int line) {
  setCursor(0, line);
  Serial1.print((String)""+(char)27+"[0K");
}

void demo() {
  //for (int i=127; i<255; i+=3) {
  //  Serial1.println((String)""+i+" "+(char)i+
  //                         " "+(i+1)+" "+(char)(i+1)+
  //                         " "+(i+2)+" "+(char)(i+2));
  //  delay(1500);
  //}
  for (int i=0; i<18; i++) {
    Serial1.println(CP852[i]);
    delay(1000);
  }
}

void setCodePage() {
  Serial1.println((String)""+(char)27+"R"+(char)0x32);
}

String toCP852(String string) {
  String result = string;
  //Serial.println("Przed cp");

  //Serial.println("Po UTF8");
  for (int i=0; i<18; i++) {
    result.replace(UTF8[i], CP852[i]);
  }
  return result;
}

void showWeather(Weather weather) {
  clearLine(4);
  setCursor(0,2);
  Serial1.print((String)""+(String)weather.temp_degC+(char)248+"C "+(String)weather.wind_speed+"km/h "+(String)weather.wind_direction); // ascii - 248 
  setCursor(0,3);
  Serial1.print((String)""+(String)weather.description); // ascii - 248 
  //Serial.print((String)""+(String)temp+(char)176+"C "+(String)wind+"km/h "+(String)description); //win - 176, ciekawe co linuks ehhh
}
