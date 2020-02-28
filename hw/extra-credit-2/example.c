#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *mask_buffer(char *buf, int size) {
  char *key = "KEYSTRING";
  int i;
  for (i = 0; i < size; i++)
    buf[i] = buf[i] ^ key[i % strlen(key)]; // XOR
  return buf;
}

char *read_file_and_dispose(char *filename) {
  char *ret = (char *)malloc(1024);
  FILE *f = fopen(filename, "rt");
  if (f) {
    memset(ret, 0, 1024);
    fgets(ret, 1023, f);
    fclose(f);
    remove(filename);
    return ret;
  }
  return 0;
}

int main(int argc, char **argv) {
  char *pBuffer;
  char *pszInput;
  int nSize = 16384;
  int i, j;

  pBuffer = (char *)malloc(nSize);
  memset(pBuffer, 0, nSize);

  /* step 1 */
  pszInput = read_file_and_dispose("msg");
  if (!pszInput)
    return 1;
  j = strlen(pszInput);
  for (i = 0; i < j; i++) {
    pBuffer[i] = pszInput[i];
    /* step 2 */
    pszInput[i] = 0; // wipe out.
  }
  free(pszInput);
  /* step 3 */
  mask_buffer(pBuffer, nSize);

  /* ======== MEMORY OBTAINED AT THIS VERY MOMENT ======== */

  /* step 4: send secret buffer */
  free(pBuffer);
  return 0;
}