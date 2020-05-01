#include <stdio.h>

extern "C" void print(char *s) {
    printf("Load/store Instruction \n");
    printf("%s\n", s);
}