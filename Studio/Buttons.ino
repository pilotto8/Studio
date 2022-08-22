byte findButton(){
    byte i = 9;
    byte c = 4;
    
    setBits(13, 15, 1);
    while(c >= 1){
        setBits(i, i + c - 1, 0);
        pushBits();
        setBits(i, i + c - 1, 1);
        if (digitalRead(DATA_IN)){
            i += c;
        }
        c /= 2;
    }
    if (!(i % 2)){
        setBits(i, i + 1, 0);
        pushBits();
    }
    reg_update = 0;
    return i - 8;
}

void buttonsHandle(){
    if (!digitalRead(DATA_IN)){
        if (millis() - last_millis > debounce){
            if (!button){
                button = findButton();
                switch (button){
                    case 1 ... 3:{
                        #if LUCE
                        if (button == center){
                            animation = !animation;
                        }
                        #endif
                        if (button == center){
                            button_pulse = center;
                        }
                        break;
                    }
                    case 4 ... 7:{
                        if (interface == alarm_inter){
                            setBits(&temp_plugs, button - 4, 2);
                            setBits(button + 1, 2);
                        }
                        //pushPlugState(button - 4, 2);
                        break;
                    }
                }
            }
            
            if (button == up || button == down){
                if (button_pulse == 0 && int((millis() - last_millis + debounce) / 10) % scrolling_time == 0){
                    button_pulse = button;
                }
                else if (int((millis() - last_millis + debounce) / 10) % scrolling_time != 0){
                    button_pulse = 0;
                }
            }

            if (button > 3 && interface != alarm_inter){
                if (millis() - last_millis >= long_press){
                    freeze_registers();
                    temp_plugs = 0;
                    setBits(&temp_plugs, button - 4, 1);
                    setBits(button + 1, 1);
                    interface = alarm_inter;
                }
            }

        }
    }
    else {
        if (button){
            setBits(9, 15, 0);
            if (button > 3){
                if (interface != alarm_inter){
                    if (millis() - last_millis < long_press){
                        pushPlugState(button - 4, 2); /// State 2 means to invert the current state
                        savePlugState();
                    }
                }
                else {
                    //setBits(&temp_plugs, button - 4, 2);
                }
            }

            button = 0;
            button_pulse = 0;
        }


        /*if (interface == alarm_inter){ // To make selected plugs blink
            if (millis() - blink_reg_millis >= 500){
                blink_reg_millis = millis();
                regState[0] ^= temp_plugs >> 5;
                regState[1] ^= temp_plugs << 3;
                reg_update = 1;
            }
        }*/


        if (reg_update){
            pushBits();
            reg_update = 0;
        }
        last_millis = millis();
    }
}