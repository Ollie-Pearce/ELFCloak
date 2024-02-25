//Flips the bit representing the EI_DATA macro inside the e_ident field of the ELF header
void 
obfuscate_endianness(char *flData)
{
	if (flData[5] == 2)
		flData[5] = 1;
	
	if (flData[5] == 1)
		flData[5] = 2;
}