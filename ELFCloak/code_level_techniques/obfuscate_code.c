#include <stdlib.h>
#include <time.h>


//Counts the number of methods the user has input
int
count_methods(char *methods_input){
    int count = 0;
    for(int i = 0; i < strlen(methods_input); i++){
        if (methods_input[i] == ';'){
            count++;
        }
    }
    count++;
    return count;
}

//Counts the number of characters inside a string of fake imports
int
count_chars(char *string, int fake_import_count){
    int length = 0;
    for(int i = 0; i < fake_import_count; i++){
        for (int j = 0; j < 22; j++){
            if(string[i * 22 + j] != '\0'){
                length++;
            }
        }
    }
    return length;
}

//Splits fake imports into a new_input pointer
void
split_input(char*** new_input, char *methods_input, int count, char separator[]){
    char* token;
    int i = 0;
    token = strtok(methods_input, separator);

    while(token != NULL){
        *(*(new_input)+i) = strdup(token);
        i++;
        token = strtok(NULL, separator);
        }
}

//Return a sequence of random length containing a random selection of fake #includes statements
char *
generate_imports(int fake_import_count)
{
    char *fake_imports = malloc((fake_import_count * 22) * sizeof(char));
    int new_rand;
    srand(time(NULL));
    char fake_includes_template[7][22] = {"#include <sys/timeb.h>", "#include <time.h>", "#include <math.h>", "#include <signal.h>", "#include <stdarg.h>", "#include <limits.h>", "#include <assert.h>"};
    

    printf("Inserting %d fake imports\n", fake_import_count);
    for(int i = 0; i < fake_import_count; i++){
        new_rand = rand() % 7;
        for(int j = 0; j < 22; j++){
            fake_imports[i * 22 + j] = fake_includes_template[new_rand][j];
        }
    }
    return fake_imports;
}

//Randomly generates calls to filler functions
void
generate_filler_calls(char *flData, int *size, char*** fillerStr, char *filler_calls, int count)
{
    int filler_rand;
    srand(time(NULL));
    char *pos = flData;
    while((pos = strstr(pos,"//fillmarker"))!= NULL){
        filler_rand = rand() % count;
        strcat(filler_calls, ((*fillerStr)[filler_rand]));
        strcat(filler_calls, ";");
        pos++;
    }
}
//Inserts fake imports at the position of the //importmarker in new file data. Writes //importmarker to file so insert_filler_methods can find a position to insert methods.
char *
insert_imports(char *flData, char *newflData, char *fileStr,int target_size, int fake_import_count, char *fake_imports, char *data_holder)
{

    char *pos; //This pointer will hold the location of the first instance of the insert marker

	for (int i = 0; i < (target_size / sizeof(char)); i++){
		newflData[i] = flData[i];

	}
    if ((pos = strstr(newflData, "//importmarker")) == NULL){
        printf("Could not locate //importmarker in file. Exiting \n");
        exit(0);
    }
    strcpy(data_holder, pos);

    for(int i = 0; i < fake_import_count; i++){
        strncpy(pos, "\n", 1);
        pos = pos + 1;
        for (int j = 0; j < 22; j++){
            if(fake_imports[i * 22 + j] != '\0'){
                strncpy(pos, &(fake_imports[i * 22 + j]), 1);
                pos = pos + 1;
            }
        }
    }
    strncpy(pos, "\n", 1);
    pos = pos + 1;

    strcpy(pos, data_holder);
	return newflData;
}

//Inserts filler methods at the position of \\importmarker in new file data
char *
insert_filler_methods(char *newflData, char *flDataFiller, int *sizeFiller, char *data_holder)
{
    
    char *pos; 

    if ((pos = strstr(newflData, "//importmarker")) == NULL){
        printf("Could not locate //importmarker in file. Exiting \n");
        exit(0);
    }
    strcpy(data_holder, pos + 14);

    for(int i = 0; i < *sizeFiller / sizeof(char); i++){
        strncpy(pos, &(flDataFiller[i]), 1);
        pos = pos + 1;
    }

    strcpy(pos, data_holder);
	return newflData;
}

