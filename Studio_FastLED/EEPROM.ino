void eepromDownload(){
    byte i;
    for (i = 0; i < 4; i++){
        led_config[i] = eeprom.read(i);
    }
}

void eepromUpload(){
    byte i;
    for (i = 0; i < 4; i++){
        eeprom.update(i,led_config[i]);
    }
}