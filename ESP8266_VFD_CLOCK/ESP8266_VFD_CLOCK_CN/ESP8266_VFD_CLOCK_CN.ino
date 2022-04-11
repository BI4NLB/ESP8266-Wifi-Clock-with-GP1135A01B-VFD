// ESP8266 ESP-12F Internet real time clock
 
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
 
#include <NTPClient.h>               // Include NTPClient library
#include <TimeLib.h>                 // Include Arduino time library

//const char *ssid     = "godlike";
//const char *password = "126shanliuyuanzhang";

const char *ssid     = "rizhao2016";
const char *password = "a13468316461a";

WiFiUDP ntpUDP;
 
// 'time1.aliyun.com' is used (default server) with +8 hour offset (28800 seconds) 10 seconds (10000 milliseconds) update interval
NTPClient timeClient(ntpUDP, "time1.aliyun.com", 28800, 10000);
//NTPClient timeClient(ntpUDP, "time.apple.com", 28800, 60000);

 
char Time[] = "00:00:00    ";
char Date[] = "2000  00  00";
//String Time_Fmt = "";
byte last_second, second_, minute_, hour_, day_, month_, weekday_;
int year_;
 
void setup() {
 
  Serial.begin(19200);
 
  WiFi.begin(ssid, password);
  delay(1000);
  Serial.print("Connecting");
  
  while ( WiFi.status() != WL_CONNECTED ) 
  { 
    Serial.print(".");
    delay(500);   
  }
  Serial.write(0x0C);  //clear VFD
  Serial.write(0x0D);
  Serial.print("WiFi Connected");
  delay(1000);
  Serial.write(0x0C);  //clear VFD
 
  timeClient.begin();

    Serial.write(0x1B);
    Serial.write(0x40);
    Serial.write(0x0C);
}
 
void loop() 
{
  timeClient.update();
  unsigned long unix_epoch = timeClient.getEpochTime();    // Get Unix epoch time from the NTP server
  second_ = second(unix_epoch);
  
  int analogValue = analogRead(A0);
  float millivolts = (analogValue/1024.0) * 1000; //LM35 output value in milivolt
  int celsius = millivolts/10;

  
  if (last_second != second_) 
  {
 
    minute_ = minute(unix_epoch);
    hour_   = hour(unix_epoch);
    day_    = day(unix_epoch);
    month_  = month(unix_epoch);
    year_   = year(unix_epoch);
    weekday_= weekday(unix_epoch);
 
    Time[7] = second_ % 10 + 48;
    Time[6] = second_ / 10 + 48;
    Time[4]  = minute_ % 10 + 48;
    Time[3]  = minute_ / 10 + 48;
    Time[1]  = hour_   % 10 + 48;
    Time[0]  = hour_   / 10 + 48;

//    Time_Fmt=timeClient.getFormattedTime();
 
    Date[10]  = day_   / 10 + 48;
    Date[11]  = day_   % 10 + 48;
    Date[6]  = month_  / 10 + 48;
    Date[7]  = month_  % 10 + 48;
    Date[2] = (year_   / 10) % 10 + 48;
    Date[3] = year_   % 10 % 10 + 48;

    // Send time and date to VFD

    Serial.write(0x1B);
    Serial.write(0x5B);
    Serial.write(0x31);
    Serial.write(0x3B);
    Serial.write(0x31);
    Serial.write(0x48);
    
    Serial.write(Date[0]);
    Serial.write(Date[1]);
    Serial.write(Date[2]);
    Serial.write(Date[3]);//year
    
    Serial.write(0xC4);
    Serial.write(0xEA);//年
    
    Serial.write(Date[6]);
    Serial.write(Date[7]);//month
    
    Serial.write(0xD4);
    Serial.write(0xC2);//月
    
    Serial.write(Date[10]);
    Serial.write(Date[11]);//day
    
    Serial.write(0xC8);
    Serial.write(0xD5);//日

    Serial.write(0xD0);
    Serial.write(0xC7);  //星
    Serial.write(0xC6);
    Serial.write(0xDA);  //期    

    switch (weekday_)
    {
        case 0x01:
            Serial.write(0xC8);
            Serial.write(0xD5);//日
            break;
        case 0x02:
            Serial.write(0xD2);
            Serial.write(0xBB);//一
            break;
        case 0x03:
            Serial.write(0xB6);
            Serial.write(0xFE);//二        
            break;
        case 0x04:
            Serial.write(0xC8);
            Serial.write(0xFD);//三
            break;
        case 0x05:
            Serial.write(0xCB);
            Serial.write(0xC4);//四
            break;
        case 0x06:
            Serial.write(0xCE);
            Serial.write(0xE5);//五       
            break;
        case 0x07:
            Serial.write(0xC1);
            Serial.write(0xF9);//六        
            break;                  

        default:            
            Serial.write(0x20);
            Serial.write(0x20);//space x 2
    }
    
    Serial.write(0x0D);

    Serial.write(0x1B);
    Serial.write(0x5B);
    Serial.write(0x32);
    Serial.write(0x3B);
    Serial.write(0x33);
    Serial.write(0x48);
        
    Serial.print(Time);
  
    Serial.print(celsius);
    
    Serial.write(0xA1);
    Serial.write(0xE6);//'C     

    Serial.write(0x0D);
 
    last_second = second_;
  }
 
  delay(200);
  
 
}
