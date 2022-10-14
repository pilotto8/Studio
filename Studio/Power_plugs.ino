void pushPlugState(byte plug, byte state){
    if (state == 2){
        state = !readBits(plug + 5);
    }
    setBits(plug + 5, state);
    digitalWrite(plug + 3, state);

    pointerPlug(3 - plug);
    if (*plug_limit > 0){
        if (state){
            *plug_reference = addTimer(*plug_limit, 1 << (7 - plug), 0);
        }
        else {
            if (alarm_timer[*plug_reference].plugs - (1 << (7 - plug)) != 0){
                setBits(&alarm_timer[*plug_reference].plugs, plug, 0);
            }
            else {
                deleteTimer(*plug_reference);
            }
        }
    }
}

void savePlugState(){
    if (reg_freezed){
        EEPROM.update(0, temp_regState[0]);
        EEPROM.update(1, temp_regState[1]);
    }
    else {
        EEPROM.update(0, regState[0]);
        EEPROM.update(1, regState[1]);
    }
}

void pointerPlug(byte plug){
    switch (plug){
        case 0:{
            plug_limit = &plug_limit_0;
            plug_trigg = &plug_trigg_0;
            plug_reference = &plug_reference_0;
            break;
        }
        case 1:{
            plug_limit = &plug_limit_1;
            plug_trigg = &plug_trigg_1;
            plug_reference = &plug_reference_1;
            break;
        }
        case 2:{
            plug_limit = &plug_limit_2;
            plug_trigg = &plug_trigg_2;
            plug_reference = &plug_reference_2;
            break;
        }
        case 3:{
            plug_limit = &plug_limit_3;
            plug_trigg = &plug_trigg_3;
            plug_reference = &plug_reference_3;
            break;
        }
    }
}
