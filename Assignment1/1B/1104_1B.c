/***********************************************************************
 * 
 *  Name: Atmik Goswami
 *  Roll: 002211001104
 *  Date: Mon Aug  5 13:03:41 IST 2024 
 * 
 *  Assignment No: 1B
 *  Assignment Details: Program to GET and SET the Environment variable and to know use of getenv and setenv system calls
 *
 *  Input Description: None 
 *  Output Description: Displays the SYSTEM environment variables USER, HOME, HOST, ARCH, DISPLAY, PRINTER, PATH and two custom environment variables
 *  			VARIABLE_1 and VARIABLE_2
 * 
 *  Compilation command: gcc 1104_1B.c -o Output1B
 *  Execution sequence: ./Output1B
 * 
 *  Sample Input: None
 *  Sample Output: 
 *  USER = be22104
 *  HOME = /home/usr/student/ug/yr22/be22104
 *  HOST is not set in the environment
 *  ARCH is not set in the environment
 *  DISPLAY is not set in the environment
 *  PRINTER is not set in the environment
 *  PATH = /usr/lib64/qt-3.3/bin:/usr/local/bin:/usr/bin:/usr/local/sbin:/usr/sbin:/home/usr/student/
 *  	   ug/yr22/be22104/.local/bin:/home/usr/student/ug/yr22/be22104/bin
 *  VARIABLE_1 = VALUE_1
 *  VARIABLE_2 = VALUE_2
 * 
 * ***********************************************************************/

#include<stdio.h>
#include<stdlib.h>

void print_env_variable(const char* var_name){
	char* value = getenv(var_name);
	if(value){
		printf("%s = %s\n", var_name, value);
	}
	else{
		printf("%s is not set in the environment\n", var_name);
	}
}

int main(){
	print_env_variable("USER");
	print_env_variable("HOME");
	print_env_variable("HOST");
	print_env_variable("ARCH");
	print_env_variable("DISPLAY");
	print_env_variable("PRINTER");
	print_env_variable("PATH");

	if(setenv("VARIABLE_1", "VALUE_1",1) != 0){
		printf("Error setting VARIABLE_1\n");	
	}
	if(setenv("VARIABLE_2", "VALUE_2",1) != 0){
		printf("Error settig VARIABLE_2\n");
	}

	print_env_variable("VARIABLE_1");
	print_env_variable("VARIABLE_2");

	return 0;
}	
