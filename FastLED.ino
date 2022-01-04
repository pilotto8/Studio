void setupPalette(){
    fill_solid(currentPalette, 16, CRGB::Black);
    CRGB custom = CHSV(27, 175, 255);
    CRGB black  = CRGB::Black;
    CRGB base  = CRGB::LightSeaGreen;
    currentPalette = CRGBPalette16(
    custom, black, black, black,
    black, black, black, black,
    black, black, black, black,
    black, black, black, black
    );
    //currentPalette[0] = CRGB::Salmon;
    currentBlending = LINEARBLEND;
}


void RGBhandle(){
    int i;
    if (previousAnimation != animation){
        previousAnimation = animation;
        animation_finished = 0;
        // Setup
        switch (animation){
            case 0: {
                for (i = 0; i < 100; i++){
                    leds[i] = CRGB::Black;
                }
                fillBuffer(16);
                LEDbuffer[9] = 15;
                LEDbuffer[10] = 15;
                break;
            }
        }
    }
    if (animation_finished == 0){
        switch (animation){
            case 0: {
                reportBuffer();
                animation_finished = wave(decrease);
                break;
            }
        }
    }
}


void reportBuffer(){
    int i;
    for (i = 0; i < 20; i++){
        leds[i] = ColorFromPalette(currentPalette, LEDbuffer[i], 255, currentBlending);
        singleRow(i);
    }
}
void fillBuffer(int value){
    int i;
    for(i = 0; i < 21; i++){
        LEDbuffer[i] = value;
    }
}

bool wave(int mode){
    int i, c;
    bool completed = 1;
    for (i = 0; i < 20; i++){
        if (LEDbuffer[i] != 0 && LEDbuffer[i] != 16){
            completed = 0;
            LEDbuffer[i] += mode;
            for (c = -1; c <= 1; c += 2){
                if (LEDbuffer[i + c] == 0 && mode == increase || LEDbuffer[i + c] == 16 && mode == decrease){
                    LEDbuffer[i + c] += mode;
                    if (c == 1){
                        i++;
                    }
                }
            }
        }
    }
    return completed;
}

void singleRow(int i){
    leds[i + 40] = leds[i];
    leds[i + 80] = leds[i];
    leds[39 - i] = leds[i];
    leds[79 - i] = leds[i];
}
void RGBclear(){
    int i;
    for (i = 0; i < 100; i++){
        leds[i] = CRGB::Black;
    }
}