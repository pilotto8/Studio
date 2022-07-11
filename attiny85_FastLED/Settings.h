// Software serial
#include <SoftwareSerial.h>
SoftwareSerial bus(3, 4); // RX, TX

// LEDs
#include <FastLED.h>
#define NUM_LEDS    100
#define BRIGHTNESS  255
#define LED_TYPE    WS2811
#define COLOR_ORDER BRG
CRGB leds[NUM_LEDS];
byte UPDATES_PER_SECOND = 80;
//CRGBPalette16 currentPalette;
//TBlendType    currentBlending;

// PIN
#define LED_PIN 2