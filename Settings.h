// LEDs
#include <FastLED.h>
#define LED_PIN     9
#define NUM_LEDS    100
#define BRIGHTNESS  255
#define LED_TYPE    WS2811
#define COLOR_ORDER BRG
CRGB leds[NUM_LEDS];
#define UPDATES_PER_SECOND 40
CRGBPalette16 currentPalette;
TBlendType    currentBlending;
byte animation = 0;
byte previousAnimation = -1;
bool animation_finished = 0;
int LEDbuffer[21];
enum modes {
    decrease = -1,
    increase = 1        
};