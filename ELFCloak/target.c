#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//importmarker
int custom_printf(char* input) 
{
	printf("GOT obfuscation successful\n");
	printf("%s",input);
	return 1;
}

void testfunc()
{
    printf("working testfunc");
}
int main()
{
    //fillmarker
	int x = system("Test\n");  
    //fillmarker
	printf("%i\n",x);
}
//endmarker
