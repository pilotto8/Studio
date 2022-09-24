void oledInit(){
    display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
    display.clearDisplay();
    display.setRotation(2);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.display();
}

void loadInterface(){
    if (interface != prev_interface){
        prev_interface = interface;
        //Setup interface
        element_total = 0;
        switch (interface){
            case sleep_inter:{
                display.clearDisplay();
                display.display();
                freeze_registers();
                break;
            }
            case home_inter:{
                
                break;
            }
            case settings_inter:{
                title_list = 1;
                defElement(0, F("Setting"), home_inter);
                defElement(1, F("Light"), light_inter);
                defElement(2, F("Plugs"), plug_inter);
                defElement(3, F("Clock"), clock_inter);
                defElement(4, F("Wake"), &moove_timer, 1, 10);
                break;
            }

            case light_inter:{
                pointerProfile(light_profile);
                title_list = 1;
                defElement(0, F("Light"), home_inter);
                defElement(1, F("Prof."), &light_profile, 0, 2);
                defElement(2, F("Hue"), light_hue, 0, 255);
                defElement(3, F("Sat."), light_saturation, 0, 255);
                defElement(4, F("Value"), light_value, 0, 255);
                defElement(5, F("Anim."), light_animation, 0, 2);
                break;
            }

            case plug_inter:{
                pointerPlug(temp_num_plug);
                title_list = 1;
                defElement(0, F("Plugs"), home_inter);
                defElement(1, F("Num"), &temp_num_plug, 0, 3);
                defElement(2, F("Limit"), plug_limit, 0, 255);
                defElement(3, F("Trigg"), plug_trigg, 0, 1);
                break;
            }
            
            case clock_inter:{
                saveTempData();
                title_list = 1;
                defElement(0, F("Clock"), home_inter);
                defElement(1, F("Min"), &temp_minute, 0, 59);
                defElement(2, F("Hour"), &temp_hour, 0, 23);
                defElement(3, F("Day"), &temp_day, 1, 31);
                defElement(4, F("Month"), &temp_month, 1, 12);
                defElement(5, F("Year"), &temp_year, 0, 99);
                break;
            }

            case alarm_inter:{
                //temp_time_span = 1;
                break;
            }
        }
        if (title_list){
            element_selected = 1;
        }
        else {
            element_selected = 0;
        }
        oled_update = 1; // To add on the running function of every interface 
    }
    //Running interface
    switch (interface){
        case sleep_inter:{
            if (button_pulse != 0 && button_pulse != -1){
                if (button_pulse == center){
                    button_pulse = 0;
                }
            }
            if (millis() - no_moovement < 720000){
                resume_registers();
                interface = home_inter;
            }
            break;
        }


        case home_inter:{
            if (button_pulse != 0 && button_pulse != -1){
                if (button_pulse == center){
                    interface = settings_inter;
                    button_pulse = 0;
                }
            }

            if (update_clock_data){
                update_clock_data = 0;
                oled_update = 1;
            }

            if (oled_update){
                display.clearDisplay();
                oled_update = 0;
                getTime();

                display.setTextSize(4);
                display.setCursor(5, 10);
                printZero(now.hour());
                display.print(':');
                //display.print(((millis() / 1000) % 2) ? ':':' ');
                printZero(now.minute());

                display.setTextSize(1);
                display.setCursor(5, 0);
                //display.print(days[now.dayOfTheWeek()]);
                printZero(now.day());
                display.print('/');
                printZero(now.month());

                if (num_timer > 0){
                    for (byte i = 0; i < num_timer && i < 3; i++){
                        display.setCursor(5, 40 + 8 * i);
                        if (alarm_timer[i].time_span >= 60){
                            display.print(alarm_timer[i].time_span / 60);
                            display.print(F("h "));
                        }
                        display.print(alarm_timer[i].time_span % 60);
                        display.print(F("min"));
                        printPlugBalls(90, 42 + 8 * i, i);
                    }
                }

                if (digitalRead(MW_DATA)){
                    display.setCursor(120, 0);
                    display.print('*');
                }
                
                // Just for testing purposes
                /*display.setCursor(0,55);
                display.print(moovement_state);
                display.print(digitalRead(LED_BUTTON));
                display.print(digitalRead(MW_DATA));
                display.drawFastHLine(0, 63, 128 - ((millis() - no_moovement) * 128 / (moove_timer * 60000)), SSD1306_WHITE);*/
                //

                display.display();
            }
            break;
        }

        case alarm_inter:{
            if (button_pulse != 0 && button_pulse != -1){
                if (button_pulse == center){
                    interface = home_inter;
                    button_pulse = 0;

                    addTimer(temp_time_span, temp_plugs, 1);
                    resume_registers();
                }

                else if (button_pulse == up && temp_time_span < 255){
                    temp_time_span++;
                }
                else if (button_pulse == down && temp_time_span > 1){
                    temp_time_span--;
                }
                oled_update = 1;
            }

            if (oled_update){
                display.clearDisplay();
                oled_update = 0;

                display.setTextSize(4);
                display.setCursor(20, 25);
                display.print(temp_time_span);

                display.setTextSize(2);
                display.print(F("min"));
                
                display.setCursor(40, 0); // x = 40
                display.print(F("Timer "));

                display.setTextSize(1);

                display.display();
            }

            if (temp_plugs == 0){
                interface = home_inter;
                resume_registers();
            }

            break;
        }
        default:{
            interfaceList(); // The vast majority of the interfaces are lists
            break;
        }
    }
}

