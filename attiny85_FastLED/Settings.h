// Software serial
/*#include <SoftwareSerial.h>
SoftwareSerial bus(3, 4); // RX, TX*/

// LEDs
#define NO_CLOCK_CORRECTION 1
#include <FastLED.h>

#define NUM_LEDS    100
#define BRIGHTNESS  255
#define LED_TYPE    WS2811
#define COLOR_ORDER BRG
CRGB leds[NUM_LEDS];
//CRGBPalette16 currentPalette;
//TBlendType    currentBlending;

// PIN
#define LED_PIN 2