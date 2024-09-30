/***********************************************************************
 *
 * Name: Atmik Goswami
 * Roll: 002211001104
 * Date: Mon Aug  5 13:38:15 IST 2024
 *
 * Assignment No: 2A
 * Assignment Details: Catch the signal 'SIGINT' and display "Ha Ha, Not Stopping". Use 'signal' system call. 
 * 		       Always use "perror" to check the return status of a library/system call. 
 * 
 * Input Description: None
 * Output Description: 
 * Press Cntrl + C to send SIGINT signals to the process, which will be caught and handled, not terminating the execution.
 * Press Cntrl + Z to exit.
 * 
 * 
 * Compilation command: gcc 1104_2A.c -o Output2A
 * Execution sequence: ./Output2A
 * 
 * 
 * Sample Input: None
 * Sample Output: 
 * ^CHa Ha, Not Stopping
 * ^CHa Ha, Not Stopping
 * ^CHa Ha, Not Stopping
 * ^CHa Ha, Not Stopping
 * ^CHa Ha, Not Stopping
 * ^Z
 * [6]+  Stopped                 ./Output2A
 *
 * ***********************************************************************/

#include<stdio.h>
#include<signal.h>
#include<errno.h>
#include<unistd.h>

void signal_handler(int signo){
	
	if(signo == SIGINT){
		printf("Ha Ha, Not Stopping\n");
	}
}

int main(){
	if(signal(SIGINT, signal_handler) == SIG_ERR){
		perror("Cannot catch SIGINT\n");
	}
	while(1){
		sleep(1);
	}
	return 0;
}
