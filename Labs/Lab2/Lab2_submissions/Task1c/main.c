

int main(int argc, char* argv[]){
    int iarray[3];
    /*      Two ways of declaring a string dynamically - On the heap.
    char *wer = "almog";
    char asdfasiarray[] = "raz";
    */
    float farray[3];
    double darray[3];
    char carray[3];
    int* iptrarray[3];
    char* cptrarray[3];

    printf("- &iarray: %p\n", &iarray);
    printf("- &farray: %p\n", &farray);
    printf("- &darray: %p\n", &darray);
    printf("- &carray: %p\n", &carray);
    printf("- &iptarray: %p\n", &iptrarray);
    printf("- &cptarray: %p\n", &cptrarray);

    printf("\n\n\n");
    printf("- iarray: %p\n", iarray);
    printf("- farray: %p\n", farray);
    printf("- darray: %p\n", darray);
    printf("- carray: %p\n", carray);
    printf("- iptarray: %p\n", iptrarray);
    printf("- cptarray: %p\n", cptrarray);

printf("\n\n\n");
    printf("- &iarray[1]: %p\n", iarray + 1);
    printf("- &farray:[1] %p\n", farray + 1);
    printf("- &darray:[1] %p\n", darray + 1);
    printf("- &carray:[1] %p\n", carray + 1);
    printf("- &iptarray:[1] %p\n", iptrarray + 1);
    printf("- &cptarray:[1] %p\n", cptrarray + 1);

}