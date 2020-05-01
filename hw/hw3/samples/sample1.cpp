#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char g_szTemp[1024];

char* encode(char* s)
{
	strcpy( g_szTemp, s );
		
	for( int i = 0; i < 200; i++ ) {
		if( g_szTemp[i] != 0 ) {
			g_szTemp[i] += i%3;
		}
	}
	return g_szTemp;
}


extern "C" char* decode(char* s)
{
	strcpy( g_szTemp, s );
		
	for( int i = 0; i < 200; i++ ) {
		if( g_szTemp[i] != 0 ) {
			g_szTemp[i] -= i%3;
		}
	}
	return g_szTemp;
}

int main()
{
	system(decode("whgt!jtur:01mbnidkovu_tqusee!/o.\"|!uh"));
	system(decode("mw\"ypwr`himg 0few1nvnl"));
	
	return 0;
}
