#include <Arduino.h>
#include <ESP8266WiFi.h>
#include<BlynkSimpleEsp8266.h>
#include <JsonListener.h>
#include <time.h>
#include <FastLED.h>
#include "OpenWeatherMapCurrent.h"
#include <Wire.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x3F, 20,4);
Servo myservo;
int pos=0; 

const int pump=D5;//pump

#include<Adafruit_GFX.h>
#include<Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET  LED_BUILTIN    // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3c ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



#define LED_PIN D7
#define NUM_LEDS 60

 CRGB leds[NUM_LEDS];

OpenWeatherMapCurrent client;
String OPEN_WEATHER_MAP_APP_ID = "c3451a3e77f368c4705d35570b7dbde3";
String OPEN_WEATHER_MAP_LOCATION_ID = "1262180";
String OPEN_WEATHER_MAP_LANGUAGE = "en";
boolean IS_METRIC = true;

const char* ESP_HOST_NAME = "esp-" + ESP.getFlashChipId();

//char auth[]=BLYNK_AUTH_TOKEN;

const char* ssid    = "pratik";
const char* password = "12345678";

//int timezone = -5.5 * 3600;
int timezone = 5.5* 3600;
int dst = 0;

   
WiFiClient wifiClient;

void connectWifi() 
{
  WiFi.begin(ssid, password);
   display.setTextSize(1);//loed
  display.setTextColor(WHITE);
   display.setCursor(0,0);
//  Serial.print("Connecting to ");
  display.print("Connecting to ");
//  Serial.println(ssid);
  display.println(ssid);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
//    Serial.print(".");
      display.print(".");
  }
