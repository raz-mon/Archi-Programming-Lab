
int system_call();

void encoder(){
    char c[1];
    while(1){
        system_call(3, 0, c, 1);
        if (*c >= 65 && *c <= 90){
            *c = *c + 32;
            system_call(4, 1, c, 1);
        }
        else if (*c == 0)
            break;
        else
            system_call(4, 1, c, 1);
    }
}

void encoder_in(char* filename){
    
    int fd = system_call(5, filename, 0, 0777);
    if (fd < 0){
        system_call(4, 2, "cannot open file!", 18);
        system_call(1, 0x55, 0, 0);
    }
    char c[1];
    while(1){
        int read = system_call(3, fd, c, 1);
        if (*c >= 65 && *c <= 90){
            *c = *c + 32;
            system_call(4, 1, c, 1);
        }
        else if (read == 0)
            break;
        else
            system_call(4, 1, c, 1);
    }
}

void encoder_out(char* filename){
    int fd = system_call(5, filename, 2, 0777);
    if (fd < 0){
        system_call(4, 2, "cannot open file!", 18);
        system_call(1, 0x55, 0, 0);
    }
    char c[1];
    while(1){
        int read = system_call(3, 0, c, 1);
        if (*c >= 65 && *c <= 90){
            *c = *c + 32;
            system_call(4, fd, c, 1);
        }
        else if (read == 0)
            break;
        else
            system_call(4, fd, c, 1);
    }
    
}

void encoder_in_and_out(char* in_name, char* out_name){
    int in = system_call(5, in_name, 2, 0777);
    if (in < 0){
        system_call(4, 2, "cannot open file!", 18);
        system_call(1, 0x55, 0, 0);
    }
    int out = system_call(5, out_name, 2, 0777);
    if (out < 0){
        system_call(4, 2, "cannot open file!", 18);
        system_call(1, 0x55, 0, 0);
    }
    char c[1];
    while(1){
        int read = system_call(3, in, c, 1);
        if (*c >= 65 && *c <= 90){
            *c = *c + 32;
            system_call(4, out, c, 1);
        }
        else if (read == 0)
            break;
        else
            system_call(4, out, c, 1);
    }
}


