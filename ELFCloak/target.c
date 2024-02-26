#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
	int y;  
	time_t t;
	t = time(NULL);
	printf("I am a target file");
}