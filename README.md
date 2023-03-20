# Arduino Meterology
by Luka Sherman

---

 
## Table of contents
1. [Descripton](#description)
1. [Demo Video](#video)
1. [Circuit](#circuit)
1. [Construction](#construction)
1. [Gallery](#gallery)
1. [Software](#software)
1. [Attribution](#attribution)
1. [Cat](#cat)
1. [Obituary](#obituary)


## Description <a name="description"></a>
The concept behind this piece is to experience other locations around the world through light. Maybe it's a place you're homesick for, a place where a person you're close to is right now, or someplace you want to travel. It could also be set to your current location, and help connect you to the outdoors, instead of solely using artificial light in the home in lieu of what's happening outside, further reinforcing a barrier between us and our environment. 

*It seeks to connect us to places and people that are meaningful to us through sharing the experience of live meterological conditions.* 

A user switches it on and points the rotary dial to one of the labeled cities, and a neon-like LED strip will illuminate and animate based on what the weather is like right now at that location. 

Weather conditions correspond to different primary and secondary colors that are cycled down the strip (sunny is yellow, clouds are grey, rain is blue, snow is grey and blue). Shade and hue are used for scale, for instance if it is warmer it will be a deeper shade. The animation speed of the light cycle corresponds to the wind speed. If it is nighttime in that city, the brightness will be lowered. If the air quality is unsafe, the light will pulse red, a practical alert if you're currently in that city. Svalbard will always depict the northern lights (green, blue, purple). Every ten minutes it will check the latest weather and adjust if necessary. 
 
Twelve cities were picked on the basis of having representation of all continents, for having a variety of weather, and some for being significant places in my life. LED strands that are popularly used today have a remote that allows the resident to pick from different colors, and one could similarly dupe this by selecting a typically sunny city if you wanted a warm yellow/orange light, or a rainy city if you wanted a cooler blue light.

This two-week project was done for Andy Davidson's HCDE 539 Physical Computing and Prototyping course at the University of Washington. 

## Demo Video <a name="video"></a>

For display purposes, the piece is sitting on a pegboard shelf, and the LED loosely bent around hooks. The LED is flexible and can be arranged to fit any space. 

[![YouTube thumbnail](http://img.youtube.com/vi/f0F9UkN5C40/0.jpg)](http://www.youtube.com/watch?v=f0F9UkN5C40)

## Circuit <a name="circuit"></a>
### Materials: <a name="materials"></a>

* [Arduino Uno WiFi Rev2](https://store-usa.arduino.cc/products/arduino-uno-wifi-rev2?selectedStore=us)
* [Adafruit Perma-Proto Half-sized Breadboard PCB](https://www.adafruit.com/product/571)
* [Adafruit NeoPixel RGB Neon-like LED Flex Strip with Silicone Tube - 1 meter](https://www.adafruit.com/product/3869)
* [Power Supply 12V 1A](https://www.amazon.com/JOVNO-100-240V-Converter-Transformer-5-5x2-5mm/dp/B0875WMYCX/ref=sr_1_1?crid=2FWC7C0XGA9T8&keywords=12v+1a+power+supply+arduino&qid=1679010743&sprefix=12v+1apower+supply+arduino\%2Caps\%2C276&sr=8-1)
* [Push button Latching Switch with LED indicator](https://www.amazon.com/HiLetgo-Latching-Pushbutton-Switch-12V-24V/dp/B078SKTGF2/ref=sr_1_2?crid=PCOI9A778K1Q&keywords=hiletgo\%2Bpower\%2Bswitch&qid=1679010529\&sprefix=hiletgo\%2Bpower\%2Bswitc\%2Caps\%2C155&sr=8-2&th=1)
* [Rotary Selector Switch 1 Pole 12 Position 6mm](https://www.amazon.com/Position-Single-Selector-Rotary-Switch/dp/B074WMC9C8/ref=sr_1_1?crid=PQBEK9DA6BGQ&keywords=rotary+selector+switch+12&qid=1679010256&sprefix=rotary+selector+switch+1\%2Caps\%2C144&sr=8-1)
* [Rotary Control Knob Machined Aluminum Alloy 6mm](https://www.amazon.com/dp/B07FTLW1D2?ref=ppx_pop_mob_ap_share)
* Resistors: 220 ohms x2, 10k ohms x1
* Capacitor 1000uf
* Wire 22 gauge, solder, heat shrink tubing

### Breadboard Diagram <a name="breadboard"></a>
![bb](/images/circuit_breadboard.png)

### Schematic Diagram <a name="schematic"></a>
![sc](/images/circuit_schematic.png)

## Construction <a name="construction"></a>

I wanted the piece to be able to be displayed in a home, so formed several constraints around 
* **Power:** It had to be powered without USB connection to a computer, and only one power input for practicality. This model of NeoPixel already required 12V of power, more than the 5V the Arduino could supply, so I used the same power source to power the Arduino with the Vin port. This works because the Vin pin goes to the Arduino's 5V regulator, and the power supply is within the 7-12V unregulated range permitted. 
* **Robust:** I wanted everything to be firmly secured, and not delicate. The metal switches are heavy-duty and give a satisfying tactile feel. Everything is firmly held in place by an acrylic housing. The 1/4in acrylic is cut into two 5in x 7in sides, with holes cut in the top piece for the two switches, and holes cut into the bottom piece to screw in the Arduino and breadboard. 3in screws were used to connect the pieces, with plastic spacers between them to fill the empty space, and to lift the breadboard to the appropriate height in reference to the Arduino. All screws are affixed to washers on the back side of the acrylic. It is portable, and can be moved around without impact to the internals, while still being able to reach the components and adjust parts as needed.
* **Artistic:** I wanted it to have an industrial feel, and the high-quality thick acrylic allows the internals to be on display. I soldered all components and attempted to arrange them in a visually appealing way. I used an old-school manual label maker to label the rotary positions with cities (arranged from clockwise latitude coordinates, clockwise around the dial), and pointed the edges at the precise rotary marker to minimize the need for additional arrows. The LED strip was selected for its finished quality that matched the feel of the overall piece. The LED on the power button adds an additional level of user feedback. 

## Gallery <a name="gallery"></a>

Front Angle 
![front angle view image](/images/angle.jpeg)

Front
![front view image](/images/front.jpeg)

Back
![back view image](/images/back.jpeg)

Side
![side view image](/images/side.jpeg)

## Software Implementation <a name="software"></a>

1. The `loop()` function primarily listens for weather or not the power switch is pressed on. It reads the power switch, delays 30ms, and reads it again, to account for a consistent state and making sure that both values are equal. If the switch is now off but was last on during the last loop iteration, it will turn off the LED switch and disconnect from WiFi. If the power switch is on, it will proceed with the sequence:
2. If the WiFi is not already connected, it will connect. It uses the WiFiNINA library by Arduino, and uses the WiFi network name and password hard-coded in the secrets.h file that is located at the same directory as the .ino file.  
3. The script will then check to see which city the rotary selector switch is pointed to. It loops through pins 2-13, gets the corresponding String from a const array `PIN_CITIES` at the same position as the pin number, and updates the global String `currentCity` accordingly. 
4. If there is a new `currentCity` (i.e. it has changed since the loop's last iteration) or it has been over 10 minutes since the last API call, it will make an `httpRequest` to the API to get the current weather. Using the WiFiNINA connection already established, it will send a GET request to the OpenWeatherMap.org server, specifying the city, my personal OpenWeatherMap.org free API key, output mode (JSON), units (imperial), and the info that I don't need (daily and hourly forecasts). If the client is available, it will start saving the API's JSON response as soon as it detects the first ''\{'' to a global String variable `jsonText`. When the client is no longer available it will close the connection so it will be ready for the next call. 
5. The raw JSON is now coded into integers that will be used by the LED display. The JSON is deserialized into a `DynamicJsonDocument` from the ArduinoJson library. The values temperature (float signifying degrees Fahrenheit), description (String i.e. "Clouds" "Rain" "Clear" "Snow" etc), cloud level (int 0-100), probability of precipitation (float 0.00 to 1.00), wind speed (float 0.00 to 1.00), snow fall in the last three hours (float signifying inches), rain fall in the last three hours (float signifying inches), and if it is day or night (boolean). Those numbers are used to calculate variables that will be used for the LED display: `speed` (based on wind), `color` (based on current weather), and `brightness` (based on day vs nighttime).
6. The NeoPixel strip is illuminated with use of the `Adafruit_NeoPixel` library. The strip was instantiated as a global variable on upload, with parameters `LED_COUNT` (this strip has 60 LEDs, although they are grouped in threes, so it only recognizes 20 LED addresses), the `LED_PIN` it is connected to, and the pixel type flags (this strip uses two that are added together, `NEO_GRB` per the RGB bitstream and `NEO_KHZ400` per the 400 KHz pixels using WS2811 drivers). The script iterates through the 20 addresses, setting the color for each pixel, brightness, at the signified speed. 
7. The strip will continue with the same animation profile until either a new rotary position is selected, or if 10 minutes have elapsed since the last `httpRequest`. The free version of the API key can be used 60 times a minute, or 1,000,000 times a month. I have two API personal keys in case of capacity or for demo purposes. 
8. If the power switch is turned off, the LED strip will turn off after it finishes the current cycle (0-3 seconds) and it will disconnect from WiFi.

## Attribution <a name="attribution"></a>

### API:
* [OpenWeatherMap](https://openweathermap.org)  free API to get current weather conditions at a given location.

### Arduino Libraries:
* [Adafruit\_NeoPixel](https://adafruit.github.io/Adafruit_NeoPixel/html/class_adafruit___neo_pixel.html) to send commands to light the LED strip.
* [ArduinoJson](https://arduinojson.org) to parse the JSON API response.
* [WiFiNINA](https://docs.arduino.cc/tutorials/communication/wifi-nina-examples) to connect the Arduino to WiFi.

### Software:
* [Fritzing](http://fritzing.org) to draw the above circuit breadboard and schematic diagrams.

### Inspiration:
* [ESP8266 Weather Forecaster for Arduino and OpenWeatherMap API](https://randomnerdtutorials.com/esp8266-weather-forecaster/)

## Cat <a name="cat"></a>

![cat staring at led strip](/images/cat.jpeg)

Candid demonstration of an unintentional side effect - my cat loves watching the light cycle.

## Obituary <a name="obituary"></a>
† *This project is dedicated to the brave Arduino Uno WiFi Rev2 that I accidentally fried in the process, may she rest in smoldered pieces.*