#include "Settings.h"
#define OLED 1
#define BUTTONS 1
#define RTC 1
#define MOOVEMENT 1
volatile byte* NEW_PARAMETER[] = {

0}; // Just for developing purposes. If there are new parameters just put them here one time


// Debug flags
#define shift_setup 0

void setup() {
    Serial.begin(1000000);
    pinMode(SRCLK, OUTPUT);
    pinMode(RCLK, OUTPUT);
    pinMode(DATA_OUT, OUTPUT);
    pinMode(DATA_IN, INPUT_PULLUP);
    
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);

    pinMode(MW_DATA, INPUT);
    pinMode(LED_BUTTON, INPUT_PULLUP);

    pinMode(WAKE_SERIAL, OUTPUT);
    pinMode(INTERRUPT, INPUT);
    //attachInterrupt(digitalPinToInterrupt(INTERRUPT), pushSerial, RISING);
    //digitalWrite(WAKE_SERIAL, 0);
    
    #if OLED
    oledInit();
    #endif

    
    #if RTC
    rtc.begin();
    rtc.disable32K();
    rtc.clearAlarm(1);
    rtc.clearAlarm(2);
    rtc.writeSqwPinMode(DS3231_OFF);
    rtc.disableAlarm(2);
    //rtc.setAlarm2(rtc.now(),DS3231_A2_PerMinute);
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    #endif

    
    #if !shift_setup
    if (NEW_PARAMETER[0] != 0){
        for(byte i = 0; NEW_PARAMETER[i]; i++){ 
            eepromUpdate(NEW_PARAMETER[i]);
        }
    }

    EEPROM.update(14 + eeprom_offset, 0);
    /*EEPROM.update(15 + eeprom_offset, 28);
    EEPROM.update(18 + eeprom_offset, 170);
    EEPROM.update(21 + eeprom_offset, 255);
    EEPROM.update(24 + eeprom_offset, 1);*/
    
    eepromDownload();
    
    regState[0] = EEPROM.read(0);
    regState[1] = EEPROM.read(1);
    if (regState[1] << 1 != 0){ //Recovery from register corruption (buttons wuldn't work)
        regState[1] >>= 6;
        regState[1] <<= 6;
    }
    pushBits();
    for (byte i = 0; i < 4; i++){
        byte state = readSReg(0, i + 4);
        byte plug = i - 1;
        if (state){
            pointerPlug(3 - plug);
            if (*plug_limit > 0){
                *plug_reference = addTimer(*plug_limit, 1 << (7 - plug), 0);
            }

            digitalWrite(i + 3, 1);
        }
        
    }

    update_clock = millis();
    no_interaction = millis();
    no_moovement = millis();
    #endif

}


void loop() {
    #if BUTTONS
        buttonsHandle();
    #endif

    #if OLED
        loadInterface();
    #endif

    #if RTC
        if (millis() - update_clock >= 200){
            update_clock = millis();
            checkTimer();
            update_clock_data = 1;
        }
    #endif

    if (interface != home_inter && interface != sleep_inter && millis() - no_interaction >= 40000){
        //sendLightData(28, 170, 255, 2);
        selector = 1;
        if (interface == alarm_inter){
            #if BUTTONS
            resume_registers();
            #endif
        }
        interface = home_inter;
    }

    #if MOOVEMENT
        checkMoovement();
    #endif

    if (serial_call){
        if (millis() - mill_serial_call > 150){
            pushSerial();
            serial_call = 0;
        }
    }
}

/*
void serialEvent(){
    if (Serial.available() > 0){
        byte receiver = 0;
        receiver = Serial.read();
        if (receiver == 1){
            pushSerial();
        }
    }
}*/