void oledInit(){
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println("failed");
    }
    display.clearDisplay();
    display.setRotation(2);
    display.setTextSize(1);      // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE); // Draw white text
    display.display();
}

void loadInterface(){
    if (interface != prev_interface){
        prev_interface = interface;
        //Setup interface
        element_total = 0;
        switch (interface){
            case home_inter:{
                break;
            }
            case settings_inter:{
                title_list = 1;
                defElement(0, "Settings", home_inter);
                defElement(1, "Light", light_inter);
                defElement(2, "Power plug", plug_inter);
                defElement(3, "Clock", clock_inter);
                defElement(4, "Oled", oled_inter);
                defElement(5, "Prova", &prova, 0, 5);
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
        case home_inter:{
            break;
        }
        default:{
            interfaceList(); // The vast majority of the interfaces are lists
            break;
        }
    }
}

void defElement(byte number, String name, byte interface){
    element_list[number].name = name;
    element_list[number].interface = interface;
    element_total++;
}
void defElement(byte number, String name, int* pointer, byte min, byte max){
    element_list[number].name = name;
    element_list[number].pointer = pointer;
    element_list[number].min = min;
    element_list[number].max = max;
    element_list[number].interface = 0;
    element_total++;
}

#define rows 5
void interfaceList(){
    int i;
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
        button_pulse = -1;
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
                    display.print(temp);
                }
                else {
                    display.print(*element_list[element_selected - (element_selected % rows) + i].pointer);
                }
            }
            else if ((title_list && element_selected / rows == 0) && i != 0 || !(element_selected / rows == 0 && title_list)){ 
                display.print(char(62));
            }
        }
        if (selector){
            display.drawCircle(0, (64 / rows) * (element_selected % rows) + 4, 2, SSD1306_WHITE);
        }
        else {
            display.fillCircle(0, (64 / rows) * (element_selected % rows) + 4, 2, SSD1306_WHITE);
        }
        if (title_list && element_selected / rows == 0){
            display.drawFastHLine(0, 64 / rows - 4, 128, SSD1306_WHITE);
        }
        display.display();
    }
}