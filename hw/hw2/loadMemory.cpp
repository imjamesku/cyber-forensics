#include <stdio.h>
#include <stdlib.h>
// #define DEBUG
#ifdef DEBUG
#define D(x) x
#else
#define D(x)
#endif

typedef struct tag_ELEM {
  int value;
} ELEM;
typedef struct tag_BUFENC {
  ELEM *pElem[8];
  struct tag_BUFENC *pNext;
} BUFENC;

char *ptr2str(BUFENC *pFirst) {
  BUFENC *pCur = pFirst;
  char *ret = (char *)malloc(1024);
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
    // printf("%c\n", rec);
    pCur = pCur->pNext;
  }
  ret[n] = 0;

  return ret;
}
void printMemory(char *buffer, long fsize, int64_t base) {
  for (size_t i = 0; i < fsize; i++) {
    if (i % 8 == 0) {
      printf("%lx:", base + i);
    }
    printf("%x ", (unsigned char)buffer[i]);
    if (i % 8 == 7) {
      printf("\n");
    }
  }
}
int main(int argc, char *argv[]) {
  if (argc == 1) {
    printf("Please enter the path to 2824-7fffbe15c000-7fffbe17d000.dump as an "
           "argument");
    return 0;
  }
  FILE *f = fopen(argv[1], "rb");
  fseek(f, 0, SEEK_END);
  long fsize = ftell(f);
  printf("size: %ld\n", fsize);
  fseek(f, 0, SEEK_SET); /* same as rewind(f); */

  char *buffer = (char *)malloc(fsize + 1);
  fread(buffer, 1, fsize, f);
  fclose(f);

  D(printMemory(buffer, fsize, 0x7fffbe15c000);)

  printf("buffer:%lx\n", buffer);
  printf("buffer+fsize:%lx\n", buffer + fsize);

  u_int64_t head = (u_int64_t)(buffer + (0x7fffbe15eaf0 - 0x7fffbe15c000));
  int64_t *cur = (int64_t *)head;
  size_t byteCount = 0;
  while (cur != NULL) {
    for (size_t i = 0; i < 8; i++) {
      D(printf("changing addr:%lx\n", cur);)
      D(printf("original value:%lx\n", *cur);)
      *cur = *cur - 0x7fffbe15c000 + (int64_t)buffer;
      D(printf("new value:%lx\n\n", *cur);)

      cur++;
    }
    D(printf("changing addr:%lx\n", cur);)
    D(printf("original value:%lx\n", *cur);)

    byteCount++;

    if (*cur != 0) {
      *cur = *cur - 0x7fffbe15c000 + (int64_t)buffer;
      cur = (int64_t *)*cur;
      D(printf("new value:%lx\n\n-----------------------------\n", *cur);)
      fflush(stdout);
    } else {
      break;
    }
  }

  printf("total number of bytes: %ld\n", byteCount);

  D(printMemory(buffer, fsize, (int64_t)buffer);)

  BUFENC *pFirst = (BUFENC *)head;
  char *secretText = ptr2str(pFirst);
  printf("secret text: %s", secretText);

  return 0;
}
