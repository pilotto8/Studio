void oledInit(){
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println("failed");
    }
    display.clearDisplay();
    display.setRotation(2);
    display.setTextSize(1);      // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE); // Draw white text
    display.setCursor(0, 0);     // Start at top-left corner
    display.print("Ciao");         // Use full 256 char 'Code Page 437' font
    display.display();
}
int prova = 3;
void loadInterface(){
    if (interface != prev_interface){
        prev_interface = interface;
        //Setup interface
        switch (interface){
            case home_inter:{
                element_total = 0;
                title_list = 0;
                break;
            }
            case settings_inter:{
                element_total = 4;
                title_list = 1;
                defElement(0, "Settings", home_inter);
                defElement(1, "Light", light_inter);
                defElement(2, "Power plug", plug_inter);
                defElement(3, "Prova", &prova, 0, 5);
                break;
            }
        }
        if (title_list){
            element_selected = 1;
        }
        else {
            element_selected = 0;
        }
        oled_update = 1;
    }
    //Running interface
    switch (interface){
        case home_inter:{
            break;
        }
        case settings_inter ... light_inter:{
            interfaceList();
            break;
        }
    }
}

struct {
    String name;
    int* pointer;
    byte interface;
    byte min;
    byte max;
}element_list[10];

void defElement(byte number, String name, byte interface){
    element_list[number].name = name;
    element_list[number].interface = interface;
}
void defElement(byte number, String name, int* pointer, byte min, byte max){
    element_list[number].name = name;
    element_list[number].pointer = pointer;
    element_list[number].min = min;
    element_list[number].max = max;
    element_list[number].interface = 0;
}

#define ROWS 5
void interfaceList(){
    int i;
    if (button_pulse != 0 && button_pulse != -1){
        if (button_pulse == center){
            if (element_list[element_selected].interface == 0){
                // Some variables may have problems with on time updates so I use a temporaneous variable
                if (!selector){
                    *element_list[element_selected].pointer = temp;
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
    if (oled_update){
        display.clearDisplay();
        oled_update = 0;
        for (i = 0; i < element_total - (i - i % ROWS) && i < ROWS; i++){
            display.setCursor(7, (64 / ROWS) * i);
            display.print(element_list[element_selected - (element_selected % ROWS) + i].name);
            if (element_list[element_selected - (element_selected % ROWS) + i].interface == 0){
                display.setCursor(100, (64 / ROWS) * i);
                if (!selector && i == element_selected % ROWS){
                    display.print(temp);
                }
                else {
                    display.print(*element_list[element_selected - (element_selected % ROWS) + i].pointer);
                }
            }
        }
        if (selector){
            display.drawCircle(0, (64 / ROWS) * (element_selected % ROWS) + 4, 2, SSD1306_WHITE);
        }
        else {
            display.fillCircle(0, (64 / ROWS) * (element_selected % ROWS) + 4, 2, SSD1306_WHITE);
        }
        if (title_list && element_selected / ROWS == 0){
            display.drawFastHLine(0, 64 / ROWS - 4, 128, SSD1306_WHITE);
        }
        display.display();
    }
}