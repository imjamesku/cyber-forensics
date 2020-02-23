//gcc -o main main.c -ldl -m32
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>

typedef int (*FUNCPTR)(int);

int main()
{
	void *handle;
	char *error;
	int i, r;

	handle = dlopen ("./libstdcall.so", RTLD_NOW);
	if (!handle) {
		fputs (dlerror(), stderr);
		exit(1);
	}

	FUNCPTR fp = dlsym(handle, "foo");
	if ((error = dlerror()) != NULL)  {
		fputs(error, stderr);
		exit(1);
	}

	r = fp(2);
	r = fp(2);
	r = fp(2);
	r = fp(2);
	r = fp(2);
	r = fp(2);
	r = fp(2);
	r = fp(2);
	printf ("%d\n", r);
	for( i = 0; i < 20; i++ ) {
		r += r * 1024;
	}
	
	for( i = 0; i < 20; i++ ) {
		printf(".....");
	}
	printf("\n");
	
	dlclose(handle);
	
	return 0;
}