//gcc -fPIC -rdynamic -shared -m32 lib.c -o lib.so
#include <stdio.h> 
int foo(int input)
{
	return 1024;
}