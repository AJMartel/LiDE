#include <stdio.h>        // C library to perform Input/Output operations
#include <unistd.h>       // C standard symbolic constants and types
#include <string.h>       // C Strings
#include <stdlib.h>       // C Standard General Utilities Library
#include <arpa/inet.h>    // C definitions for internet operations
#include <sys/socket.h>   // C Internet Protocol family
#include "LiDE.h"         //
//====Start-Local_IP_Address====
#include <sys/types.h>    // C data types                --|
#include <sys/ioctl.h>    // C generic I/O operation       |== Support IP Address Function
#include <netinet/in.h>   // C Internet Protocol family    |
#include <net/if.h>       // C Internet Protocol family  --|
//====End-Local_IP_Address====

/*
 * Send the file to the currently connected client, return the number of bytes sent or 0 on error. 
 * The moment there is an error with a send call, it just dies and returns -1,  
 * this could obviously be improved to make it a little "smarter".
 */

int send_file(FILE *fd, int connfd)
{
	int filesize,
	    number_complete_chunks,
	    final_chunk_size,
	    total_bytes_sent = 0,
	    bytes_sent = 0,
	    i;

	fseek(fd, 0L, SEEK_END);
	filesize = ftell(fd);
	rewind(fd);

	number_complete_chunks = filesize / CHUNK_SIZE;
	final_chunk_size = filesize % CHUNK_SIZE;

	char *chunk;

	if((chunk = (char *) malloc(CHUNK_SIZE)) == NULL)
	{
		perror("Malloc");
		exit(1);
	}


	for (i = 0; i < number_complete_chunks; i++)
	{
		fread(chunk, CHUNK_SIZE, 1, fd);

		if ((bytes_sent = send(connfd, chunk, CHUNK_SIZE, 0)) < 0)
			return -1;

		total_bytes_sent += bytes_sent;
		printf("\x1B[31m%d\x1B[32m of %d\040bytes\r\e[?25l\033[0m", total_bytes_sent, filesize); // the "\r" refreshes current line and "\e[?25l" removes the cursor
	}

	// send the last incomplete chunk at the end of the file.
	fread(chunk, final_chunk_size, 1, fd);

	if ((bytes_sent = send(connfd, chunk, final_chunk_size, 0)) < 0)
		return -1;

	total_bytes_sent += bytes_sent;
		printf("\x1B[32m%d of %d\040bytes\033[0m\n", total_bytes_sent, filesize);

	free(chunk);
	return total_bytes_sent;
}

/*
 * Wait for a port connection asking for our file.
 */

int listen_for_inbound_requests(int port, FILE *filename_fd)
{
	int listenfd, bytes_sent;
	int connfd;
	struct sockaddr_in serv, cli;	
	socklen_t len;

	//====Start-Local_IP_Address====
	int ipfd;
	struct ifreq ifr;
	ipfd = socket(AF_INET, SOCK_DGRAM, 0);
	ifr.ifr_addr.sa_family = AF_INET; /* I want to get an IPv4 IP address */
	strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1); /* I want IP address attached to "eth0" */
	ioctl(ipfd, SIOCGIFADDR, &ifr);
	close(ipfd);
	//====End-Local_IP_Address====

	char *ip_buff; // Somewhere to put an IP address.
	
	if((ip_buff = (char *) malloc(INET_ADDRSTRLEN)) == NULL)
	{
		perror("Malloc");
		exit(1);
	}

	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("Error creating socket");
	}

	int tr = 1;
	if (setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&tr,sizeof(int)) == -1) { 
		perror("setsockopt");
		exit(1);
	}

	bzero(&serv, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = htonl(INADDR_ANY);
	serv.sin_port = port;

	if ((bind(listenfd, (struct sockaddr *) &serv, sizeof(serv))) == -1){
		perror("Bind error");
		exit(1);
	} 
	

	if ((listen(listenfd, 5)) == -1){ // We don't require a huge backlog (arg 2). 
		perror("Send error");
		exit(1);
	}

	//====Start-Local_IP_Address====
	printf("\x1B[33mTo capture the sent file, enter the following into the remote system: \n\x1B[31mnetcat %s %d > received_file && md5sum received_file\033[0m\n", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr), ntohs(serv.sin_port)); /* display result */
	//====End-Local_IP_Address====

	printf("\x1B[32mSending on port %d...\033[0m\n", ntohs(serv.sin_port));
	

	for (;;) {
		len = sizeof(cli);
		connfd = accept(listenfd, (struct sockaddr *) &cli, &len);

		inet_ntop(AF_INET, &(cli.sin_addr), ip_buff, INET_ADDRSTRLEN);
		printf("\x1B[32mConnection from %s, port %d\033[0m\n", ip_buff, ntohs(cli.sin_port));

		if ((bytes_sent = send_file(filename_fd, connfd)) < 0)
			printf("\x1B[31mError transferring file.\033[0m\n\e[?25h");
		else
			printf("\x1B[32mFinished transfer to %s\n\%d bytes sent.\033[0m\n\e[?25h", ip_buff, bytes_sent);exit(0);
	}

	free(ip_buff);

	return 0;
}
