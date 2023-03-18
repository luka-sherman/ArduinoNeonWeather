/*
  Luka Sherman

  HCDE 539 Winter 2023
  University of Washington
*/

#include <ArduinoJson.h>
#include <WiFiNINA.h>
#include <Adafruit_NeoPixel.h>

/*
example of redacted secrets.h file from same directory: 
  #define SECRET_SSID "my network name";
  #define SECRET_PASS "my network password";
  #define SECRET_API_KEY "my openWeatherMap api key";
  #define SECRET_API_BACKUP_KEY "my backup openWeatherMap api key";
*/
#include "secrets.h"

#define POWER_PIN A0
#define LED_PIN A1
#define LED_COUNT 20
#define SERVER "api.openweathermap.org"

const char SSID[] = SECRET_SSID;
const char SSID_PASSWORD[] = SECRET_PASS;
const String API_KEY = SECRET_API_KEY;

WiFiClient client;
const unsigned long apiGetInterval = 600000;
unsigned long lastConnectionTime = 0;

bool oldState = LOW;
String jsonText;
String aqiJsonText;

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
int currentCity = 0;


/*
initalize input pins and clear led strip
*/
void setup() {

  Serial.begin(9600);
  Serial.println("\n\n\nstarted");

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
      jsonText =  makehttpRequest("/data/2.5/forecast?q=" + PIN_CITIES[currentCity][0] + "&APPID=" + API_KEY + "&mode=json&units=imperial&exclude=hourly,daily&cnt=2 HTTP/1.1");
      aqiJsonText = makehttpRequest("/data/2.5/air_pollution?lat=" + PIN_CITIES[currentCity][1] + "&lon=" + PIN_CITIES[currentCity][2] + "&appid=" + API_KEY);
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
      if (currentCity!=i) {
        currentCity = i;
        Serial.println("new city: " + PIN_CITIES[i][0]);
        return true;
      }
      return false;
    }
  }
  currentCity = 2;
  Serial.println("\nERROR: could not detect rotary pin, will use previous value of " + String(PIN_CITIES[currentCity][0]) + "\n");
  return false;
}


/*
updates jsonText with string returned from api server 
*/
String makehttpRequest(String apiCall) {

  boolean isJsonStarted = false;
  String json = "";

  if (client.connect(SERVER, 80)) {

    Serial.println("client connected, getting json...");
    client.println("GET "+apiCall);
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
        json += c;
      }
    }

  } else {
    Serial.println("connection failed");
  }

  client.stop();
  Serial.println("returning: "+json);
  return json;
}


/* 
sets speed, brightness, primaryColor, secondaryColor based on jsonText
*/
void setValuesFromJson() {

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, jsonText);
  if (error)
    return;

  DynamicJsonDocument aqiDoc(1024);
  DeserializationError error2 = deserializeJson(aqiDoc, aqiJsonText);
  if (error2)
    return;

  float temp_feels_like = doc["list"][0]["main"]["feels_like"];     // in F
  String description = doc["list"][0]["weather"][0]["main"];        // example Clouds Clear Snow
  int clouds = doc["list"][0]["clouds"]["all"];                     // range 0 - 100
  float probPercip = doc["list"][0]["pop"];                         // 0.00 to 1.00
  float windSpeed = doc["list"][0]["wind"]["speed"];                // 0.00 to 1.00
  float snowFall = doc["list"][0]["snow"]["3h"];                    // inches
  float rainFall = doc["list"][0]["rain"]["3h"];                    // inches
  bool isDay = doc["list"][0]["sys"]["pod"] == "d" ? true : false;  // "n" or "d"

  int aqi = aqiDoc["list"][0]["main"]["aqi"];                        // 1 = Good, 2 = Fair, 3 = Moderate, 4 = Poor, 5 = Very Poor

  Serial.println("\ntemp_feels_like:" + String(temp_feels_like));
  Serial.println("description: \t" + description);
  Serial.println("clouds: \t" + String(clouds));
  Serial.println("probPercip: \t" + String(probPercip));
  Serial.println("windSpeed: \t" + String(windSpeed));
  Serial.println("snowFall: \t" + String(snowFall));
  Serial.println("rainFall: \t" + String(rainFall));
  Serial.println("isDay:\ \t\t" + String(isDay));
  Serial.println("aqi: \t\t" + String(aqi));

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
  if (PIN_CITIES[currentCity][0].equals("SVALBARD,SJ")) {
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