//Inserts an array of filler calls into //fillmarkers in source code
char *
insert_filler_methods_call(char *newflData,char*** fillerStr, int count, char *data_holder)
{
    int filler_rand;
    srand(time(NULL));
    char *pos = newflData;

    while((pos = strstr(pos,"//fillmarker"))!= NULL){
        
        strcpy(data_holder, pos + 12);
        filler_rand = rand() % count;

        for(int i = 0; i < strlen((*fillerStr)[filler_rand]); i++){
            strncpy(pos, &((*fillerStr)[filler_rand][i]), 1);
            pos = pos + 1;
        }
        strncpy(pos, ";", 1);
        pos = pos + 1;
        strcpy(pos, data_holder);
    }
	return newflData;
}

//Writes an obfuscated source code file to disk
void
write_source_file(char *newflData,char *fileStr,int size)
{
	FILE *pFile;
	char newFileStr[300] = {'\0'};
    strncpy(newFileStr, fileStr, (strlen(fileStr) - 2));

	strcat(newFileStr, "obf.c");

    printf("Writing obfuscated source code to %s \n", newFileStr);
	pFile = fopen(newFileStr, "w");
	for(int n = 0; n < (size *1.2); n++)//Added a buffer here as sometimes elfcloak miscalculates size
	{
        if(newflData[n] != NULL){
                        if(newflData[n] == '/' && newflData[n + 1] == '/' && newflData[n + 2] == 'e' && newflData[n + 3] == 'n' && newflData[n + 4] == 'd' && newflData[n + 5] == 'm' && newflData[n + 6] == 'a' && newflData[n + 7] == 'r'){//If endmarker is found stop writing.
                break;
            }

		    fwrite(&newflData[n], 1, 1, pFile);
        }

	}
	fclose(pFile);
}

//Inserts fake imports, filler methods and their associated calls into a source code file
void 
obfuscate_code(char *flData, int target_size, char *fileStr, char *data_holder)
{
    char *flDataFiller;
    FILE *pFile;
    int sizeFiller, size, num_of_methods_input, valid_methods = 0;
    srand(time(NULL));
    int fake_import_count = (rand() % 6) + 1;
    int fake_import_size;
    char methods_input[256];
    char filler_calls[256];

    while (valid_methods == 0){
        printf("Name methods to be inserted into target file. Seperated by semicolons \n");
        scanf("%s", methods_input);
        if(methods_input[strlen(methods_input) - 1] == ';'){
            printf("Invalid input, the final method call does not require a semicolon \n");
        }
		else
		{
			valid_methods = 1;
		}
	}
    num_of_methods_input = count_methods(methods_input); 
    char **input_array = malloc(num_of_methods_input * sizeof(char *));
    split_input(&input_array, methods_input, num_of_methods_input, ";");
    
    pFile = fopen("filler_methods.c", "r");
    if (pFile == NULL){
			printf("Error opening file %s, save a filler_methods.c file in the ELFCloak Directory\n", fileStr);
            exit(0);
	}

    fseek(pFile, 0L, SEEK_END);
  	sizeFiller = ftell(pFile);
	rewind(pFile);
    flDataFiller = read_file(&sizeFiller, pFile); //Leave this in main for now... could have its own file
    char *fake_imports = generate_imports(fake_import_count);
    fake_import_size = count_chars(fake_imports, fake_import_count);
    
    generate_filler_calls(flData, &size, &input_array, filler_calls, num_of_methods_input);

    size = target_size + (fake_import_size + sizeFiller + strlen(filler_calls));
    int to_read = (size / sizeof(char));

    char * newflData = (char *)malloc (size);
	newflData = insert_imports(flData, newflData, fileStr, target_size, fake_import_count, fake_imports, data_holder);
    newflData = insert_filler_methods(newflData, flDataFiller, &sizeFiller, data_holder);

    char **randomised_input_array = malloc((num_of_methods_input+ sizeFiller) * sizeof(char *));

    for (int i = 0; i < num_of_methods_input; i++){
        free(*(input_array + i));
    }
    free(input_array);
    

    split_input(&randomised_input_array, filler_calls, num_of_methods_input, ";");

    newflData = insert_filler_methods_call(newflData, &randomised_input_array, num_of_methods_input, data_holder);
    
    
	write_source_file(newflData, fileStr, to_read);

    for (int i = 0; i < num_of_methods_input; i++){
        free(*(randomised_input_array + i));
    }
    free(fake_imports);
    free(flDataFiller);
    free(randomised_input_array);
    free(newflData);
}