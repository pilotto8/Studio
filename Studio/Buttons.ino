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
                        if (millis() - last_millis >= long_press){

                        }
                        //pushPlugState(button - 3, 2);
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
        }
    }
    else {
        if (button){
            setBits(9, 15, 0);
            if (millis() - last_millis < long_press){
                pushPlugState(button - 3, 2); /// State 2 means to invert the current state
                savePlugState();
            }
            button = 0;
            button_pulse = 0;
        }
        if (reg_update){
            pushBits();
            reg_update = 0;
        }
        last_millis = millis();
    }
}