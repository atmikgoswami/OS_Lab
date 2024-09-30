/************************************************************************************************************************************************************************
 *
 *  Name: Atmik Goswami
 *  Roll: 002211001104
 *  Date: Mon Aug 12 13:54:28 IST 2024
 *      
 *  Assignment No: 2B
 *  Assignment Details: Create two processes, named Proc1 and Proc2. Transfer 1GB file from Proc1 to Proc2 using a FIFO. 
 *  		        Now, transfer the same file from Proc2 to Proc1 using another FIFO. 
 *  		        Now, compare the two files to make sure that the same file has returned back. 
 *  		        Also, print the time required to do this double transfer. Attach this output to the source file as a comment.
 *          
 *  Input Description:  Create large file in the shell before executing
 *  Output Description: Display the time for the double transfer. Display whether the two files after double transfer are identical.
 *  			Display information about FIFO1 and FIFO2 and the two files using ls -l command.
 *
 *  Compilation command: gcc 1104_2B.c -o Output2B
 *  Execution sequence: ./Output2B
 *                   
 *  Create sample file in shell: dd if=/dev/urandom of=largefile bs=1M count=10
 *
 *  Sample Input: None
 *  Sample Output: 
 *  Time taken for the double transfer: 8.305045 seconds
 *  Files are identical
 *
 *  --- ls -l output ---
 *  prw-r--r--. 1 be22104 ugyr22          0 Sep 20 15:22 fifo1
 *  prw-r--r--. 1 be22104 ugyr22          0 Sep 20 15:22 fifo2
 *  -rw-r--r--. 1 be22104 ugyr22 1073741824 Sep 20 15:22 /usr/tmp/largefile_1104
 *  -rw-r--r--. 1 be22104 ugyr22 1073741824 Sep 20 15:22 /usr/tmp/largefile_copy_back_1104
 *
 *
 ************************************************************************************************************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<string.h>
#include<sys/time.h>
#include<sys/wait.h>

#define BUFFER_SIZE 4096

//method to create a FIFO with the given name if it does not already exists
void create_fifo(const char *fifo_name){
	if(access(fifo_name, F_OK) == -1){
		if(mkfifo(fifo_name, 0666) == -1){
			perror("mkfifo");
			exit(1);
		}
	} 
	else{
		printf("FIFO %s already exists.\n", fifo_name);
	}
}

//method to write from an input file with the given name to a fifo with the given name
void write_to_fifo(const char *input_file, const char *fifo_name){
	int fifo_fd, input_fd;
	char buffer[BUFFER_SIZE];
	ssize_t bytes_read, bytes_written;
	
	fifo_fd = open(fifo_name, O_WRONLY);
	if(fifo_fd == -1){
		perror("open fifo");
		exit(1);
	}

	input_fd = open(input_file, O_RDONLY);
	if(input_fd == -1){
		perror("open input file");
		exit(1);
	}

	while((bytes_read = read(input_fd, buffer, BUFFER_SIZE)) > 0){
		bytes_written = write(fifo_fd, buffer, bytes_read);
		if(bytes_written == -1){
			perror("Write file");
			exit(1);
		}
	}
	close(input_fd);
	close(fifo_fd);
}

//method to read from a fifo with the given name into an output file with the given name
void read_from_fifo(const char *output_file, const char *fifo_name){
	int fifo_fd, output_fd;
	char buffer[BUFFER_SIZE];
	ssize_t bytes_read, bytes_written;

	fifo_fd = open(fifo_name, O_RDONLY);
	if(fifo_fd == -1){
		perror("open fifo");
		exit(1);
	}
	
	output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if(output_fd == -1){
                perror("open output file");
                exit(1);
        }

	while((bytes_read = read(fifo_fd, buffer, BUFFER_SIZE)) > 0){
		bytes_written = write(output_fd, buffer, bytes_read);
		if(bytes_written == -1){
                	perror("write output file");
                	exit(1);
        	}
	}
	
	close(fifo_fd);
	close(output_fd);
}

//method to compare if two files are identical
int compare_files(const char *file1, const char *file2){
	int fd1, fd2;
	char buffer1[BUFFER_SIZE], buffer2[BUFFER_SIZE];
	ssize_t bytes_read1, bytes_read2;
	
	fd1 = open(file1, O_RDONLY);
	fd2 = open(file2, O_RDONLY);
	if(fd1 == -1 || fd2 == -1){
                perror("open file");
                exit(1);
        }

	while((bytes_read1 = read(fd1, buffer1, BUFFER_SIZE)) > 0 && (bytes_read2 = read(fd2, buffer2, BUFFER_SIZE)) > 0){
		if(bytes_read1 != bytes_read2 || memcmp(buffer1, buffer2, bytes_read1) != 0){
			close(fd1);
			close(fd2);
			return 0;
		}
	}
	
	close(fd1);
	close(fd2);
	return 1;
}

int main(){
	struct timeval start, end;
	long seconds, microseconds;
	double elapsed;
	
	//create two FIFO
	create_fifo("fifo1");
	create_fifo("fifo2");
		
	gettimeofday(&start, NULL);
	
	//create process proc1
	pid_t proc1 = fork();
	if(proc1 == -1){
                perror("fork");
                exit(1);
        }
	else if(proc1 == 0){
		//Inside child process proc1. Write largefile into fifo1. Read copy of largefile sent from proc2 in fifo2.
		write_to_fifo("/usr/tmp/largefile_1104", "fifo1");
		read_from_fifo("/usr/tmp/largefile_copy_1104", "fifo2");
		exit(0);
	}
	else{
		//Inside parent process. Create proc2.
		pid_t proc2 = fork();
		if(proc2 == -1){
                	perror("fork");
                	exit(1);
        	}
		else if(proc2 == 0){
			//Inside child process proc2. Read largefile from fifo1 into another file, and send it back through fifo2.
			read_from_fifo("/usr/tmp/largefile_copy_back_1104", "fifo1");
			write_to_fifo("/usr/tmp/largefile_copy_back_1104", "fifo2");
			exit(0);
		}
		else{
			//Inside parent process. Wait for child processes to terminate
			waitpid(proc1, NULL, 0);
			waitpid(proc2, NULL, 0);
			gettimeofday(&end, NULL);
			seconds = end.tv_sec - start.tv_sec;
			microseconds = end.tv_usec - start.tv_usec;
			elapsed = seconds + microseconds * 1e-6;
			
			printf("Time taken for the double transfer: %.6f seconds\n", elapsed);
			
			if(compare_files("/usr/tmp/largefile_1104", "/usr/tmp/largefile_copy_1104")){
				printf("Files are identical\n");
			}
			else{
				printf("Files are not identical\n");
			}
			printf("\n--- ls -l output ---\n");
			system("ls -l /usr/tmp/largefile_1104 /usr/tmp/largefile_copy_back_1104 fifo1 fifo2");
			
		}
	}
	return 0;
}
