
void create_runner(char* fileStr)
{
    char* ch_mod_command[100]; //hold chmod command
	char* exec_command[100]; //hold execution command

    strcpy(ch_mod_command, "chmod +x ");
	strcat(ch_mod_command, fileStr);
	strcat(ch_mod_command, "new");

	strcpy(exec_command, "./");
	strcat(exec_command, fileStr);
	strcat(exec_command, "new");

    
    char prog_output_str[500];
    int sys_return = system(ch_mod_command);

    FILE *exec_output;
    exec_output = popen(exec_command, "r");

    while (fgets(prog_output_str, sizeof(prog_output_str), exec_output) != NULL){//Retrieve output of command run
        printf("%s", prog_output_str);
	};
    fclose(exec_output);
}