/***********************************************************************
 * 
 *  Name: Atmik Goswami
 *  Roll: 002211001104
 *  Date: Mon Sep  2 14:09:43 IST 2024
 *   
 *  Assignment No: 3
 *  Assignment Details: Create two processes and transfer 1GB file from parent to child using a Socket. 
 *  			Now, transfer the same file from child to parent using the same Socket. 
 *			Now, the parent process should compare the two files to make sure that the same file has returned back. 
 *			Print the time required to do this double transfer. Attach this output to the source file as a comment.
 *          
 *  Input Description: None
 *  Output Description: Display the time for the double transfer. Display whether the two files after double transfer are identical.
 *                      Display information about the sockets created and the two files using ls -l command. 
 * 
 *  Compilation command: gcc 1104_3A.c -o Output3A
 *  Execution sequence: ./Output3A
 *                   
 *                    
 *  Sample Input: None
 *  Sample Output:
 *  Socket Type: SOCK_STREAM
 *  Protocol: 6
 *  SO_REUSEADDR: Disabled
 *  Time taken for the double transfer: 13.046197 seconds
 *  Files are identical
 *
 *  --- ls -l output ---
 *  -rw-r--r--. 1 be22104 ugyr22 1073741824 Sep 20 15:22 /usr/tmp/largefile_1104
 *  -rw-r--r--. 1 be22104 ugyr22 1073741824 Sep 20 15:44 /usr/tmp/largefile_copy_1104
 *  -rw-r--r--. 1 be22104 ugyr22 1073741824 Sep 20 15:44 /usr/tmp/received_file_1104 
 *  
 *
 ************************************************************************/


#include<arpa/inet.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<sys/time.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>

#define BUFFER_SIZE 4096

//function to compare the contents of two files
int compare_files(const char *file1, const char *file2){
        int fd1, fd2;
        char buffer1[BUFFER_SIZE], buffer2[BUFFER_SIZE];
        ssize_t bytes_read1, bytes_read2;

        fd1 = open(file1, O_RDONLY);
        fd2 = open(file2, O_RDONLY);
        if(fd1 == -1 || fd2 == -1){
                perror("Open file");
                exit(1);
        }

        while((bytes_read1 = read(fd1, buffer1, BUFFER_SIZE)) > 0
                        && (bytes_read2 = read(fd2, buffer2, BUFFER_SIZE)) > 0){

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

void print_socket_info(int sockfd) {
    int optval;
    socklen_t optlen;

    optlen = sizeof(optval);
    if (getsockopt(sockfd, SOL_SOCKET, SO_TYPE, &optval, &optlen) < 0) {
        perror("getsockopt SO_TYPE");
        return;
    }
    printf("Socket Type: %s\n", (optval == SOCK_STREAM) ? "SOCK_STREAM" : "SOCK_DGRAM");

    if (getsockopt(sockfd, SOL_SOCKET, SO_PROTOCOL, &optval, &optlen) < 0) {
        perror("getsockopt SO_PROTOCOL");
        return;
    }
    printf("Protocol: %d\n", optval);

    optlen = sizeof(optval);
    if (getsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, &optlen) < 0) {
        perror("getsockopt SO_REUSEADDR");
        return;
    }
    printf("SO_REUSEADDR: %s\n", (optval ? "Enabled" : "Disabled"));
}

int main(){
        struct timeval start, end;
        long seconds, microseconds;
        double elapsed;
        gettimeofday(&start, NULL);

	//create child process
        pid_t childproc = fork();

        if(childproc == -1){
                perror("fork");
                exit(1);
        }
        else if(childproc == 0){
		//inside child process
                int s, bytes, fd;
                char buf[BUFFER_SIZE];
                struct sockaddr_in channel;

                memset(&channel, 0, sizeof(channel));
                channel.sin_family = AF_INET;
                channel.sin_addr.s_addr = inet_addr("127.0.0.1");
                channel.sin_port = htons(5000);

                s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                if (s < 0) {
                        perror("socket failed");
                        exit(1);
                }

                sleep(1);//TO SYNCHRONIZE SERVER AND CLIENT PROCESSES
               	int flag = -1;
                while(flag = connect(s, (struct sockaddr *)&channel, sizeof(channel)) < 0){
                	perror("Connect failed");
                }

                fd = open("/usr/tmp/received_file_1104", O_WRONLY | O_CREAT | O_TRUNC, 0666);
                if (fd < 0) {
                        perror("open failed");
                        exit(1);
                }

                while ((bytes = read(s, buf, BUFFER_SIZE)) > 0) {
                        write(fd, buf, bytes);
                }
                close(fd);
                shutdown(s, SHUT_RD);

                fd = open("/usr/tmp/received_file_1104", O_RDONLY);
                if (fd < 0) {
                        perror("open failed");
                        exit(1);
                }

                while ((bytes = read(fd, buf, BUFFER_SIZE)) > 0) {
                        write(s, buf, bytes);
                }
                close(fd);
                shutdown(s, SHUT_WR);
                close(s);
        }
        else{
                int s, b, l, fd, sa, bytes, on = 1;
                char buf[BUFFER_SIZE];
                struct sockaddr_in channel;

                memset(&channel, 0, sizeof(channel));
                channel.sin_family = AF_INET;
                channel.sin_addr.s_addr = htonl(INADDR_ANY);
                channel.sin_port = htons(5000);

                s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		print_socket_info(s);
                if (s < 0) {
                        perror("socket failed");
                       exit(1);
                }

                if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) < 0) {
                        perror("setsockopt failed");
                        exit(1);
                }

                b = bind(s, (struct sockaddr *)&channel, sizeof(channel));
                if (b < 0) {
                        perror("bind failed");
                        exit(1);
                }

                l = listen(s, 1);
                if (l < 0) {
                        perror("listen failed");
                        exit(1);
                }

                sa = accept(s, 0, 0);
                if (sa < 0) {
                        perror("accept failed");
                        exit(1);
                }

                fd = open("/usr/tmp/largefile_1104", O_RDONLY);
                while ((bytes = read(fd, buf, BUFFER_SIZE)) >0) {
                        write(sa, buf, bytes);
                }
                close(fd);
                shutdown(sa, SHUT_WR);

                fd = open("/usr/tmp/largefile_copy_1104", O_WRONLY | O_CREAT | O_TRUNC, 0666);
                while ((bytes = read(sa, buf, BUFFER_SIZE)) >0) {
                        write(fd, buf, bytes);
                }
                close(fd);
                close(sa);
                close(s);

                waitpid(childproc, NULL, 0);

                gettimeofday(&end, NULL);
                seconds = end.tv_sec - start.tv_sec;
                microseconds = end.tv_usec - start.tv_usec;
                elapsed = seconds + microseconds * 1e-6;

                printf("Time taken for the double transfer: %.6f seconds\n", elapsed);

                if (compare_files("/usr/tmp/largefile_1104", "/usr/tmp/largefile_copy_1104")) {
                        printf("Files are identical\n");
                }
                else {
                        printf("Files are not identical\n");
                }
                printf("\n--- ls -l output ---\n");
                system("ls -l /usr/tmp/largefile_1104 /usr/tmp/largefile_copy_1104 /usr/tmp/received_file_1104");

        }
        return 0;
}
