#include <nds.h>
#include <stdio.h>

static const char* testvar;

__attribute__((constructor)) static void test0ctor(void)
{
	iprintf("[test0] Loaded\n");
	testvar = "This string is stored in the overlay";
}

__attribute__((destructor)) static void test0dtor(void)
{
	iprintf("[test0] Unloaded\n");
}

const char* test0Func(void)
{
	return testvar;
}
