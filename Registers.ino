byte findButton(){
    byte i = 9;
    byte c = 4;
    /*regState[0] = B00000100;
    regState[1] = B10000000;*/
    
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
    return i - 8;
}

void pushBits(){
    CLR(PORTD, 7);
    shiftOut(DATA_OUT, RCLK, MSBFIRST, regState[0]);
    shiftOut(DATA_OUT, RCLK, MSBFIRST, regState[1]);
    SET(PORTD, 7);
}

void setBits(byte a, byte b, bool bit){
    if (b > 15){
        b = 15;
    }
    if (a < 0){
        a = 0;
    }

    byte temp[2] = { 0 };
    byte i;
    for (i = 0; i < 16; i++){
        temp[i / 8]<<=1;
        if (i >= a && i <= b){
            temp[i / 8]++;
        }
    }
    if (bit){
        regState[0] |= temp[0];
        regState[1] |= temp[1];
    }
    else{
        regState[0] -= (regState[0] & temp[0]);
        regState[1] -= (regState[1] & temp[1]);
    }
}