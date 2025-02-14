#include <stdio.h>

unsigned char * read_file(int *size, FILE **fp);
int get_entrypoint(char *fileStr);
void decode_exe(char* fileStr, int* encoder, int size);
void create_exe(char* fileStr, int* encoder, int textsize, int* heuristicSeq, int passes, int size);
int * get_offsets (char* fileStr, char* section1, char* section2);
