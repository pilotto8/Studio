int* address[]{&prova, 0};

void eepromUpdate(int* pointer){
    int i;
    for (i = 0; address[i] != 0; i++){
        if (pointer == address[i]){
            EEPROM.update(i, *pointer);
            parExecutor(i);
            return;
        }
    }
}

void eepromDownload(){
    int i;
    for (i = 0; address[i] != 0; i++){
        *address[i] = EEPROM.read(i);
        parExecutor(i);
    }
}

void parExecutor(int i){ 
    switch (i){
        case 0:{
            break;
        }
    }
}