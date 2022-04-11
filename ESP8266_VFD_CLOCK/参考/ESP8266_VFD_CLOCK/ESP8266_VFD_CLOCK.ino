// ESP8266 ESP-01 Internet real time clock
 
 
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
 
#include <NTPClient.h>               // Include NTPClient library
#include <TimeLib.h>                 // Include Arduino time library

const char *ssid     = "godlike";
const char *password = "126shanliuyuanzhang";

WiFiUDP ntpUDP;
 
// 'time.nist.gov' is used (default server) with +1 hour offset (3600 seconds) 60 seconds (60000 milliseconds) update interval
NTPClient timeClient(ntpUDP, "time1.aliyun.com", 28800, 60000);
 
char Time[] = "TIME:00:00:00";
char Date[] = "DATE:00/00/2000";
byte last_second, second_, minute_, hour_, day_, month_;
int year_;
 
void setup() {
 
  Serial.begin(19200);
 
  WiFi.begin(ssid, password);
 
  //Serial.print("Connecting.");
  while ( WiFi.status() != WL_CONNECTED ) {
    delay(500);
    //Serial.print(".");
  }
  //Serial.println("connected");
 
  timeClient.begin();

    Serial.write(0x1B);
    Serial.write(0x40);
    Serial.write(0x0C);
}
 
void loop() {
 
  timeClient.update();
  unsigned long unix_epoch = timeClient.getEpochTime();    // Get Unix epoch time from the NTP server
 
  second_ = second(unix_epoch);
  if (last_second != second_) 
  {
 
    minute_ = minute(unix_epoch);
    hour_   = hour(unix_epoch);
    day_    = day(unix_epoch);
    month_  = month(unix_epoch);
    year_   = year(unix_epoch);
    weekday_= weekday(unix_epoch);
 
    Time[12] = second_ % 10 + 48;
    Time[11] = second_ / 10 + 48;
    Time[9]  = minute_ % 10 + 48;
    Time[8]  = minute_ / 10 + 48;
    Time[6]  = hour_   % 10 + 48;
    Time[5]  = hour_   / 10 + 48;
 
    Date[5]  = day_   / 10 + 48;
    Date[6]  = day_   % 10 + 48;
    Date[8]  = month_  / 10 + 48;
    Date[9]  = month_  % 10 + 48;
    Date[13] = (year_   / 10) % 10 + 48;
    Date[14] = year_   % 10 % 10 + 48;

 
    // Send time and date to serial monitor

    //Serial.write(0x1B);
    //Serial.write(0x52);
    //Serial.write(0x42);

    Serial.write(0x1B);
    Serial.write(0x5B);
    Serial.write(0x31);
    Serial.write(0x3B);
    Serial.write(0x33);
    Serial.write(0x48);
    
    Serial.print(Date);
    
    Serial.write(0x0D);

    Serial.write(0x1B);
    Serial.write(0x5B);
    Serial.write(0x32);
    Serial.write(0x3B);
    Serial.write(0x34);
    Serial.write(0x48);
    
    Serial.print(Time);

    Serial.write(0x0D);
 
    last_second = second_;
  }
 
  delay(200);
 
}