void defElement(byte number, String name, byte interface){
    if (number > max_element_list){
        return;
    }
    element_list[number].name = name;
    element_list[number].interface = interface;
    element_total++;
}
void defElement(byte number, String name, byte* pointer, byte min, byte max){
    if (number > max_element_list){
        return;
    }
    element_list[number].name = name;
    element_list[number].pointer = pointer;
    element_list[number].min = min;
    element_list[number].max = max;
    element_list[number].interface = 0;
    element_total++;
}

void interfaceList(){
    byte i;
    // Buttons interaction
    if (button_pulse != 0 && button_pulse != -1){
        if (button_pulse == center){
            if (element_list[element_selected].interface == 0){
                // Some variables may have problems with real time updates so I use a temporaneous variable
                if (!selector){
                    *element_list[element_selected].pointer = temp;
                    eepromUpdate(element_list[element_selected].pointer);
                }
                else {
                    temp = *element_list[element_selected].pointer;
                }
                selector = !selector;
            }
            else {
                interface = element_list[element_selected].interface;
            }
            button_pulse = 0; // To add on every center command
        }
        else if (button_pulse == up){
            if (selector && element_selected > 0){
                element_selected--;
            }
            else if (!selector && temp < element_list[element_selected].max){
                temp++;
            }
        }
        else if (button_pulse == down){
            if (selector && element_selected < element_total - 1){
                element_selected++;
            }
            else if (!selector && temp > element_list[element_selected].min){
                temp--;
            }
        }
        oled_update = 1;
    }

    // Print elements on the display
    if (oled_update){
        display.clearDisplay();
        oled_update = 0;
        for (i = 0; i < element_total - (element_selected - element_selected % rows) && i < rows; i++){
            display.setCursor(7, (64 / rows) * i);
            display.print(element_list[element_selected - (element_selected % rows) + i].name);
            display.setCursor(100, (64 / rows) * i);
            if (element_list[element_selected - (element_selected % rows) + i].interface == 0){
                if (!selector && i == element_selected % rows){
                    if (element_list[element_selected - (element_selected % rows) + i].max == 1 && element_list[element_selected - (element_selected % rows) + i].min == 0){
                        display.print(temp ? "ON" : "OFF");
                    }
                    else {
                        display.print(temp);
                    }
                }
                else {
                    if (element_list[element_selected - (element_selected % rows) + i].max == 1 && element_list[element_selected - (element_selected % rows) + i].min == 0){
                        display.print(*element_list[element_selected - (element_selected % rows) + i].pointer ? "ON" : "OFF");
                    }
                    else {
                        display.print(*element_list[element_selected - (element_selected % rows) + i].pointer);
                    }
                }
            }
            else if ((title_list && element_selected / rows == 0) && i != 0 || !(element_selected / rows == 0 && title_list)){ 
                display.print(char(62));
            }
        }
        if (selector){
            display.drawCircle(3, (64 / rows) * (element_selected % rows) + 3, 2, SSD1306_WHITE);
        }
        else {
            display.fillCircle(3, (64 / rows) * (element_selected % rows) + 3, 2, SSD1306_WHITE);
        }
        if (title_list && element_selected / rows == 0){
            display.drawFastHLine(0, 64 / rows - 4, 128, SSD1306_WHITE);
        }
        display.display();
    }
}

void printZero(byte value){
    if (value < 10){
        display.print(0);
    }
    display.print(value);
}

void printPlugBalls(byte x, byte y, byte alarm){
    byte i;
    for (i = 0; i < 4; i++){
        if (readBits(&alarm_timer[alarm].plugs, 3 - i)){
            display.fillCircle(x + 6 * i, y, 2, SSD1306_WHITE);
        }
        else{
            display.drawCircle(x + 6 * i, y, 2, SSD1306_WHITE);
        }
    }
}