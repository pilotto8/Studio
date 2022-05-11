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

