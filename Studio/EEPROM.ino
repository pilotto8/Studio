byte* address[]{
    &prova, &temp_minute, &temp_hour, &temp_day, &temp_month, &temp_year
    ,0};

void eepromUpdate(byte* pointer){
    byte i;
    for (i = 0; address[i] != 0; i++){
        if (pointer == address[i]){
            EEPROM.update(i + eeprom_offset, *pointer);
            parExecutor(i);
            return;
        }
    }
}

void eepromDownload(){
    byte i;
    for (i = 0; address[i] != 0; i++){
        *address[i] = EEPROM.read(i + eeprom_offset);
        parExecutor(i);
    }
}

void parExecutor(byte i){ 
    switch (i){
        case 1 ... 5:{
            rtc.adjust(DateTime(temp_year, temp_month, temp_day, temp_hour, temp_minute, 0));
            break;
        }
    }
}