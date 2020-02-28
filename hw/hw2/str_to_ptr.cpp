#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct tag_ELEM {
    int value;
} ELEM;
typedef struct tag_BUFENC {
    ELEM* pElem[8];
    struct tag_BUFENC* pNext;
} BUFENC;

void print_bufenc(BUFENC* pFirst) {
    BUFENC* pCur = pFirst;
    while (pCur) {
        printf("BUFENC: %p\n", pCur);
        for (int i = 0; i < 8; i++) {
            printf(" - pElem[%d] = %p", i, pCur->pElem[i]);
            if (pCur->pElem[i] != (ELEM*)0xdeadbeef) {
                printf(", Value: %x\n", pCur->pElem[i]->value);
            } else {
                printf("\n");
            }
        }
        printf(" - Next: %p\n", pCur->pNext);
        pCur = pCur->pNext;
    }
}

BUFENC* str2ptr(char* str) {
    BUFENC* pFirst = (BUFENC*)malloc(sizeof(BUFENC));
    BUFENC* pCur = pFirst;
    BUFENC* pLast = 0;
    for (int i = 0; i < strlen(str); i++) {
        char b = str[i];

        for (int j = 7; 0 <= j; j--) {
            if ((b >> j) & 0x01) {
                pCur->pElem[j] = (ELEM*)malloc(sizeof(ELEM));
                pCur->pElem[j]->value = rand() % 0xdeadbeef;
            } else {
                pCur->pElem[j] = (ELEM*)malloc(sizeof(ELEM));
                pCur->pElem[j]->value = 0xdeadbeef;
                //(ELEM*)0xdeadbeef;
            }
        }
        pCur->pNext = (BUFENC*)malloc(sizeof(BUFENC));
        pLast = pCur;
        pCur = pCur->pNext;
    }
    if (pLast) {
        free(pLast->pNext);
        pLast->pNext = 0;
        pCur = 0;
    }

    return pFirst;
}

char* ptr2str(BUFENC* pFirst) {
    BUFENC* pCur = pFirst;
    char* ret = (char*)malloc(1024);
    int n = 0;
    while (pCur) {
        char rec = 0;
        for (int j = 7; 0 <= j; j--) {
            if (pCur->pElem[j]) {
                if (pCur->pElem[j]->value != 0xdeadbeef) {
                    rec = (0x01 << j) | rec;
                }
            }
        }
        ret[n++] = rec;
        //printf("%c\n", rec);
        pCur = pCur->pNext;
    }
    ret[n] = 0;

    return ret;
}

int main() {
    BUFENC* p = str2ptr((char*)"hello baby");
    print_bufenc(p);
    printf("----------------------\n");
    printf("%s\n", ptr2str(p));
    return 1;
}