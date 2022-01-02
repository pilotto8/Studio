void setupPalette(){
    fill_solid(currentPalette, 16, CRGB::Black);
    currentPalette[0] = CHSV(25, 190, 255);
    currentBlending = NOBLEND;
}
byte steps_table[]{
    11, 11
};
void RGBhandle(){
    int i;
    if (previousAnimation != animation){
        previousAnimation = animation;
        animationSteps = steps_table[animation];
    }
    if (animationSteps != 0){
        animationSteps--;
        Serial.println(animationSteps);
        switch (animation){
            case 0: {
                if(animationSteps == 10){
                    RGBclear();
                }
                else {
                    
                    
                    i = 9 - animationSteps;
                    leds[i + 10] = ColorFromPalette(currentPalette, 0, BRIGHTNESS, currentBlending);
                    singleRow(i + 10);
                    leds[9 - i] = ColorFromPalette(currentPalette, 0, BRIGHTNESS, currentBlending);
                    singleRow(9 - i);
                    break;
                }
                
            }
        }
    }
}

void singleRow(int i){
    leds[i + 40] = leds[i];
    leds[i + 80] = leds[i];
    leds[39 - i] = leds[i];
    leds[79 - i] = leds[i];
}
void RGBclear(){
    for (i = 0; i < 100; i++){
        leds[i] = CRGB::Black;
    }
}