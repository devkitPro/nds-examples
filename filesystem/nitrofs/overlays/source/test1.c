#include <nds.h>
#include <stdio.h>

int var;

void test1FuncInItcm(void)
{
	iprintf("[test1] Hello ITCM %p\n", &var);
}
