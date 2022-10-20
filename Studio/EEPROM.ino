const byte* address[] {
    &temp_minute, &temp_hour, &temp_day, &temp_month, &temp_year,
    &temp_num_plug, 
    &plug_trigg_0, &plug_trigg_1, &plug_trigg_2, &plug_trigg_3,
    &plug_limit_0, &plug_limit_1, &plug_limit_2, &plug_limit_3,
    &light_profile,
    &light_hue_0, &light_hue_1, &light_hue_2,
    &light_saturation_0, &light_saturation_1, &light_saturation_2,
    &light_value_0, &light_value_1, &light_value_2,
    &light_animation_0, &light_animation_1, &light_animation_2,
    &moove_timer
    ,0};

void writeParam(byte* pointer, byte value){
    *pointer = value;
}

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
        writeParam (address[i], EEPROM.read(i + eeprom_offset));
        parExecutor(i);
    }
}

void parExecutor(byte i){ 
    switch (i){
        case 0 ... 4:{
            if (interface == clock_inter){
                byte temp_second = 0;
                if (i == 0){
                    rtc.clearAlarm(1);
                }
                else {
                    temp_second = now.second();
                }
                rtc.adjust(DateTime(temp_year + 2000, temp_month, temp_day, temp_hour, temp_minute, temp_second));

                if (num_timer > 0){
                    setTimer();
                }
                if (i == 0){
                    for (byte c = 0; c < 10 && c < num_timer; c++){
                        alarm_timer[i].time_span++;
                    }
                }
                getTime();
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
        case  14 ... 26:{
            if (interface == light_inter || i == 26){
                sendLightData(1); // profilePointer() already included
                if (interface == light_inter){
                    element_list[2].pointer = light_hue;
                    element_list[3].pointer = light_saturation;
                    element_list[4].pointer = light_value;
                    element_list[5].pointer = light_animation;
                }
            }
            break;
        }
    }
}

String downloadString(byte num_string){
    byte i, q;
    String temp = 0;
    for (i = 0, q = 0; i < num_string; i++, q += string_bytes[i]){}
    for (i = 0; i < string_bytes[i]; i++){
        temp += (char)EEPROM.read(q + i);
    }
    return temp;
}