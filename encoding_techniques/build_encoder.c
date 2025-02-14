#define MIN(x, y) ((x) < (y) ? (x) : (y)) // Taken from das.c libdasm example
struct heuristicState{
	int pushedA;
	int poppedA;
	int addedA;
	int subbedA;
};
//Perform XOR on file data
int 
xor(int modifier, int xorVal)
{
	int xor;
	xor = (modifier ^ xorVal);
	if (xor > 256)
		xor = xor - 256; // if over 265 loop round
	return xor;
}

//Fill heuristic sequence with a sequence of NoOps, Pushes and Pops
void 
heuristic(int *seq, int passes)
{
	unsigned char noop = 0x90;
	unsigned char pushA = 0x60;
	unsigned char popA = 0x61;
	int i, j, r;
	
	struct heuristicState heurS = {.pushedA = 0};
	for(i=0; i < passes; i++)
	{
		r = rand() % 3;
		if (heurS.pushedA >= (passes - i) * 0.8)// if there are as many elements to push as there are passes left (0.8 makes it so its slightly less, rather be too careful)
		{
			for(j=i; j < passes; j++)//populate the rest of passes with pops
			{
				seq[j] = popA;
				heurS.pushedA = heurS.pushedA - 1;
			}
			i = i + (j - i);
		}
		
		else if(r == 0 || i >= (passes - 1))
		{
			seq[i] = noop;
		}
		else if(r == 1)
		{
			seq[i] = pushA;
			heurS.pushedA = heurS.pushedA + 1;
		} 
		else if(r == 2 && heurS.pushedA > 0)
		{
			seq[i] = popA;
			heurS.pushedA = heurS.pushedA - 1;
		}
		else if(r == 2)
		{
			seq[i] = noop;
		}
	}
}

//Create decoded binary
void 
decode_exe(char* fileStr, int* encoder, int size)
{ 
	FILE *pFile;
	int n = 0;
	char newFileStr[300];
	
	strcpy(newFileStr, fileStr);

	strcat(newFileStr, "dec");
	pFile = fopen(newFileStr, "wb");
	for(n = 0; n < size; n++)
	{
		fwrite(&encoder[n], 1, 1, pFile);
	  	
	}
  	fclose(pFile);
    printf("%s written to memory \n", newFileStr);
}

//Creates an obfuscated binary with heuristics
void 
create_exe(char* fileStr, int* encoder, int textsize, int* heuristicSeq, int passes, 
	   int size)
{ 
	FILE *pFile;
	int n = 0;
	char newFileStr[300];
	
	strcpy(newFileStr, fileStr);
	strcat(newFileStr, "obf");
	pFile = fopen(newFileStr, "wb");
	for(n = 0; n < size; n++)
	{
		fwrite(&encoder[n], 1, 1, pFile);
	}
  	
  	for(n = 0; n < passes; n++)
  	{
  		fwrite(&heuristicSeq[n], 1, 1, pFile);
  	}
  	
  	fclose(pFile);
    printf("%s written to memory \n", newFileStr);
}

//Build encoder array
void 
buildencoder(int encode_or_decode, int size, int* offsets, 
			unsigned char *flData, char *fileStr, int *encoder, int xorVal, int *heuristicSeq, 
             int passes)
{
    int textsize = *(offsets) - *(offsets + 1);
	INSTRUCTION inst;
	int bytes = 8, instLen, c = 0, i;
	while (c < *(offsets))//This loop adapted from das.c libdasm example
	{
		instLen = get_instruction(&inst, flData + c, MODE_32);
		if (!instLen || (instLen + c > textsize)) // Illegal opcode or opcode longer than remaining buffer
			{ 
				encoder[c] = flData[c];
				c++;
				continue;
			}
		for (i = 0; i < MIN(bytes, instLen); i++)
		{
			encoder[c + i] = flData[c + i];
		}
			c+=instLen;
		}
		
	while (c < *(offsets + 1))//While c is less than the .fini offset
	{
		instLen = get_instruction(&inst, flData + c, MODE_32);
		if (!instLen || (instLen + c > textsize))
		{
			encoder[c] = xor(flData[c], xorVal);//Encode executable instructions
			c++;
			continue;
		}
		for (i = 0; i < MIN(bytes, instLen); i++)
		{
			encoder[c + i] = xor(flData[c + i], xorVal);
		}	
			c+=instLen;
	}
	
	if (passes != 0) 
		heuristic(heuristicSeq, passes);
			
	while (c < size)//Write the rest of the binary unencoded
	{
		instLen = get_instruction(&inst, flData + c, MODE_32);
		if (!instLen || (instLen + c > textsize)) 
		{
			encoder[c] = flData[c];
			c++;
			continue;
		}
		
		for (i = 0; i < MIN(bytes, instLen); i++)
		{
			encoder[c + i] = flData[c + i];
		}
		c+=instLen;
	}
	if (encode_or_decode == 2)
		decode_exe(fileStr, encoder, size);
	else
		create_exe(fileStr, encoder, textsize, heuristicSeq, passes, size);
}