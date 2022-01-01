void setupPalette(){
    
}
void RGBanimation(int animation){
    switch (animation){
        case 0:{

            break;
        }
    }
}
void singleRGBrow(){
    int i;
    for(i = 0; i < 20; i++){
        leds[i + 40] = leds[i];
        leds[i + 60] = leds[i];
    }
}