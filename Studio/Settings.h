// Software serial
#include <SoftwareSerial.h>
//SoftwareSerial bus(9, 12);

// PIN
#define RCLK 10
#define SRCLK 7
#define DATA_OUT 11
#define DATA_IN 8

// Oled
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define rows 5
#define scrolling_time 10 // *10 [ms]

enum interfaces{
    home_inter = 1,
    settings_inter,
    light_inter,
    plug_inter,
    clock_inter,
    oled_inter
};
bool oled_update;
byte interface = settings_inter;
byte prev_interface = home_inter;

struct {
    String name;
    int* pointer;
    byte interface;
    byte min;
    byte max;
}element_list[10];
byte element_total;
byte element_selected;
bool selector = 1;
int temp;
bool title_list;

// RTC
#include <RTClib.h>
RTC_DS3231 rtc;
DateTime now;
byte hour;
byte minute;
unsigned long int update_clock;

// Registers and buttons
#define CLR(x,y) (x&=(~(1<<y)))
#define SET(x,y) (x|=(1<<y))
#define eeprom_offset 2
byte regState[2];
byte button;
byte button_pulse;

bool reg_update;
enum buttons{
    null,
    up,
    center,
    down,
    bD,
    bC,
    bB,
    bA
};
unsigned long int last_millis;
#define debounce 50
#define long_press 500

// EEPROM
#include <EEPROM.h>
int prova = 3;