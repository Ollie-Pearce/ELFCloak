//Fetches the virtual address of function to replace from the .rela.plt section

int
get_lib_func_add(char* fileStr, char*  lib_func)
{
    FILE *pOutput;
	char command[23 + sizeof(fileStr)]; //hold command
    int func_add;
    char* relocStr[5];
	char outputStr[100];
	int i = 0;

    strcpy(command, "readelf -r ");
	strcat(command, fileStr);
	strcat(command, " | grep ");
    strcat(command, lib_func);

    pOutput = popen(command, "r");
	fgets(outputStr, sizeof(outputStr), pOutput);//place output of command in outputStr
    char *pStrtok = strtok(outputStr, " ");
	
	while (pStrtok != NULL)
	{
		relocStr[i] = pStrtok;
		pStrtok = strtok(NULL, " ");	
		i = i + 1;
	}
    func_add = (int)strtol(relocStr[0], NULL, 16);
    return (func_add);
}

//Calculates the physical offset of the function to replace by subtracting the address of the .got.plt. from the function's 'virtual address and adding the physical offset of the .got.plt to the result
int 
virtual_to_physical(char* fileStr, int func_dyn_add)
{
    FILE *pOutput;
	char* command[23 + sizeof(fileStr)]; //hold command
    int offsets[2];
    int section_offset;
    char* gotStr[5];
	char outputStr[100];
    int phys_add;
	int i = 0;

    strcpy(command, "readelf -S ");
	strcat(command, fileStr);
	strcat(command, " | grep .got.plt");

    pOutput = popen(command, "r");
	fgets(outputStr, sizeof(outputStr), pOutput);//place output of command in outputStr
    char *pStrtok = strtok(outputStr, " ");
	
	while (pStrtok != NULL)
	{
		gotStr[i] = pStrtok;
		pStrtok = strtok(NULL, " ");	
		i = i + 1;
	}
    offsets[0] = (int)strtol(gotStr[3], NULL, 16);//Virtual address of a section in memory 
    offsets[1] = (int)strtol(gotStr[4], NULL, 16);//Physical offset of Section in the file

    section_offset = func_dyn_add - offsets[0];
    phys_add = offsets[1] + section_offset;
    return phys_add;
}

//Returns an integer representation of a 2 character long hexidecimal string
int
convert_to_denary(unsigned char *hexToConvert)
{
    unsigned char * ptr;
    int hex_output;
    hex_output = strtol(hexToConvert, &ptr, 16);
    return hex_output;
}


//Find the value of the user defined function in the Symbol table
int
get_usr_addr(char* fileStr, unsigned char* flData, char* funcName)
{
	int i = 1, symHdr_found = 0, entry_found = 0;
    int* offsets;
    FILE* pFile = fopen(fileStr, "rb");
    Elf64_Ehdr hdr;
    Elf64_Phdr phdr;
    Elf64_Shdr symHdr;
    Elf64_Sym symTab;
    int position;

    fread(&hdr, sizeof(hdr), 1, pFile);//read pFile to ELF header struct

      while(fread(&phdr, sizeof(phdr), 1, pFile) > 0 && i < (hdr.e_phnum)){//read program headers
        i++;
    }

    fseek(pFile, hdr.e_shoff, SEEK_SET);
    i=1;
    while(fread(&symHdr, sizeof(symHdr), 1, pFile) > 0 && i < (hdr.e_shnum)){//read Section headers until symbol table is reached
        if(symHdr.sh_type == 2){
            symHdr_found = 1;
            break;       
        }
        i++;
    }
    if(symHdr_found != 1){
        printf("Could not find symbol table header. Exiting");
         exit(0);
    }
    offsets = get_offsets(fileStr, ".strtab", ".shstrtab");

    unsigned char *strTabData = (unsigned char *)malloc(offsets[1] - offsets[0]);//Allocate the size of string table to strTabData
    for (i=0; i < offsets[1] - offsets[0]; i++)//read from string table
	{
		position = (offsets[0] + i);
		strTabData[i] = flData[position];
        if (strTabData[i] == '\0'){
            strTabData[i] = ' ';
        }
	}

    unsigned char *func_index = strstr(strTabData, funcName);//Get index of custom function in string table
    fseek(pFile, symHdr.sh_offset, SEEK_SET);//Move file pointer to symbol table

    i = 0;
    while(fread(&symTab, sizeof(symTab), 1, pFile) > 0 && i < symHdr.sh_size / sizeof(Elf64_Sym)){//read symbol table entries until the custom function is found
        if(symTab.st_name == func_index - strTabData){
            entry_found = 1;
            break;
        }
        i++;
    }
    if (entry_found == 0){
        printf("Symbol could not be found in string table, make sure the function you have input is present in target file \n");
        exit(0);
    }
    printf("Symbol table value of user defined function: %lx \n", symTab.st_value);

    free(offsets);
    fclose(pFile);
    return(symTab.st_value);
}

//Overwrites the physical address of a shared library function with the virtual address of a user defined function
void 
obfuscate_got(unsigned char *flData, char* fileStr)
{
    int validFunction = 0;
    char usr_func[256], library_func[256];
    int lib_vir_add;
    int lib_phys_add;
	FILE* pFile = fopen(fileStr, "rb");
    unsigned char hex_str[16];

    while(validFunction == 0){
        printf("Enter a library function to replace with a user defined function \n");
        scanf("%s", library_func);
        lib_vir_add = get_lib_func_add(fileStr, library_func);//Get virtual address of library function from 
        if(lib_vir_add != 0){
            lib_phys_add = virtual_to_physical(fileStr, lib_vir_add);//Calulate phyical address of library function
            validFunction = 1;
        }else{
            printf("Could not find function offset, check given input and PIE status of executable \n");
        }
    }
    printf("Physical address of library function: %x \n", lib_phys_add);

    printf("Enter a user defined function to be executed in place of the Library function \n");
    scanf("%s", usr_func);

    int usr_addr;
    usr_addr = get_usr_addr(fileStr, flData, usr_func);//Get the symbol table value for the user defined fucntion
	
    sprintf(hex_str, "%x", usr_addr);

    int hex_str_size = strlen(hex_str) /2;

    unsigned char hex_str_array[hex_str_size][3];

    for(int i = 0; i < hex_str_size * 2; i = i + 2){
        hex_str_array[i][0] = hex_str[i];
        hex_str_array[i][1] = hex_str[i + 1];
    }
    
    unsigned char hex_str_littleendian[hex_str_size][3];
    for(int i = 0; i <= hex_str_size + 1; i = i + 2){
        hex_str_littleendian[i][0] = hex_str_array[hex_str_size + 1 - i][0];
        hex_str_littleendian[i][1] = hex_str_array[hex_str_size + 1 - i][1];
    }

    int j = 0;
    unsigned char hex_for_insertion1[100];
    unsigned char hex_for_insertion2[100];

    memcpy(hex_for_insertion1 + 1, hex_for_insertion2, 1);

    int denary_to_insert;
    for(int i = 0; i <= hex_str_size + 1; i = i + 2){
        hex_for_insertion1[0] = hex_str_littleendian[i][0];
        hex_for_insertion2[0] = hex_str_littleendian[i][1];
        memcpy(hex_for_insertion1 + 1, hex_for_insertion2, 1);
        
        denary_to_insert = convert_to_denary(hex_for_insertion1);
        flData[lib_phys_add + j] = denary_to_insert;//Write the virtual address of the user defined function in the Symbol table to the physical address of the library function
        hex_for_insertion1[0] = 0;
        j = j +1;
    }
    fclose(pFile);
}                     