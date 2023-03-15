/*
  Luka Sherman

  HCDE 539 Winter 2023
  University of Washington
  
  Inspired by project by Emmanuel Odunlade http://randomnerdtutorials.com
  */

#include <ArduinoJson.h>
#include <WiFiNINA.h>
#include <Adafruit_NeoPixel.h>

#include "secrets.h"

#define POWER_PIN A0
#define LED_PIN A1
#define LED_COUNT 20


/*
example of redacted secrets.h file from same directory: 
  #define SECRET_SSID "my network name";
  #define SECRET_PASS "my network password";
  #define SECRET_API_KEY "my openWeatherMap api key";
  #define SECRET_API_BACKUP_KEY "my backup openWeatherMap api key";
*/
const char SSID[] = SECRET_SSID;
const char SSID_PASSWORD[] = SECRET_PASS;
const String API_KEY = SECRET_API_KEY;

const char SERVER[] = "api.openweathermap.org";

WiFiClient client;
const unsigned long apiGetInterval = 600000;
unsigned long lastConnectionTime = 0;

bool oldState = LOW;
String jsonText;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ400);

uint32_t primaryColor = strip.Color(227, 194, 27);
uint32_t secondaryColor = strip.Color(255, 174, 0);
int brightness = 50;
int speed = 50;

const String PIN_CITIES[][3] = {
  { "", "", "" },
  { "", "", "" },
  { "SEATTLE,US", "047.6078", "-122.2913" },
  { "SAN FRANCISCO,US", "037.7563", "-122.4209" },
  { "ROCHESTER,US", "043.1300", "-077.6267" },
  { "BARRANQUILLA,CO", "010.9639", "-074.7964" },
  { "BOSTON,US", "042.3584", "-071.0598" },
  { "PARIS,FR", "048.8534", "0002.3488" },
  { "LAGOS,NG", "006.5833", "0003.7500" },
  { "SVALBARD,SJ", "078.1571", "0015.8643" },
  { "MUMBAI,IN", "019.0144", "0072.8479" },
  { "ADELAIDE,AU", "-34.9333", "0138.6000" },
  { "MCMURDO STATION,AQ", "-77.6554", "0168.2227" },
  { "HILO,US", "019.7300", "-155.0900" },
};
String currentCity = "";


/*
initalize input pins and clear led strip
*/
void setup() {

  Serial.begin(9600);

  for (int i = 2; i <= 13; ++i) {
    pinMode(i, INPUT_PULLUP);
  }
  pinMode(POWER_PIN, INPUT);

  strip.begin();
  strip.show();
}


/*
if power button pressed down, connect to wifi, send httprequest to api, and color led strip. If powered off, clear strip and disconnect 
*/
void loop() {

  bool newState = digitalRead(POWER_PIN);
  delay(30);
  bool newerState = digitalRead(POWER_PIN);

  if (newState == HIGH && newerState == HIGH) {

    if (WiFi.status() != WL_CONNECTED) {
      connectWiFi();
    }

    if (isNewRotaryCity() || (millis() - lastConnectionTime) >= apiGetInterval) {
      Serial.println("Has been " + String(millis() - lastConnectionTime) + "ms since last httpRequest compared to " + String(apiGetInterval) + "ms interval");
      lastConnectionTime = millis();
      makehttpRequest();
      setValuesFromJson();
    }

    colorLedStrip();
  }

  else if (newState == LOW && newerState == LOW && oldState == HIGH) {
    strip.clear();
    strip.show();

    if (WiFi.status() == WL_CONNECTED) {
      client.stop();
      Serial.println("client disconnected");
      delay(1000);
      Serial.println(String(WiFi.status()));
    }
  }

  oldState = newState;
}


/*
connect to wifi with credentials defined in secrets.h
*/
void connectWiFi() {

  WiFi.begin(SSID, SSID_PASSWORD);

  Serial.println("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected to network SSID: " + String(WiFi.SSID()));
  Serial.println("wifi shield IP Address: " + String(WiFi.localIP()));
  Serial.println("received signal strength (RSSI): " + String(WiFi.RSSI()) + " dBm\n");
}


/*
sets value of currentCity based on rotary switch selector position
*/
bool isNewRotaryCity() {
  for (int i = 2; i <= 13; ++i) {
    if (digitalRead(i) == LOW) {
      String newCity = PIN_CITIES[i][0];
      if (!currentCity.equals(newCity)) {
        Serial.println("new city: " + newCity);
        currentCity = newCity;
        return true;
      }
      return false;
    }
  }
  currentCity = PIN_CITIES[2][0];
  Serial.println("\nERROR: could not detect rotary pin, will use previous value of " + String(currentCity) + "\n");
  return false;
}


