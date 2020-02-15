//gcc -fPIC -rdynamic -shared -m32 libstdcall.c -o libstdcall.so
#include <stdio.h>
#define stdcall __attribute__((stdcall))
int stdcall foo(int input)
{
	return 1024;
}