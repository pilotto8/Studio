void findButton(){
    int i;
    CLR(PORTD, 7);
    shiftOut(DATA_OUT, RCLK, MSBFIRST, B00000100);
    shiftOut(DATA_OUT, RCLK, MSBFIRST, B10001000);
    SET(PORTD, 7);
    
}