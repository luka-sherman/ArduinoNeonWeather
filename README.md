# ArduinoNeonWeather
 
# Table of contents
1. [Descripton](#description)
2. [Some paragraph](#circuit)
    1. [Sub paragraph](#subparagraph1)
3. [Another paragraph](#paragraph2)

## Description <a name="description"></a>
The concept behind this piece is to experience other locations around the world through light. Maybe it's a place you're homesick for, a place where a person you're close to is right now, or someplace you want to travel to. It could also be set to your current location, and help connect you to the outdoors, instead of creating a barrier between us and our environment by using artificial light in the home in lieu of what's happening outside. It seeks to connect us to places and people that are meaningful to us through sharing the experience of live weather qualities. 

A user switches it on and points the rotary dial to one of the labeled cities, and a neon-like LED strip will illuminate and animate based on what the weather is like right now at that location. 

Weather conditions correspond to different primary and secondary colors that are cycled down the strip (sunny is yellow, clouds are grey, rain is blue, snow is grey and blue). Shade and hue are used for scale, for instance if it is warmer it will be a deeper shade. The animation speed of the light cycle corresponds to the wind speed. If it is nighttime in that city, the brightness will be lowered. If the air quality is unsafe, the light will pulse red, a practical alert if you're currently in that city. Svalbard will always depict the northern lights (green, blue, purple). Every ten minutes it will check the latest weather and adjust if necessary. 
 
Twelve cities were picked on the basis of having representation of all continents, for having a variety of weather, and some for being significant places in my life. LED strands that are popularly used today have a remote that allows the resident to pick from different colors, and one could similarly dupe this by selecting a typically sunny city if you wanted a warm yellow/orange light, or a rainy city if you wanted a cooler blue light.

This two-week project was done for Andy Davidson's HCDE 539 Physical Computing and Prototyping course at the University of Washington. 

## Circuit <a name="circuit"></a>
The first paragraph text

Materials:

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

![bb](/images/circuit_breadboard.png)

![sc](/images/circuit_schematic.png)
    

### Sub paragraph <a name="subparagraph1"></a>
This is a sub paragraph, formatted in heading 3 style

## Another paragraph <a name="paragraph2"></a>
The second paragraph text