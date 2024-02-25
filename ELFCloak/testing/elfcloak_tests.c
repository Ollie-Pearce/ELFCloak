#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "elfcloak.h"
#include <stdlib.h>


char lorem[] = {"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nullam interdum id augue vel efficitur. Ut vulputate molestie mi, id volutpat nunc fringilla sed. Vestibulum at ultricies dolor, eu tincidunt risus. Mauris varius varius tempor. Suspendisse bibendum arcu id iaculis pulvinar. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Donec ut metus lectus. "};

void
TESTwrite_source_file(){
    write_source_file(lorem , "lorem_ipsum" ,strlen(lorem));
    int sys_return = system("find -name lorem_ipsobf.c | grep .");
    assert(sys_return == 0);
}

void 
TESTxor(){
    assert(xor(5, 9) == 12);
}
void
TESTobfuscate_section_type(unsigned char *flData){
    obfuscate_section_type(flData, "target");
    
}
void
TESTobfuscate_class(unsigned char *flData)
{
    int old_class = flData[4];
    obfuscate_class(flData);
    assert(old_class != flData[4]);
    obfuscate_class(flData);
}

void
TESTobfuscate_endianness(unsigned char *flData){
    int old_endianness = flData[5];


    obfuscate_endianness(flData);

    assert(old_endianness != flData[5]);
    obfuscate_endianness(flData);
}

void
TESTpasses()
{
    assert((get_passes()) < 106);
}

void
TESTcount_methods(){
    assert(count_methods("test1;test2;test3")==3);
}

void
test_main()
{
    FILE *pTestFile;
    int size;
    
    pTestFile = fopen("target", "rb");
    fseek(pTestFile, 0L, SEEK_END);
  	size = ftell(pTestFile);
	rewind(pTestFile);
    
    unsigned char *flData = read_file(&size, pTestFile);
    
    TESTobfuscate_class(flData);
    TESTobfuscate_endianness(flData);
    TESTobfuscate_section_type(flData);
    TESTwrite_source_file();
    TESTxor();
    TESTpasses();
    TESTcount_methods();
    printf("tests passed \n");
    exit(0);
}

