void sendPreset(byte hue, byte saturation){
    bus.write(100);
    bus.write(hue);
    bus.write(saturation);
}

void sendBrightness(byte index, byte value){
    bus.write((char)index);
    bus.write((char)value);
}