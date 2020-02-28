#include <stdio.h>
#include <stdlib.h>

int main() {
    int64_t addr;
    unsigned char *cptr = NULL;
    while (1) {
        scanf("%lx", &addr);
        printf("%lx\n", addr);
        cptr = (unsigned char *)&addr;
        for (size_t i = 0; i < 8; i++) {
            printf("%x ", *(cptr + i));
        }
        printf("\n");
    }
    return 0;
}