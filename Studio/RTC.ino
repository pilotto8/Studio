void getTime(){
    now = rtc.now();
}

void setAlarm(byte hour, byte minute, byte plugs, byte state){
    
}

struct {
    byte plugs;
    bool state;
    DateTime alarm;
    byte days;
}rtc_trigg[8];

byte active_trigg;

void setBits(int* byt, byte a, bool bit){
    byte temp = 1;
    temp =<< a; 
    if (bit){
        *byt |= temp;
    }
    else {
        *byt -= (*byt & temp);
    }
}


bool readBits(int* byt, byte a){
    byte temp = *byt << a;
    if (temp >> 7){
        return 1;
    }
    return 0;
}