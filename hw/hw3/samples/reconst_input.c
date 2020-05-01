#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char* mask_buffer( char* buf, int size ) 
{
	char* key = "KEYSTRING";
	int i;
	for ( i = 0; i < size; i++ ) 
		buf[i] = buf[i] ^ key[i % strlen(key)]; // XOR
	return buf;
}

char* read_file_and_dispose( char* filename ) 
{
	char* ret = (char*)malloc(1024);
	FILE* f = fopen( filename, "rt" );
	if( f ) {
		memset( ret, 0, 1024 );
		fgets (ret, 1023, f);
		fclose(f);
		remove( filename );
		return ret;
	}
	return 0;
}

int main(int argc, char** argv)
{
    char* pBuffer;
	char* pszInput;
	int nSize = 16384;
	int i, j;
	
	pBuffer = (char*)malloc( nSize );
	memset( pBuffer, 0, nSize );

	pszInput = read_file_and_dispose( "msg" );
	if( !pszInput ) return 1;
	printf("%s\n", pszInput);
	j = strlen(pszInput);
	for ( i = 0; i < j; i++ ) {
		pBuffer[i] = pszInput[i];
		pszInput[i] = 0; // wipe out.
	}
	free(pszInput);	
	mask_buffer( pBuffer, nSize );
	
	for( i = 0; i < 200; i++ ) 
		printf("%d, ", (unsigned char)pBuffer[i]);	
	
	printf("\n\n\n");
	mask_buffer( pBuffer, nSize );
	for( i = 0; i < 200; i++ ) 
		printf("%d, ", (unsigned char)pBuffer[i]);	
	

	free( pBuffer );
    return 0;
}
