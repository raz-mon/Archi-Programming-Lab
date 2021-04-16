




void getParams(char* params, int argc, char** argv){
    if (argc == 2){
        for (int i = 0; i < 2; i++){
            params[i] = argv[1][i];
        }
    }
}