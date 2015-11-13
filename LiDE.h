#define CHUNK_SIZE 1024
char buffer[CHUNK_SIZE];
//====Start-Local_IP_Address====
char *ip_interface;
//====End-Local_IP_Address====
void print_help(void);
char *read_file(FILE *filename_fd, int from);