//  Serial.println("");
//  Serial.println("WiFi connected!");
//  Serial.println(WiFi.localIP());
//  Serial.println();
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
 
  
 lcd.init ();//LCD Display
 lcd.backlight();
 lcd.clear();
 
  

   if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  myservo.attach(D3);//servo

  
  display.clearDisplay();//oled
  display.setTextSize(1);//loed
  display.setTextColor(WHITE);//oled
  display.display();//oled

  connectWifi();

  pinMode(D5,OUTPUT);//PUMP
 
  Serial.println("\n\nNext Loop-Step: " + String(millis()) + ":");


  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
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

    
    lcd.setCursor(1, 0);
    lcd.print("WEATHER STATION");
  
   
  digitalWrite(D5,HIGH);
 delay(5000);
  digitalWrite(D5,LOW);
  delay(5000);
  
   myservo.write(90);
   
  display.setCursor(1,0);
 display.print("Date : ");
  display.print(p_tm->tm_mday);
  display.print("/");
  display.print(p_tm->tm_mon + 1);
  display.print("/");
  display.print(p_tm->tm_year + 1900);
  display.println(" ");
  display.print("Time : ");
  display.print(p_tm->tm_hour-2);
  display.print(":");
  display.print(p_tm->tm_min+30);
  display.print(":");
  display.println(p_tm->tm_sec);
  display.printf("Main: %s\n", data.main.c_str());
  display.printf("Description: %s\n", data.description.c_str());
  display.printf("Temp: %f\n", data.temp);
  display.printf("Clouds: %d\n", data.clouds);
  display.display();
  display.clearDisplay();
 
  String c=data.description.c_str(); 
  int h=p_tm->tm_hour;
  int temp=data.temp;

  Serial.println(c +" " + h +" "+ temp);

  /*------------------------Morning-------------------------*/
  if(h >= 5 && h < 9){
  if(c == "clear sky" || c == "few clouds" || c == "haze")
   Serial.println(h + " " + c);
  {
   leds[24]=CRGB(255, 47, 0); 
   leds[23]=CRGB(36, 0, 16);
   
   leds[16]=CRGB(0,0,0); 
   leds[17]=CRGB(0,0,0); 
   leds[18]=CRGB(0,0,0); 
   leds[19]=CRGB(255, 255, 255);
   leds[20]=CRGB(0,0,0); 
   leds[21]=CRGB(0,0,0);
   leds[22]=CRGB(0,0,0);
   FastLED.show();
   delay(5000);
  }
  if( c == "scattered clouds")
   Serial.println(h + " " + c);
  {
   leds[24]=CRGB(255, 47, 0); 
   leds[23]=CRGB(36, 0, 16); 

   leds[16]=CRGB(5, 173, 245); 
   leds[17]=CRGB(245, 36, 5); 
   leds[18]=CRGB(245, 36, 5); 
   leds[19]=CRGB(245, 36, 5);
   leds[20]=CRGB(245, 36, 5); 
   leds[21]=CRGB(245, 36, 5);
   leds[22]=CRGB(245, 36, 5);
   FastLED.show();
   delay(5000);
  }
  if((c == "broken clouds" || c == "overcast clouds"))
   Serial.println(h + " " + c);
  {
   leds[16]=CRGB(24, 82, 89); 
   leds[17]=CRGB(24, 82, 89); 
   leds[18]=CRGB(104, 0, 214); 
   leds[19]=CRGB(104, 0, 214);
   leds[20]=CRGB(104, 0, 214); 
   leds[21]=CRGB(99, 12, 6);
   leds[22]=CRGB(99, 12, 6);
   
   leds[24]=CRGB(255, 47, 0); 
   leds[23]=CRGB(36, 0, 16); 
   FastLED.show();
   delay(5000);
  }
  if((c == "light rain" || c == "moderate rain" || c == "heavy intensity rain"))
   Serial.println(h + " " + c);
  {
   leds[24]=CRGB(255, 47, 0); 
   leds[23]=CRGB(36, 0, 16);
   FastLED.show();
   
   leds[16]=CRGB(59, 0, 79); 
   leds[17]=CRGB(59, 0, 79); 
   leds[18]=CRGB(59, 0, 79); 
   leds[19]=CRGB(59, 0, 79);
   leds[20]=CRGB(59, 0, 79); 
   leds[21]=CRGB(59, 0, 79);
   leds[22]=CRGB(59, 0, 79);
   FastLED.show();
   delay(50);
   leds[16]=CRGB(255, 255, 255); 
   leds[17]=CRGB(255, 255, 255); 
   leds[18]=CRGB(255, 255, 255); 
   leds[19]=CRGB(255, 255, 255);
   leds[20]=CRGB(255, 255, 255); 
   leds[21]=CRGB(255, 255, 255);
   leds[22]=CRGB(255, 255, 255);
   FastLED.show();
   delay(100);
   leds[16]=CRGB(59, 0, 79); 
   leds[17]=CRGB(59, 0, 79); 
   leds[18]=CRGB(59, 0, 79); 
   leds[19]=CRGB(59, 0, 79);
   leds[20]=CRGB(59, 0, 79); 
   leds[21]=CRGB(59, 0, 79);
   leds[22]=CRGB(59, 0, 79);
   FastLED.show();
   delay(500);
   leds[16]=CRGB(255, 255, 255); 
   leds[17]=CRGB(255, 255, 255); 
   leds[18]=CRGB(255, 255, 255); 
   leds[19]=CRGB(255, 255, 255);
   leds[20]=CRGB(255, 255, 255); 
   leds[21]=CRGB(255, 255, 255);
   leds[22]=CRGB(255, 255, 255);
   FastLED.show();
   delay(100);
   leds[16]=CRGB(59, 0, 79); 
   leds[17]=CRGB(59, 0, 79); 
   leds[18]=CRGB(59, 0, 79); 
   leds[19]=CRGB(59, 0, 79);
   leds[20]=CRGB(59, 0, 79); 
   leds[21]=CRGB(59, 0, 79);
   leds[22]=CRGB(59, 0, 79);
   FastLED.show();
   delay(50);
   leds[16]=CRGB(255, 255, 255); 
   leds[17]=CRGB(255, 255, 255); 
   leds[18]=CRGB(255, 255, 255); 
   leds[19]=CRGB(255, 255, 255);
   leds[20]=CRGB(255, 255, 255); 
   leds[21]=CRGB(255, 255, 255);
   leds[22]=CRGB(255, 255, 255);
   FastLED.show();
   delay(100);
   leds[16]=CRGB(59, 0, 79); 
   leds[17]=CRGB(59, 0, 79); 
   leds[18]=CRGB(59, 0, 79); 
   leds[19]=CRGB(59, 0, 79);
   leds[20]=CRGB(59, 0, 79); 
   leds[21]=CRGB(59, 0, 79);
   leds[22]=CRGB(59, 0, 79);
   FastLED.show();
   delay(500);
   leds[16]=CRGB(255, 255, 255); 
   leds[17]=CRGB(255, 255, 255); 
   leds[18]=CRGB(255, 255, 255); 
   leds[19]=CRGB(255, 255, 255);
   leds[20]=CRGB(255, 255, 255); 
   leds[21]=CRGB(255, 255, 255);
   leds[22]=CRGB(255, 255, 255);
   FastLED.show();
   delay(100);
   leds[16]=CRGB(59, 0, 79); 
   leds[17]=CRGB(59, 0, 79); 
   leds[18]=CRGB(59, 0, 79); 
   leds[19]=CRGB(59, 0, 79);
   leds[20]=CRGB(59, 0, 79); 
   leds[21]=CRGB(59, 0, 79);
   leds[22]=CRGB(59, 0, 79);
   FastLED.show();
   delay(500);
   leds[16]=CRGB(255, 255, 255); 
   leds[17]=CRGB(255, 255, 255); 
   leds[18]=CRGB(255, 255, 255); 
   leds[19]=CRGB(255, 255, 255);
   leds[20]=CRGB(255, 255, 255); 
   leds[21]=CRGB(255, 255, 255);
   leds[22]=CRGB(255, 255, 255);
   FastLED.show();
   delay(100);
   leds[16]=CRGB(59, 0, 79); 
   leds[17]=CRGB(59, 0, 79); 
   leds[18]=CRGB(59, 0, 79); 
   leds[19]=CRGB(59, 0, 79);
   leds[20]=CRGB(59, 0, 79); 
   leds[21]=CRGB(59, 0, 79);
   leds[22]=CRGB(59, 0, 79);
   FastLED.show();
   delay(50);
   leds[16]=CRGB(255, 255, 255); 
   leds[17]=CRGB(255, 255, 255); 
   leds[18]=CRGB(255, 255, 255); 
   leds[19]=CRGB(255, 255, 255);
   leds[20]=CRGB(255, 255, 255); 
   leds[21]=CRGB(255, 255, 255);
   leds[22]=CRGB(255, 255, 255);
   FastLED.show();
   delay(100);
   leds[16]=CRGB(59, 0, 79); 
   leds[17]=CRGB(59, 0, 79); 
   leds[18]=CRGB(59, 0, 79); 
   leds[19]=CRGB(59, 0, 79);
   leds[20]=CRGB(59, 0, 79); 
   leds[21]=CRGB(59, 0, 79);
   leds[22]=CRGB(59, 0, 79);
   FastLED.show();
   delay(500);
   leds[16]=CRGB(255, 255, 255); 
   leds[17]=CRGB(255, 255, 255); 
   leds[18]=CRGB(255, 255, 255); 
   leds[19]=CRGB(255, 255, 255);
   leds[20]=CRGB(255, 255, 255); 
   leds[21]=CRGB(255, 255, 255);
   leds[22]=CRGB(255, 255, 255);
   FastLED.show();
   delay(100);
   leds[16]=CRGB(59, 0, 79); 
   leds[17]=CRGB(59, 0, 79); 
   leds[18]=CRGB(59, 0, 79); 
   leds[19]=CRGB(59, 0, 79);
   leds[20]=CRGB(59, 0, 79); 
   leds[21]=CRGB(59, 0, 79);
   leds[22]=CRGB(59, 0, 79);
   FastLED.show();
   delay(50);
   leds[16]=CRGB(255, 255, 255); 
   leds[17]=CRGB(255, 255, 255); 
   leds[18]=CRGB(255, 255, 255); 
   leds[19]=CRGB(255, 255, 255);
   leds[20]=CRGB(255, 255, 255); 
   leds[21]=CRGB(255, 255, 255);
   leds[22]=CRGB(255, 255, 255);
   FastLED.show();
   delay(100);
   leds[16]=CRGB(59, 0, 79); 
   leds[17]=CRGB(59, 0, 79); 
   leds[18]=CRGB(59, 0, 79); 
   leds[19]=CRGB(59, 0, 79);
   leds[20]=CRGB(59, 0, 79); 
   leds[21]=CRGB(59, 0, 79);
   leds[22]=CRGB(59, 0, 79);
   FastLED.show();
   delay(500);
   leds[16]=CRGB(255, 255, 255); 
   leds[17]=CRGB(255, 255, 255); 
   leds[18]=CRGB(255, 255, 255); 
   leds[19]=CRGB(255, 255, 255);
   leds[20]=CRGB(255, 255, 255); 
   leds[21]=CRGB(255, 255, 255);
   leds[22]=CRGB(255, 255, 255);
   FastLED.show();
   delay(100);
   leds[16]=CRGB(59, 0, 79); 
   leds[17]=CRGB(59, 0, 79); 
   leds[18]=CRGB(59, 0, 79); 
   leds[19]=CRGB(59, 0, 79);
   leds[20]=CRGB(59, 0, 79); 
   leds[21]=CRGB(59, 0, 79);
   leds[22]=CRGB(59, 0, 79);
   FastLED.show();
   delay(500);
   leds[16]=CRGB(255, 255, 255); 
   leds[17]=CRGB(255, 255, 255); 
   leds[18]=CRGB(255, 255, 255); 
   leds[19]=CRGB(255, 255, 255);
   leds[20]=CRGB(255, 255, 255); 
   leds[21]=CRGB(255, 255, 255);
   leds[22]=CRGB(255, 255, 255);
   FastLED.show();
   delay(100);
   leds[16]=CRGB(59, 0, 79); 
   leds[17]=CRGB(59, 0, 79); 
   leds[18]=CRGB(59, 0, 79); 
   leds[19]=CRGB(59, 0, 79);
   leds[20]=CRGB(59, 0, 79); 
   leds[21]=CRGB(59, 0, 79);
   leds[22]=CRGB(59, 0, 79);
   FastLED.show();
   delay(500);
   leds[16]=CRGB(255, 255, 255); 
   leds[17]=CRGB(255, 255, 255); 
   leds[18]=CRGB(255, 255, 255); 
   leds[19]=CRGB(255, 255, 255);
   leds[20]=CRGB(255, 255, 255); 
   leds[21]=CRGB(255, 255, 255);
   leds[22]=CRGB(255, 255, 255);
   FastLED.show();
   delay(100);
   leds[16]=CRGB(59, 0, 79); 
   leds[17]=CRGB(59, 0, 79); 
   leds[18]=CRGB(59, 0, 79); 
   leds[19]=CRGB(59, 0, 79);
   leds[20]=CRGB(59, 0, 79); 
   leds[21]=CRGB(59, 0, 79);
   leds[22]=CRGB(59, 0, 79);
   delay(200);
   }
  }


  /*------------------------Afternoon-------------------------*/
  if((h >= 9 && h < 17)){
  if((c == "clear sky" || c == "few clouds" || c == "haze"))
   Serial.println(h + " " + c);
  {
   leds[23]=CRGB(255, 47, 0); 
   leds[24]=CRGB(255, 42, 0);
   
   leds[16]=CRGB(245, 187, 93); 
   leds[17]=CRGB(245, 187, 93); 
   leds[18]=CRGB(245, 187, 93); 
   leds[19]=CRGB(245, 187, 93);
   leds[20]=CRGB(245, 187, 93); 
   leds[21]=CRGB(245, 187, 93);
   leds[22]=CRGB(245, 187, 93);
   FastLED.show();
   delay(5000);
  }
  if(c == "scattered clouds")
   Serial.println(h + " " + c);
  {
   leds[16]=CRGB(255, 0, 0); 
   leds[17]=CRGB(255, 0, 0); 
   leds[18]=CRGB(230, 252, 255); 
   leds[19]=CRGB(230, 252, 255);
   leds[20]=CRGB(230, 252, 255); 
   leds[21]=CRGB(230, 252, 255);
   leds[22]=CRGB(230, 252, 255);
   
   leds[23]=CRGB(255, 47, 0); 
   leds[24]=CRGB(255, 42, 0);
    FastLED.show();
    delay(5000);
  }
  if((c == "broken clouds" || c == "overcast clouds"))
   Serial.println(h + " " + c);
  {
   leds[16]=CRGB(24, 82, 89); 
   leds[17]=CRGB(24, 82, 89); 
   leds[18]=CRGB(24, 82, 89); 
   leds[19]=CRGB(41, 8, 69);
   leds[20]=CRGB(41, 8, 69); 
   leds[21]=CRGB(99, 12, 6);
   leds[22]=CRGB(99, 12, 6);
   
   leds[23]=CRGB(255, 47, 0); 
   leds[24]=CRGB(255, 42, 0);
    FastLED.show();
    delay(5000);
  }
  if((c == "light rain" || c == "moderate rain" || c == "heavy intensity rain"))
   Serial.println(h + " " + c);
  {
   leds[23].setRGB(255, 47, 0); 
   leds[24].setRGB(255, 42, 0);
    FastLED.show();

   leds[16]=CRGB(0,0,0); 
   leds[17]=CRGB(0,0,0); 
   leds[18]=CRGB(59, 44, 36); 
   leds[19]=CRGB(59, 44, 36);
   leds[20]=CRGB(59, 44, 36); 
   leds[21]=CRGB(59, 44, 36);
   leds[22]=CRGB(59, 44, 36);
   FastLED.show();
   delay(50);
   leds[16]=CRGB(255, 255, 255); 
   leds[17]=CRGB(255, 255, 255); 
   leds[18]=CRGB(255, 255, 255); 
   leds[19]=CRGB(255, 255, 255);
   leds[20]=CRGB(255, 255, 255); 
   leds[21]=CRGB(255, 255, 255);
   leds[22]=CRGB(255, 255, 255);
   FastLED.show();
   delay(100);
   leds[16]=CRGB(0,0,0); 
   leds[17]=CRGB(0,0,0); 
   leds[18]=CRGB(59, 44, 36); 
   leds[19]=CRGB(59, 44, 36);
   leds[20]=CRGB(59, 44, 36); 
   leds[21]=CRGB(59, 44, 36);
   leds[22]=CRGB(59, 44, 36);
   FastLED.show();
   delay(500);
   leds[16]=CRGB(255, 255, 255); 
   leds[17]=CRGB(255, 255, 255); 
   leds[18]=CRGB(255, 255, 255); 
   leds[19]=CRGB(255, 255, 255);
   leds[20]=CRGB(255, 255, 255); 
   leds[21]=CRGB(255, 255, 255);
   leds[22]=CRGB(255, 255, 255);
   FastLED.show();
   delay(100);
   leds[16]=CRGB(0,0,0); 
   leds[17]=CRGB(0,0,0); 
   leds[18]=CRGB(59, 44, 36); 
   leds[19]=CRGB(59, 44, 36);
   leds[20]=CRGB(59, 44, 36); 
   leds[21]=CRGB(59, 44, 36);
   leds[22]=CRGB(59, 44, 36);
   FastLED.show();
   delay(50);
   leds[16]=CRGB(255, 255, 255); 
   leds[17]=CRGB(255, 255, 255); 
   leds[18]=CRGB(255, 255, 255); 
   leds[19]=CRGB(255, 255, 255);
   leds[20]=CRGB(255, 255, 255); 
   leds[21]=CRGB(255, 255, 255);
   leds[22]=CRGB(255, 255, 255);
   FastLED.show();
   delay(100);
   leds[16]=CRGB(0,0,0); 
   leds[17]=CRGB(0,0,0); 
   leds[18]=CRGB(59, 44, 36); 
   leds[19]=CRGB(59, 44, 36);
   leds[20]=CRGB(59, 44, 36); 
   leds[21]=CRGB(59, 44, 36);
   leds[22]=CRGB(59, 44, 36);
   FastLED.show();
   delay(500);
   leds[16]=CRGB(255, 255, 255); 
   leds[17]=CRGB(255, 255, 255); 
   leds[18]=CRGB(255, 255, 255); 
   leds[19]=CRGB(255, 255, 255);
   leds[20]=CRGB(255, 255, 255); 
   leds[21]=CRGB(255, 255, 255);
   leds[22]=CRGB(255, 255, 255);
   FastLED.show();
   delay(100);
   leds[16]=CRGB(0,0,0); 
   leds[17]=CRGB(0,0,0); 
   leds[18]=CRGB(59, 44, 36); 
   leds[19]=CRGB(59, 44, 36);
   leds[20]=CRGB(59, 44, 36); 
   leds[21]=CRGB(59, 44, 36);
   leds[22]=CRGB(59, 44, 36);
   FastLED.show();
   delay(500);
   leds[16]=CRGB(255, 255, 255); 
   leds[17]=CRGB(255, 255, 255); 
   leds[18]=CRGB(255, 255, 255); 
   leds[19]=CRGB(255, 255, 255);
   leds[20]=CRGB(255, 255, 255); 
   leds[21]=CRGB(255, 255, 255);
   leds[22]=CRGB(255, 255, 255);
   FastLED.show();
   delay(100);
   leds[16]=CRGB(0,0,0); 
   leds[17]=CRGB(0,0,0); 
   leds[18]=CRGB(59, 44, 36); 
   leds[19]=CRGB(59, 44, 36);
   leds[20]=CRGB(59, 44, 36); 
   leds[21]=CRGB(59, 44, 36);
   leds[22]=CRGB(59, 44, 36);
   FastLED.show();
   delay(50);
   leds[16]=CRGB(255, 255, 255); 
   leds[17]=CRGB(255, 255, 255); 
   leds[18]=CRGB(255, 255, 255); 
   leds[19]=CRGB(255, 255, 255);
   leds[20]=CRGB(255, 255, 255); 
   leds[21]=CRGB(255, 255, 255);
   leds[22]=CRGB(255, 255, 255);
   FastLED.show();
   delay(100);
   leds[16]=CRGB(0,0,0); 
   leds[17]=CRGB(0,0,0); 
   leds[18]=CRGB(59, 44, 36); 
   leds[19]=CRGB(59, 44, 36);
   leds[20]=CRGB(59, 44, 36); 
   leds[21]=CRGB(59, 44, 36);
   leds[22]=CRGB(59, 44, 36);
   FastLED.show();
   delay(500);
   leds[16]=CRGB(255, 255, 255); 
   leds[17]=CRGB(255, 255, 255); 
   leds[18]=CRGB(255, 255, 255); 
   leds[19]=CRGB(255, 255, 255);
   leds[20]=CRGB(255, 255, 255); 
   leds[21]=CRGB(255, 255, 255);
   leds[22]=CRGB(255, 255, 255);
   FastLED.show();
   delay(100);
   leds[16]=CRGB(0,0,0); 
   leds[17]=CRGB(0,0,0); 
   leds[18]=CRGB(59, 44, 36); 
   leds[19]=CRGB(59, 44, 36);
   leds[20]=CRGB(59, 44, 36); 
   leds[21]=CRGB(59, 44, 36);
   leds[22]=CRGB(59, 44, 36);
   FastLED.show();
   delay(50);
   leds[16]=CRGB(255, 255, 255); 
   leds[17]=CRGB(255, 255, 255); 
   leds[18]=CRGB(255, 255, 255); 
   leds[19]=CRGB(255, 255, 255);
   leds[20]=CRGB(255, 255, 255); 
   leds[21]=CRGB(255, 255, 255);
   leds[22]=CRGB(255, 255, 255);
   FastLED.show();
   delay(100);
   leds[16]=CRGB(0,0,0); 
   leds[17]=CRGB(0,0,0); 
   leds[18]=CRGB(59, 44, 36); 
   leds[19]=CRGB(59, 44, 36);
   leds[20]=CRGB(59, 44, 36); 
   leds[21]=CRGB(59, 44, 36);
   leds[22]=CRGB(59, 44, 36);
   FastLED.show();
   delay(500);
   leds[16]=CRGB(255, 255, 255); 
   leds[17]=CRGB(255, 255, 255); 
   leds[18]=CRGB(255, 255, 255); 
   leds[19]=CRGB(255, 255, 255);
   leds[20]=CRGB(255, 255, 255); 
   leds[21]=CRGB(255, 255, 255);
   leds[22]=CRGB(255, 255, 255);
   FastLED.show();
   delay(100);
   leds[16]=CRGB(0,0,0); 
   leds[17]=CRGB(0,0,0); 
   leds[18]=CRGB(59, 44, 36); 
   leds[19]=CRGB(59, 44, 36);
   leds[20]=CRGB(59, 44, 36); 
   leds[21]=CRGB(59, 44, 36);
   leds[22]=CRGB(59, 44, 36);
   FastLED.show();
   delay(500);
   leds[16]=CRGB(255, 255, 255); 
   leds[17]=CRGB(255, 255, 255); 
   leds[18]=CRGB(255, 255, 255); 
   leds[19]=CRGB(255, 255, 255);
   leds[20]=CRGB(255, 255, 255); 
   leds[21]=CRGB(255, 255, 255);
   leds[22]=CRGB(255, 255, 255);
   FastLED.show();
   delay(100);
   leds[16]=CRGB(0,0,0); 
   leds[17]=CRGB(0,0,0); 
   leds[18]=CRGB(59, 44, 36); 
   leds[19]=CRGB(59, 44, 36);
   leds[20]=CRGB(59, 44, 36); 
   leds[21]=CRGB(59, 44, 36);
   leds[22]=CRGB(59, 44, 36);
   FastLED.show();
   delay(500);
   leds[16]=CRGB(255, 255, 255); 
   leds[17]=CRGB(255, 255, 255); 
   leds[18]=CRGB(255, 255, 255); 
   leds[19]=CRGB(255, 255, 255);
   leds[20]=CRGB(255, 255, 255); 
   leds[21]=CRGB(255, 255, 255);
   leds[22]=CRGB(255, 255, 255);
   FastLED.show();
   delay(100);
  }
  }

  /*------------------------Evening-------------------------*/
  if((h >= 17 && h < 19)){
    if((c == "clear sky" || c == "few clouds" || c == "haze"))
  
   Serial.println(h + " " + c);
  {
   leds[23]=CRGB(255, 47, 0); 
   leds[24]=CRGB(71, 42, 10);
   
   leds[16]=CRGB(66, 16, 65); 
   leds[17]=CRGB(66, 16, 65); 
   leds[18]=CRGB(245, 36, 5); 
   leds[19]=CRGB(245, 36, 5);
   leds[20]=CRGB(245, 36, 5); 
   leds[21]=CRGB(245, 36, 5);
   leds[22]=CRGB(245, 36, 5);
   FastLED.show();
   delay(5000);
  }
  if( c == "scattered clouds")
   Serial.println(h + " " + c);
  {
   leds[23]=CRGB(255, 47, 0); 
   leds[24]=CRGB(71, 42, 10);
   FastLED.show();
   
   leds[16]=CRGB(245, 36, 5); 
   leds[17]=CRGB(245, 36, 5); 
   leds[18]=CRGB(245, 36, 5); 
   leds[19]=CRGB(0, 89, 110);
   leds[20]=CRGB(245, 36, 5); 
   leds[21]=CRGB(245, 36, 5);
   leds[22]=CRGB(245, 36, 5);
   FastLED.show();
   delay(5000);
  }
  if((c == "broken clouds" || c == "overcast clouds"))
   Serial.println(h + " " + c);
  {
   leds[23]=CRGB(255, 47, 0); 
   leds[24]=CRGB(71, 42, 10);
   
   leds[16]=CRGB(245, 36, 5); 
   leds[17]=CRGB(245, 36, 5); 
   leds[18]=CRGB(66, 16, 65); 
   leds[19]=CRGB(66, 16, 65);
   leds[20]=CRGB(66, 16, 65); 
   leds[21]=CRGB(66, 16, 65);
   leds[22]=CRGB(66, 16, 65);
   FastLED.show(); 
   delay(5000);
  }
  if( (c == "light rain" || c == "moderate rain" || c == "heavy intensity rain"))
   Serial.println(h + " " + c);
  {
   leds[23]=CRGB(255, 47, 0); 
   leds[24]=CRGB(71, 42, 10); 
   FastLED.show(); 

   leds[16]=CRGB(46, 15, 0); 
   leds[17]=CRGB(46, 15, 0); 
   leds[18]=CRGB(46, 15, 0); 
   leds[19]=CRGB(46, 15, 0);
   leds[20]=CRGB(46, 15, 0); 
   leds[21]=CRGB(46, 15, 0);
   leds[22]=CRGB(46, 15, 0);
   FastLED.show();
   delay(50);
   leds[16]=CRGB(255, 255, 255); 
   leds[17]=CRGB(255, 255, 255); 
   leds[18]=CRGB(255, 255, 255); 
   leds[19]=CRGB(255, 255, 255);
   leds[20]=CRGB(255, 255, 255); 
   leds[21]=CRGB(255, 255, 255);
   leds[22]=CRGB(255, 255, 255);
   FastLED.show();
   delay(100);
   leds[16]=CRGB(46, 15, 0); 
   leds[17]=CRGB(46, 15, 0); 
   leds[18]=CRGB(46, 15, 0); 
   leds[19]=CRGB(46, 15, 0);
   leds[20]=CRGB(46, 15, 0); 
   leds[21]=CRGB(46, 15, 0);
   leds[22]=CRGB(46, 15, 0);
   FastLED.show();
   delay(500);
   leds[16]=CRGB(255, 255, 255); 
   leds[17]=CRGB(255, 255, 255); 
   leds[18]=CRGB(255, 255, 255); 
   leds[19]=CRGB(255, 255, 255);
   leds[20]=CRGB(255, 255, 255); 
   leds[21]=CRGB(255, 255, 255);
   leds[22]=CRGB(255, 255, 255);
   FastLED.show();
   delay(100);
   leds[16]=CRGB(46, 15, 0); 
   leds[17]=CRGB(46, 15, 0); 
   leds[18]=CRGB(46, 15, 0); 
   leds[19]=CRGB(46, 15, 0);
   leds[20]=CRGB(46, 15, 0); 
   leds[21]=CRGB(46, 15, 0);
   leds[22]=CRGB(46, 15, 0);
   FastLED.show();
   delay(50);
   leds[16]=CRGB(255, 255, 255); 
   leds[17]=CRGB(255, 255, 255); 
   leds[18]=CRGB(255, 255, 255); 
   leds[19]=CRGB(255, 255, 255);
   leds[20]=CRGB(255, 255, 255); 
   leds[21]=CRGB(255, 255, 255);
   leds[22]=CRGB(255, 255, 255);
   FastLED.show();
   delay(100);
   leds[16]=CRGB(46, 15, 0); 
   leds[17]=CRGB(46, 15, 0); 
   leds[18]=CRGB(46, 15, 0); 
   leds[19]=CRGB(46, 15, 0);
   leds[20]=CRGB(46, 15, 0); 
   leds[21]=CRGB(46, 15, 0);
   leds[22]=CRGB(46, 15, 0);
   FastLED.show();
   delay(500);
   leds[16]=CRGB(255, 255, 255); 
   leds[17]=CRGB(255, 255, 255); 
   leds[18]=CRGB(255, 255, 255); 
   leds[19]=CRGB(255, 255, 255);
   leds[20]=CRGB(255, 255, 255); 
   leds[21]=CRGB(255, 255, 255);
   leds[22]=CRGB(255, 255, 255);
   FastLED.show();
   delay(100);
   leds[16]=CRGB(46, 15, 0); 
   leds[17]=CRGB(46, 15, 0); 
   leds[18]=CRGB(46, 15, 0); 
   leds[19]=CRGB(46, 15, 0);
   leds[20]=CRGB(46, 15, 0); 
   leds[21]=CRGB(46, 15, 0);
   leds[22]=CRGB(46, 15, 0);
   FastLED.show();
   delay(500);
   leds[16]=CRGB(255, 255, 255); 
   leds[17]=CRGB(255, 255, 255); 
   leds[18]=CRGB(255, 255, 255); 
   leds[19]=CRGB(255, 255, 255);
   leds[20]=CRGB(255, 255, 255); 
   leds[21]=CRGB(255, 255, 255);
   leds[22]=CRGB(255, 255, 255);
   FastLED.show();
   delay(100);
   leds[16]=CRGB(46, 15, 0); 
   leds[17]=CRGB(46, 15, 0); 
   leds[18]=CRGB(46, 15, 0); 
   leds[19]=CRGB(46, 15, 0);
   leds[20]=CRGB(46, 15, 0); 
   leds[21]=CRGB(46, 15, 0);
   leds[22]=CRGB(46, 15, 0);
   FastLED.show();
   delay(50);
   leds[16]=CRGB(255, 255, 255); 
   leds[17]=CRGB(255, 255, 255); 
   leds[18]=CRGB(255, 255, 255); 
   leds[19]=CRGB(255, 255, 255);
   leds[20]=CRGB(255, 255, 255); 
   leds[21]=CRGB(255, 255, 255);
   leds[22]=CRGB(255, 255, 255);
   FastLED.show();
   delay(100);
   leds[16]=CRGB(46, 15, 0); 
   leds[17]=CRGB(46, 15, 0); 
   leds[18]=CRGB(46, 15, 0); 
   leds[19]=CRGB(46, 15, 0);
   leds[20]=CRGB(46, 15, 0); 
   leds[21]=CRGB(46, 15, 0);
   leds[22]=CRGB(46, 15, 0);
   FastLED.show();
   delay(500);
   leds[16]=CRGB(255, 255, 255); 
   leds[17]=CRGB(255, 255, 255); 
   leds[18]=CRGB(255, 255, 255); 
   leds[19]=CRGB(255, 255, 255);
   leds[20]=CRGB(255, 255, 255); 
   leds[21]=CRGB(255, 255, 255);
   leds[22]=CRGB(255, 255, 255);
   FastLED.show();
   delay(100);
   leds[16]=CRGB(46, 15, 0); 
   leds[17]=CRGB(46, 15, 0); 
   leds[18]=CRGB(46, 15, 0); 
   leds[19]=CRGB(46, 15, 0);
   leds[20]=CRGB(46, 15, 0); 
   leds[21]=CRGB(46, 15, 0);
   leds[22]=CRGB(46, 15, 0);
   FastLED.show();
   delay(50);
   leds[16]=CRGB(255, 255, 255); 
   leds[17]=CRGB(255, 255, 255); 
   leds[18]=CRGB(255, 255, 255); 
   leds[19]=CRGB(255, 255, 255);
   leds[20]=CRGB(255, 255, 255); 
   leds[21]=CRGB(255, 255, 255);
   leds[22]=CRGB(255, 255, 255);
   FastLED.show();
   delay(100);
   leds[16]=CRGB(46, 15, 0); 
   leds[17]=CRGB(46, 15, 0); 
   leds[18]=CRGB(46, 15, 0); 
   leds[19]=CRGB(46, 15, 0);
   leds[20]=CRGB(46, 15, 0); 
   leds[21]=CRGB(46, 15, 0);
   leds[22]=CRGB(46, 15, 0);
   FastLED.show();
   delay(500);
   leds[16]=CRGB(255, 255, 255); 
   leds[17]=CRGB(255, 255, 255); 
   leds[18]=CRGB(255, 255, 255); 
   leds[19]=CRGB(255, 255, 255);
   leds[20]=CRGB(255, 255, 255); 
   leds[21]=CRGB(255, 255, 255);
   leds[22]=CRGB(255, 255, 255);
   FastLED.show();
   delay(100);
   leds[16]=CRGB(46, 15, 0); 
   leds[17]=CRGB(46, 15, 0); 
   leds[18]=CRGB(46, 15, 0); 
   leds[19]=CRGB(46, 15, 0);
   leds[20]=CRGB(46, 15, 0); 
   leds[21]=CRGB(46, 15, 0);
   leds[22]=CRGB(46, 15, 0);
   FastLED.show();
   delay(500);
   leds[16]=CRGB(255, 255, 255); 
   leds[17]=CRGB(255, 255, 255); 
   leds[18]=CRGB(255, 255, 255); 
   leds[19]=CRGB(255, 255, 255);
   leds[20]=CRGB(255, 255, 255); 
   leds[21]=CRGB(255, 255, 255);
   leds[22]=CRGB(255, 255, 255);
   FastLED.show();
   delay(100);
   leds[16]=CRGB(46, 15, 0); 
   leds[17]=CRGB(46, 15, 0); 
   leds[18]=CRGB(46, 15, 0); 
   leds[19]=CRGB(46, 15, 0);
   leds[20]=CRGB(46, 15, 0); 
   leds[21]=CRGB(46, 15, 0);
   leds[22]=CRGB(46, 15, 0);
   FastLED.show();
   delay(500);
   leds[16]=CRGB(255, 255, 255); 
   leds[17]=CRGB(255, 255, 255); 
   leds[18]=CRGB(255, 255, 255); 
   leds[19]=CRGB(255, 255, 255);
   leds[20]=CRGB(255, 255, 255); 
   leds[21]=CRGB(255, 255, 255);
   leds[22]=CRGB(255, 255, 255);
   FastLED.show();
   delay(100);
   leds[16]=CRGB(46, 15, 0); 
   leds[17]=CRGB(46, 15, 0); 
   leds[18]=CRGB(46, 15, 0); 
   leds[19]=CRGB(46, 15, 0);
   leds[20]=CRGB(46, 15, 0); 
   leds[21]=CRGB(46, 15, 0);
   leds[22]=CRGB(46, 15, 0);
   FastLED.show();
   delay(200);
  }
  }

  /*------------------------Night-------------------------*/
  if((h >= 19 && h < 5)){ 
  if((c == "clear sky" || c == "few clouds" || c == "haze"))
   Serial.println(h + " " + c);
  {
   leds[23]=CRGB(232, 224, 216); 
   leds[24]=CRGB(61, 51, 48); 
   
   leds[16]=CRGB(0,0,0); 
   leds[17]=CRGB(0,0,0); 
   leds[18]=CRGB(20, 27, 31); 
   leds[19]=CRGB(20, 27, 31);
   leds[20]=CRGB(40, 51, 56); 
   leds[21]=CRGB(40, 51, 56);
   leds[22]=CRGB(40, 51, 56);
   FastLED.show();
   delay(5000);
  }
  if(c == "scattered clouds")
   Serial.println(h + " " + c);
  {
   leds[23]=CRGB(232, 224, 216); 
   leds[24]=CRGB(61, 51, 48); 
   
   leds[16]=CRGB(0,0,0); 
   leds[17]=CRGB(0,0,0); 
   leds[18]=CRGB(16, 46, 71); 
   leds[19]=CRGB(16, 46, 71);
   leds[20]=CRGB(16, 46, 71); 
   leds[21]=CRGB(40, 51, 56);
   leds[22]=CRGB(40, 51, 56);
   FastLED.show();
   delay(5000);
  }
  if((c == "broken clouds" || c == "overcast clouds"))
   Serial.println(h + " " + c);
  {
   leds[23]=CRGB(232, 224, 216); 
   leds[24]=CRGB(61, 51, 48); 
   leds[16]=CRGB(0,0,0); 
   leds[17]=CRGB(0,0,0); 
   leds[18]=CRGB(4, 12, 18); 
   leds[19]=CRGB(4, 12, 18);
   leds[20]=CRGB(4, 12, 18); 
   leds[21]=CRGB(4, 12, 18);
   leds[22]=CRGB(40, 51, 56);
   FastLED.show();
   delay(5000);
  }
  if((c == "light rain" || c == "moderate rain" || c == "heavy intensity rain"))
   Serial.println(h + " " + c);
  {
   leds[23]=CRGB(232, 224, 216); 
   leds[24]=CRGB(61, 51, 48); 
    FastLED.show();

   leds[16]=CRGB(0,0,0); 
   leds[17]=CRGB(0,0,0); 
   leds[18]=CRGB(0,0,0); 
   leds[19]=CRGB(0, 17, 20);
   leds[20]=CRGB(0, 17, 20); 
   leds[21]=CRGB(0, 17, 20);
   leds[22]=CRGB(0, 17, 20);
   FastLED.show();
   delay(50);
   leds[16]=CRGB(255, 255, 255); 
   leds[17]=CRGB(255, 255, 255); 
   leds[18]=CRGB(255, 255, 255); 
   leds[19]=CRGB(255, 255, 255);
   leds[20]=CRGB(255, 255, 255); 
   leds[21]=CRGB(255, 255, 255);
   leds[22]=CRGB(255, 255, 255);
   FastLED.show();
   delay(100);
   leds[16]=CRGB(0,0,0); 
   leds[17]=CRGB(0,0,0); 
   leds[18]=CRGB(0,0,0); 
   leds[19]=CRGB(0, 17, 20);
   leds[20]=CRGB(0, 17, 20); 
   leds[21]=CRGB(0, 17, 20);
   leds[22]=CRGB(0, 17, 20);
   FastLED.show();
   delay(500);
   leds[16]=CRGB(255, 255, 255); 
   leds[17]=CRGB(255, 255, 255); 
   leds[18]=CRGB(255, 255, 255); 
   leds[19]=CRGB(255, 255, 255);
   leds[20]=CRGB(255, 255, 255); 
   leds[21]=CRGB(255, 255, 255);
   leds[22]=CRGB(255, 255, 255);
   FastLED.show();
   delay(100);
   leds[16]=CRGB(0,0,0); 
   leds[17]=CRGB(0,0,0); 
   leds[18]=CRGB(0,0,0); 
   leds[19]=CRGB(0, 17, 20);
   leds[20]=CRGB(0, 17, 20); 
   leds[21]=CRGB(0, 17, 20);
   leds[22]=CRGB(0, 17, 20);
   FastLED.show();
   delay(50);
   leds[16]=CRGB(255, 255, 255); 
   leds[17]=CRGB(255, 255, 255); 
   leds[18]=CRGB(255, 255, 255); 
   leds[19]=CRGB(255, 255, 255);
   leds[20]=CRGB(255, 255, 255); 
   leds[21]=CRGB(255, 255, 255);
   leds[22]=CRGB(255, 255, 255);
   FastLED.show();
   delay(100);
   leds[16]=CRGB(0,0,0); 
   leds[17]=CRGB(0,0,0); 
   leds[18]=CRGB(0,0,0); 
   leds[19]=CRGB(0, 17, 20);
   leds[20]=CRGB(0, 17, 20); 
   leds[21]=CRGB(0, 17, 20);
   leds[22]=CRGB(0, 17, 20);
   FastLED.show();
   delay(500);
   leds[16]=CRGB(255, 255, 255); 
   leds[17]=CRGB(255, 255, 255); 
   leds[18]=CRGB(255, 255, 255); 
   leds[19]=CRGB(255, 255, 255);
   leds[20]=CRGB(255, 255, 255); 
   leds[21]=CRGB(255, 255, 255);
   leds[22]=CRGB(255, 255, 255);
   FastLED.show();
   delay(100);
   leds[16]=CRGB(0,0,0); 
   leds[17]=CRGB(0,0,0); 
   leds[18]=CRGB(0,0,0); 
   leds[19]=CRGB(0, 17, 20);
   leds[20]=CRGB(0, 17, 20); 
   leds[21]=CRGB(0, 17, 20);
   leds[22]=CRGB(0, 17, 20);
   FastLED.show();
   delay(500);
   leds[16]=CRGB(255, 255, 255); 
   leds[17]=CRGB(255, 255, 255); 
   leds[18]=CRGB(255, 255, 255); 
   leds[19]=CRGB(255, 255, 255);
   leds[20]=CRGB(255, 255, 255); 
   leds[21]=CRGB(255, 255, 255);
   leds[22]=CRGB(255, 255, 255);
   FastLED.show();
   delay(100);
   leds[16]=CRGB(0,0,0); 
   leds[17]=CRGB(0,0,0); 
   leds[18]=CRGB(0,0,0); 
   leds[19]=CRGB(0, 17, 20);
   leds[20]=CRGB(0, 17, 20); 
   leds[21]=CRGB(0, 17, 20);
   leds[22]=CRGB(0, 17, 20);
   FastLED.show();
   delay(50);
   leds[16]=CRGB(255, 255, 255); 
   leds[17]=CRGB(255, 255, 255); 
   leds[18]=CRGB(255, 255, 255); 
   leds[19]=CRGB(255, 255, 255);
   leds[20]=CRGB(255, 255, 255); 
   leds[21]=CRGB(255, 255, 255);
   leds[22]=CRGB(255, 255, 255);
   FastLED.show();
   delay(100);
   leds[16]=CRGB(0,0,0); 
   leds[17]=CRGB(0,0,0); 
   leds[18]=CRGB(0,0,0); 
   leds[19]=CRGB(0, 17, 20);
   leds[20]=CRGB(0, 17, 20); 
   leds[21]=CRGB(0, 17, 20);
   leds[22]=CRGB(0, 17, 20);
   FastLED.show();
   delay(500);
   leds[16]=CRGB(255, 255, 255); 
   leds[17]=CRGB(255, 255, 255); 
   leds[18]=CRGB(255, 255, 255); 
   leds[19]=CRGB(255, 255, 255);
   leds[20]=CRGB(255, 255, 255); 
   leds[21]=CRGB(255, 255, 255);
   leds[22]=CRGB(255, 255, 255);
   FastLED.show();
   delay(100);
   leds[16]=CRGB(0,0,0); 
   leds[17]=CRGB(0,0,0); 
   leds[18]=CRGB(0,0,0); 
   leds[19]=CRGB(0, 17, 20);
   leds[20]=CRGB(0, 17, 20); 
   leds[21]=CRGB(0, 17, 20);
   leds[22]=CRGB(0, 17, 20);
   FastLED.show();
   delay(50);
   leds[16]=CRGB(255, 255, 255); 
   leds[17]=CRGB(255, 255, 255); 
   leds[18]=CRGB(255, 255, 255); 
   leds[19]=CRGB(255, 255, 255);
   leds[20]=CRGB(255, 255, 255); 
   leds[21]=CRGB(255, 255, 255);
   leds[22]=CRGB(255, 255, 255);
   FastLED.show();
   delay(100);
   leds[16]=CRGB(0,0,0); 
   leds[17]=CRGB(0,0,0); 
   leds[18]=CRGB(0,0,0); 
   leds[19]=CRGB(0, 17, 20);
   leds[20]=CRGB(0, 17, 20); 
   leds[21]=CRGB(0, 17, 20);
   leds[22]=CRGB(0, 17, 20);
   FastLED.show();
   delay(500);
   leds[16]=CRGB(255, 255, 255); 
   leds[17]=CRGB(255, 255, 255); 
   leds[18]=CRGB(255, 255, 255); 
   leds[19]=CRGB(255, 255, 255);
   leds[20]=CRGB(255, 255, 255); 
   leds[21]=CRGB(255, 255, 255);
   leds[22]=CRGB(255, 255, 255);
   FastLED.show();
   delay(100);
   leds[16]=CRGB(0,0,0); 
   leds[17]=CRGB(0,0,0); 
   leds[18]=CRGB(0,0,0); 
   leds[19]=CRGB(0, 17, 20);
   leds[20]=CRGB(0, 17, 20); 
   leds[21]=CRGB(0, 17, 20);
   leds[22]=CRGB(0, 17, 20);
   FastLED.show();
   delay(500);
   leds[16]=CRGB(255, 255, 255); 
   leds[17]=CRGB(255, 255, 255); 
   leds[18]=CRGB(255, 255, 255); 
   leds[19]=CRGB(255, 255, 255);
   leds[20]=CRGB(255, 255, 255); 
   leds[21]=CRGB(255, 255, 255);
   leds[22]=CRGB(255, 255, 255);
   FastLED.show();
   delay(100);
   leds[16]=CRGB(0,0,0); 
   leds[17]=CRGB(0,0,0); 
   leds[18]=CRGB(0,0,0); 
   leds[19]=CRGB(0, 17, 20);
   leds[20]=CRGB(0, 17, 20); 
   leds[21]=CRGB(0, 17, 20);
   leds[22]=CRGB(0, 17, 20);
   FastLED.show();
   delay(500);
   leds[22]=CRGB(255, 255, 255);
   leds[16]=CRGB(255, 255, 255); 
   leds[17]=CRGB(255, 255, 255); 
   leds[18]=CRGB(255, 255, 255); 
   leds[19]=CRGB(255, 255, 255);
   leds[20]=CRGB(255, 255, 255); 
   leds[21]=CRGB(255, 255, 255);
   FastLED.show();
   delay(100);
  }
  }

  /*-----------------------Temperature-------------------------*/
  if(temp<20)
   Serial.println(temp);
  {
    leds[0]=CRGB(0,255,255);
    leds[1]=CRGB(0,255,255);
    leds[2]=CRGB(0,255,255);
    leds[3]=CRGB(0,255,255);
    leds[4]=CRGB(0,255,255);
    leds[5]=CRGB(0,255,255);
    leds[6]=CRGB(0,255,255);
    leds[7]=CRGB(0,255,255);
    leds[8]=CRGB(0,255,255);
    leds[9]=CRGB(0,255,255);
    leds[10]=CRGB(0,255,255);
    leds[11]=CRGB(0,255,255);
    leds[12]=CRGB(0,255,255);
    leds[13]=CRGB(0,255,255);
    leds[14]=CRGB(0,255,255);
    leds[15]=CRGB(0,255,255);
    FastLED.show();
  }
  if(temp>=20 && temp<30)
   Serial.println(temp);
  {
    leds[0]=CRGB(255, 251, 0);
    leds[1]=CRGB(255, 251, 0);
    leds[2]=CRGB(255, 251, 0);
    leds[3]=CRGB(255, 251, 0);
    leds[4]=CRGB(255, 251, 0);
    leds[5]=CRGB(255, 251, 0);
    leds[6]=CRGB(255, 251, 0);
    leds[7]=CRGB(255, 251, 0);
    leds[8]=CRGB(255, 251, 0);
    leds[9]=CRGB(255, 251, 0);
    leds[10]=CRGB(255, 251, 0);
    leds[11]=CRGB(255, 251, 0);
    leds[12]=CRGB(255, 251, 0);
    leds[13]=CRGB(255, 251, 0);
    leds[14]=CRGB(255, 251, 0);
    leds[15]=CRGB(255, 251, 0);
    FastLED.show();
  }
  if(temp>=30 && temp<40)
   Serial.println(temp);
  {
    leds[0]=CRGB(255, 60, 0);
    leds[1]=CRGB(255, 60, 0);
    leds[2]=CRGB(255, 60, 0);
    leds[3]=CRGB(255, 60, 0);
    leds[4]=CRGB(255, 60, 0);
    leds[5]=CRGB(255, 60, 0);
    leds[6]=CRGB(255, 60, 0);
    leds[7]=CRGB(255, 60, 0);
    leds[8]=CRGB(255, 60, 0);
    leds[9]=CRGB(255, 60, 0);
    leds[10]=CRGB(255, 60, 0);
    leds[11]=CRGB(255, 60, 0);
    leds[12]=CRGB(255, 60, 0);
    leds[13]=CRGB(255, 60, 0);
    leds[14]=CRGB(255, 60, 0);
    leds[15]=CRGB(255, 60, 0);
    FastLED.show();
  }
  if(temp>=40 && temp<50)
   Serial.println(temp);
  {
    leds[0]=CRGB(255, 0, 0);
    leds[1]=CRGB(255, 0, 0);
    leds[2]=CRGB(255, 0, 0);
    leds[3]=CRGB(255, 0, 0);
    leds[4]=CRGB(255, 0, 0);
    leds[5]=CRGB(255, 0, 0);
    leds[6]=CRGB(255, 0, 0);
    leds[7]=CRGB(255, 0, 0);
    leds[8]=CRGB(255, 0, 0);
    leds[9]=CRGB(255, 0, 0);
    leds[10]=CRGB(255, 0, 0);
    leds[11]=CRGB(255, 0, 0);
    leds[12]=CRGB(255, 0, 0);
    leds[13]=CRGB(255, 0, 0);
    leds[14]=CRGB(255, 0, 0);
    leds[15]=CRGB(255, 0, 0);
    
    FastLED.show();
  }
}
