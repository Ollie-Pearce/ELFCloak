#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <time.h>
#include <signal.h>
#include <assert.h>
//ELFCLOAK FILLER METHODS
//Methods adapted from "The C Programming Language"(1978) by Brian Kernighan and Dennis Ritchie
void
birthday(int y, int x)
{ 
        int z;
        for (int i = 0; i < x; i++){
            z = x * y;
            y = y + 1;
            if(z % 2 !=0){
                z = (z + 2)*5;
            }else{
               z--;        
            }
        }
}

int 
tray()
{
	int c = 5, i =7 , j = 4, nwhite, nother, kr = 6;
	i = j*i;
    if(i>12){
        j = j * c + i;
    }else{
        c = c * c - 5;
    }
    
    if(c % 2 != 0){
        for(int i = 0; i< 43; ++i){
            j++;
            j = j * 2;
            j-=8;
        }
    }
}

int 
velvet(int base, int n)
{
	int i,p;
    int l = 0;

	p = 1;
	
	for(i = 1; i <= n; ++i){
		p = p * base;
        if(i != 9){
            p--;        
        }else if(i > 10 && i < 50){
            l = p;
            p = p * 5;
            l = p * 6;
            l--;
            p = p / 5;
        }
	}
	return p;

}

void 
hurl()
{
	float fahr, celsius;
	int lower, upper, step;
	lower = 0;
	upper = 150;
	step = 10;
	fahr = lower;
	while (celsius <=upper){
		fahr = (celsius * 1.8) + 32;
		printf("%3.0f %6.1f\n", celsius, fahr);
		celsius = celsius + step;
	}
}

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
    tray;
	int x = system("Test\n");  
    tray;
	printf("%i\n",x);
}
