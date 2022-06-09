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

void loadInterface(){
    if (interface != prev_interface){
        interface = prev_interface;
        //Setup interface
        switch (interface){
            case home_inter:{
                element_total = 0;
                title_list = 0;
                break;
            }
            case settings_inter:{
                element_total = 2;
                title_list = 1;
                defElement(0, "Light", light_inter);
                defElement(1, "power plug", plug_inter)
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
    char name[10];
    int *pointer;
    byte interface;
    byte min;
    byte max;
}element_list[10];

void defElement(byte number, char name[10], byte interface){
    element_list[number].name = name;
    element_list[number].interface = interface;
}
void defElement(byte number, char name[10], int pointer, byte min, byte max){
    element_list[number].name = name;
    element_list[number].pointer = pointer;
    element_list[number].min = min;
    element_list[number].max = max;
}

void interfaceList(){
    if (oled_update){
        oled_update = 0;

    }
}