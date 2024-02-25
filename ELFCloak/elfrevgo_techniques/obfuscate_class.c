//Flips the bit representing the EI_CLASS macro inside the e_ident field of the ELF header
void 
obfuscate_class(char *flData)
{
	if (flData[4] == 2) 
		flData[4] = 1;
	else 
		flData[4] = 2;
	
}