byte* address[]{
    &temp_minute, &temp_hour, &temp_day, &temp_month, &temp_year,
    &temp_num_plug, &plug_trigg_0, &plug_trigg_1, &plug_trigg_2, &plug_trigg_3,
    &plug_limit_0, &plug_limit_1, &plug_limit_2, &plug_limit_3
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
        case 0 ... 4:{
            if (interface == clock_inter){
                if (i == 0){
                    rtc.clearAlarm(1);
                }
                rtc.adjust(DateTime(temp_year + 2000, temp_month, temp_day, temp_hour, temp_minute, 0));

                if (num_timer > 0){
                    setTimer();
                }
                if (i == 0){
                    for (byte c = 0; c < 10 && c < num_timer; c++){
                        alarm_timer[i].time_span++;
                    }
                }
                saveTempData();
            }
            break;
        }
        case 5:{
            if (interface == plug_inter){
                pointerPlug(temp_num_plug);
                element_list[2].pointer = plug_limit;
                element_list[3].pointer = plug_trigg;
            }
            break;
        }
    }
}