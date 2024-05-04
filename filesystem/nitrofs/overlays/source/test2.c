#include <nds.h>
#include <stdio.h>

static int numcalls;

__attribute__((constructor)) static void test2ctor(void)
{
	iprintf("[test2] Loaded\n");
}

__attribute__((destructor)) static void test2dtor(void)
{
	iprintf("[test2] Unloaded\n");
}

void test2Func(void)
{
	iprintf("[test2] Called %d times\n", ++numcalls);
}