/*
updates jsonText with string returned from api server 
*/
void makehttpRequest() {

  boolean isJsonStarted = false;
  jsonText = "";

  if (client.connect(SERVER, 80)) {

    Serial.println("client connected, getting json...");
    client.println("GET /data/2.5/forecast?q=" + currentCity + "&APPID=" + API_KEY + "&mode=json&units=imperial&exclude=hourly,daily&cnt=2 HTTP/1.1");
    client.println("Host: " + String(SERVER));
    client.println("User-Agent: ArduinoWiFi/1.1");
    client.println("Connection: close");
    client.println();

    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println(">>> Client Timeout !");
      }
    }

    while (client.available()) {
      char c = client.read();

      if (c == '{') {
        isJsonStarted = true;
      }

      if (isJsonStarted == true) {
        jsonText += c;
      }
    }

  } else {
    Serial.println("connection failed");
  }

  client.stop();
}


/* 
sets speed, brightness, primaryColor, secondaryColor based on jsonText
*/
void setValuesFromJson() {

  Serial.println(jsonText);

  DynamicJsonDocument doc(1024);

  DeserializationError error = deserializeJson(doc, jsonText);
  if (error)
    return;

  float temp_feels_like = doc["list"][0]["main"]["feels_like"];     // in F
  String description = doc["list"][0]["weather"][0]["main"];        // example Clouds Clear Snow
  int clouds = doc["list"][0]["clouds"]["all"];                     // range 0 - 100
  float probPercip = doc["list"][0]["pop"];                         // 0.00 to 1.00
  float windSpeed = doc["list"][0]["wind"]["speed"];                // 0.00 to 1.00
  float snowFall = doc["list"][0]["snow"]["3h"];                    // inches
  float rainFall = doc["list"][0]["rain"]["3h"];                    // inches
  bool isDay = doc["list"][0]["sys"]["pod"] == "d" ? true : false;  // "n" or "d"

  Serial.println(temp_feels_like);
  Serial.println(description);
  Serial.println(clouds);
  Serial.println(probPercip);
  Serial.println(windSpeed);
  Serial.println(snowFall);
  Serial.println(rainFall);
  Serial.println(isDay);

  speed = round(200 / (windSpeed + .01));

  if (description.equals("Clouds")) {
    primaryColor = strip.Color(158, 158, 158);
    secondaryColor = strip.Color(196, 196, 196);
  } else if (description.equals("Snow")) {
    primaryColor = strip.Color(227, 194, 27);
    secondaryColor = strip.Color(1, 1, 122);
  } else if (description.equals("Rain")) {
    primaryColor = strip.Color(227, 194, 27);
    secondaryColor = strip.Color(1, 1, 122);
  }
  if (currentCity.equals("SVALBARD,SJ")) {
    primaryColor = strip.Color(168, 71, 163);
    secondaryColor = strip.Color(43, 26, 110);
  }

  if (isDay) {
    brightness = 150;
  } else {
    brightness = 30;
  }
}


/* 
send colors to LED strip based on global values of speed, brightness, primaryColor, secondaryColor 
*/
void colorLedStrip() {

  strip.setBrightness(brightness);

  for (int i = 0; i < strip.numPixels(); i++) {  // For each pixel in strip...
    strip.setPixelColor(i, primaryColor);        //  Set pixel's color (in RAM)
    strip.show();                                //  Update strip to match
    delay(speed);                                //  Pause for a moment
  }

  uint8_t targetColor[] = { 255, 192, 203 };
  strip.setPixelColor(10, 0, 255, 0);
  strip.show();
  uint8_t fadeValue;  // 0 = off, 255 = full brightness
  for (int pixelId = 0; pixelId < 20; pixelId++) {
    // start at zero brightness
    fadeValue = 0;
    // fade in
    while (fadeValue < 255) {
      strip.setPixelColor(pixelId, fadeValue * targetColor[0] / 255, fadeValue * targetColor[1] / 255, fadeValue * targetColor[2] / 255);
      fadeValue++;
      strip.show();
      delay(5);
    }
    // fade out
    while (fadeValue > 0) {
      strip.setPixelColor(pixelId, fadeValue * targetColor[0] / 255, fadeValue * targetColor[1] / 255, fadeValue * targetColor[2] / 255);
      fadeValue--;
      strip.show();
      delay(5);
    }
  }
}
