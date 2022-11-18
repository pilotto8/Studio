// PIN
// Taken: 0,1,2,3,4,5,6,7,8,9,10,11,12,13   A4,A5
#define RCLK 10
#define SRCLK 7
#define DATA_OUT 11
#define DATA_IN 8
#define MW_DATA 12
#define INTERRUPT 2
#define LED_BUTTON 9
#define WAKE_SERIAL 13


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
//#define max_element_list 6

enum interfaces{
    sleep_inter,
    home_inter,
    settings_inter,
    light_inter,
    plug_inter,
    clock_inter,
    alarm_inter,
    moovement_inter
};
bool oled_update;
byte interface = home_inter;
byte prev_interface = settings_inter;

struct {
    String name;
    byte* pointer;
    byte interface;
    byte min;
    byte max;
}element_list[rows];
byte element_total;
byte element_selected;
bool selector = 1;
byte temp;
bool title_list;


// RTC
#include <RTClib.h>
#define max_timer 7
RTC_DS3231 rtc;
DateTime now;
byte hour;
byte minute;
unsigned long int update_clock;
bool update_clock_data;

struct {
    byte time_span;
    byte plugs;
} alarm_timer[max_timer];

byte num_timer;
byte temp_time_span;
byte temp_plugs;
byte prev_minute;

byte temp_minute;
byte temp_hour;
byte temp_day;
byte temp_month;
byte temp_year;

//const char days[][3] PROGMEM = {"Dom", "Lun", "Mar", "Mer", "Gio", "Ven", "Sab"};

// Registers and buttons
#define CLR(x,y) (x&=(~(1<<y)))
#define SET(x,y) (x|=(1<<y))
byte regState[2];
byte temp_regState[2];
byte button;
byte button_pulse;
bool led_button;

bool reg_update;
bool reg_freezed;
enum buttons{
    null,
    up,
    center,
    down
};
/*
    bD,
    bC,
    bB,
    bA
*/
unsigned long int last_millis;
unsigned long int last_millis_1;
#define debounce 50
#define long_press 500

// EEPROM
#include <EEPROM.h>
#define eeprom_offset 2

// Power plugs
byte temp_num_plug;

byte* plug_limit;
byte* plug_trigg;
byte* plug_reference;

byte plug_limit_0;
byte plug_limit_1;
byte plug_limit_2;
byte plug_limit_3;

byte plug_trigg_0;
byte plug_trigg_1;
byte plug_trigg_2;
byte plug_trigg_3;

byte plug_reference_0;
byte plug_reference_1;
byte plug_reference_2;
byte plug_reference_3;


// Leds
byte light_profile = 0;

byte* light_hue;
byte* light_saturation;
byte* light_value;
byte* light_animation;

byte light_hue_0;
byte light_hue_1;
byte light_hue_2;

byte light_saturation_0;
byte light_saturation_1;
byte light_saturation_2;

byte light_value_0;
byte light_value_1;
byte light_value_2;

byte light_animation_0;
byte light_animation_1;
byte light_animation_2;

// Timers
unsigned long int no_interaction;

// Microwave sensor
bool moovement_state = 1;
unsigned long int no_moovement;
unsigned int mill_wake;
unsigned int mill_wake_ignore;

byte moove_timer;
byte moove_wake;
bool shutdown_method;

// Microphone
/*unsigned long int no_microphone;
byte tap_map[5];
byte tap_index;*/


// Serial comunication
char data_light[4];
bool requested;