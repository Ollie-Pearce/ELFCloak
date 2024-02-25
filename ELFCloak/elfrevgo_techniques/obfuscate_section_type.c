#include <stdio.h>
#include <stdlib.h>
//Check if these imports are needed

//Overwrites the sh_type value of all section headers to 0
void 
obfuscate_section_type(char *flData, char* fileStr)
{
	int i, position;
	Elf64_Ehdr hdr;
	FILE* pFile = fopen(fileStr, "rb");
	fread(&hdr, sizeof(hdr), 1, pFile);//read pFile to ELF header struct
	
	for (i=0; i < hdr.e_shnum; i++)//Loop while i < the number of section headers
	{
		position = (hdr.e_shoff + (i * hdr.e_shentsize + 4)); //uint32_t has size 4 so increment by 4 to skip past it
		flData[position] = 0;//Set section type to 0
	}
	fclose(pFile);
}