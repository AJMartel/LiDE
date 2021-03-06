#include <stdio.h>        // C library to perform Input/Output operations
#include <unistd.h>       // C standard symbolic constants and types
#include <string.h>       // C Strings
#include <stdlib.h>       // C Standard General Utilities Library
#include <arpa/inet.h>    // C definitions for internet operations
#include <sys/socket.h>   // C Internet Protocol family
#include <sys/types.h>    // C data types
#include "LiDE.h"         //    
//====Start-MD5-Support====
#include <sys/stat.h>     // C data returned by the stat() function    --|
#include <sys/mman.h>     // C memory management declarations            |== Support MD5 Function
#include <fcntl.h>        // C file control options                      |
#include <openssl/md5.h>  // C support MD5 functions                   --|

unsigned char result[MD5_DIGEST_LENGTH];

// Print the MD5 sum as hex-digits.
void print_md5_sum(unsigned char* md) {
    int i;
    for(i=0; i <MD5_DIGEST_LENGTH; i++) {
            printf("%02x",md[i]);
    }
}

// Get the size of the file by its file descriptor
unsigned long get_size_by_fd(int fd) {
    struct stat statbuf;
    if(fstat(fd, &statbuf) < 0) exit(-1);
    return statbuf.st_size;
}
//====End-MD5-Support====

void print_help(void){
	printf("\x1B[33m%s (\033[1mLi\033[22mnux \033[1mD\033[22misk \033[1mE\033[22mxtractor) - v%s\033[0m \n\n", PROG_NAME, PROG_VERSION);
	printf("This is a linux TCP enabled cat/dd utility. \n\
It enables you to copy a file, full disk or partition across network.\n\n\
Usage; \n\
$ \x1B[32m./lide [-h]\033[0m                        Prints this help.\n\n\
To host a file:\n\
$ \x1B[32m./lide <filename>\033[0m                  Will use default port 31337\n\
    or\n\
$ \x1B[32m./lide -p <port> <filename>\033[0m        To specify a port to send on.\n\
$ \x1B[32m./lide -i <eth0|wlan0> <filename>\033[0m  To specify an interface to send on.\n\
$ \x1B[32m./lide -i <eth0|wlan0> -p <port> <filename>\033[0m\n\n\
Then to retrieve it on another host:\n\
$ \x1B[32mnetcat [LiDE_Host_IP] [PORT] > retrieved_file && md5sum retrieved_file\033[0m\n\n");
	exit(0);
}

int main(int argc, char *argv[])
{
	FILE *filename_fd;
	char *filename_to_serve;
	int listen_port = htons(31337);

	//====Start-MD5-Support====
	int file_descript;
	unsigned long file_size;
	char *file_buffer;
	//====End-MD5-Support====
	//====Start-Local_IP_Address====
	ip_interface = "eth0";             // Default network interface
	//====End-Local_IP_Address====

	if (argc < 2)
		print_help();

	char c;
	while ((c = getopt(argc, argv, "hi:p:")) != -1) {
		switch(c){

			case 'h':
				print_help();
				break;
			//====Start-Local_IP_Address====
			case 'i':
				if (optarg != NULL){
					ip_interface = optarg;
					break;
				}
				else break;
			//====End-Local_IP_Address====
			case 'p':
				if ((optarg != NULL) && (optarg[0] != '\0')){

					listen_port = htons(atoi(optarg));

					if (listen_port < 1024 && getuid() > 0){
						printf("Invalid port number. Binding to a port lower than 1024 requires root priveleges.\n");
						exit(0);
					}
					else {
						listen_port = htons(atoi(optarg));
						break;
					}
				}
				else break;
			default:
				print_help();
				break;
		}
	}

	// Get the filename, which is the argument without an option.
	if ((argc - optind) == 1)
	{
		if (strlen(argv[argc-1]) > 255)
		{
			printf("Filename too long.\n");
			exit(0);
		}

		if((filename_to_serve = malloc(strlen(argv[argc-1]) + 1)) == NULL)
		{
			perror("Malloc");
			exit(1);
		}
		strncpy(filename_to_serve, argv[argc-1], strlen(argv[argc-1]));
	}

	if ((filename_fd = fopen(filename_to_serve, "r")) != NULL){
		//====Start-MD5-Functions==== 
		file_descript = open(filename_to_serve, O_RDONLY);
		file_size = get_size_by_fd(file_descript);
    
		file_buffer = mmap(0, file_size, PROT_READ, MAP_SHARED, file_descript, 0);
		MD5((unsigned char*) file_buffer, file_size, result);
		munmap(file_buffer, file_size); 

		printf("\x1B[33m%s (\033[1mLi\033[22mnux \033[1mD\033[22misk \033[1mE\033[22mxtractor) - v%s\033[0m \n\n", PROG_NAME, PROG_VERSION);
		printf("\x1B[32mMD5 Hash:  ");
		print_md5_sum(result);
		printf("\nFilename:  %s\nFile size: %lu\040bytes\033[0m\n\n", argv[argc-1], file_size);
		//====End-MD5-Functions====

		listen_for_inbound_requests(listen_port, filename_fd);		
		printf("%d\n", listen_port);
	}

	else
		perror("Error opening file");

	free(filename_to_serve);

	return 0;
}
