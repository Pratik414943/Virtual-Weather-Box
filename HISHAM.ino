#include <Arduino.h>
#include <ESP8266WiFi.h>
#include<BlynkSimpleEsp8266.h>
#include <JsonListener.h>
#include <time.h>
#include "OpenWeatherMapCurrent.h"
#include <Wire.h>
#include<Adafruit_GFX.h>
#include<Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET  LED_BUILTIN    // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3c ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


 

OpenWeatherMapCurrent client;
String OPEN_WEATHER_MAP_APP_ID = "c3451a3e77f368c4705d35570b7dbde3";
String OPEN_WEATHER_MAP_LOCATION_ID = "1269321";
String OPEN_WEATHER_MAP_LANGUAGE = "en";
boolean IS_METRIC = true;

const char* ESP_HOST_NAME = "esp-" + ESP.getFlashChipId();
const char* ssid    = "pratik";
const char* password = "12345678";

//int timezone = -5.5 * 3600;
int timezone = 19800;
int dst = 0;

   
WiFiClient wifiClient;

void connectWifi() 
{
  WiFi.begin(ssid, password);
   display.setTextSize(1);//loed
  display.setTextColor(WHITE);
   display.setCursor(0,0);
  display.print("Connecting to ");
  display.println(ssid);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
   display.print(".");
  }
  display.println("");
  display.println("WiFi connected!");
  display.println(WiFi.localIP());
  display.println();
  display.display();
  display.clearDisplay();
}


void setup() 
{
  Serial.begin(115200);
  delay(500);

   if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  display.clearDisplay();//oled
  display.setTextSize(1);//loed
  display.setTextColor(WHITE);//oled
  display.display();//oled

  connectWifi();


 
  Serial.println("\n\nNext Loop-Step: " + String(millis()) + ":");


  
  configTime(timezone, dst, "pool.ntp.org","time.nist.gov");
}

void loop()
{
  OpenWeatherMapCurrentData data;
  client.setLanguage(OPEN_WEATHER_MAP_LANGUAGE);
  client.setMetric(IS_METRIC);
  client.updateCurrentById(&data, OPEN_WEATHER_MAP_APP_ID, OPEN_WEATHER_MAP_LOCATION_ID);

  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);
     
  display.setCursor(1,0);
 display.print("Date : ");
  display.print(p_tm->tm_mday);
  display.print("/");
  display.print(p_tm->tm_mon + 1);
  display.print("/");
  display.print(p_tm->tm_year + 1900);
  display.println(" ");
  display.print("Time : ");
  display.print(p_tm->tm_hour);
  display.print(":");
  display.print(p_tm->tm_min);
  display.print(":");
  display.println(p_tm->tm_sec);
  display.printf("Main: %s\n", data.main.c_str());
  display.printf("Description: %s\n", data.description.c_str());
  display.printf("Temp: %f\n", data.temp);
  display.printf("Clouds: %d\n", data.clouds);
  display.display();
  display.clearDisplay();
 
//  String c=data.description.c_str(); 
//  int H=p_tm->tm_hour;
//  int t=data.temp;
//
//  Serial.println(c +" " + H +" "+ t);
}
  

  /*-----------------------Temperature-------------------------*/
 
