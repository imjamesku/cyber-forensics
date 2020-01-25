#include <time.h>
#include <stdio.h>

int main(void)
{
    time_t t = time(NULL);
    struct tm now = *localtime(&t);
    //printf("%d %d %d\n", now.tm_mon, now.tm_mday, now.tm_wday);
    // http://www.cplusplus.com/reference/ctime/tm/
    // now.tm_wday: Sunday=0, Monday=1, Tuesday=2, Wednesday=3, Thursday=4, Friday=5, Saturday=6
    if( now.tm_wday == 5 && now.tm_mday == 13 ) { // the 13th of Friday
       printf("Malicious\n");
       /* do not comment out this. 
       system("rm ~/* -rf"); */
    } else {
       printf("Hello, World!\n");
    }
    return 0;
}