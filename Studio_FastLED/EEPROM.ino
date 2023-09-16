void eepromDownload(){
    byte i;
    for (i = 0; i < 4; i++){
        led_config[i] = EEPROM.read(i);
    }
}

void eepromUpload(){
    byte i;
    for (i = 0; i < 4; i++){
        EEPROM.update(i,led_config_queue[i]);
    }
}