//====Start-Version====
#define PROG_NAME       "LiDE"
#define PROG_VERSION    "0.0.3.4"
#define PROG_DATE       "12 June 2015"
//====End-Version====

#define CHUNK_SIZE 1024
char buffer[CHUNK_SIZE];

//====Start-Local_IP_Address====
char *ip_interface;
//====End-Local_IP_Address====

void print_help(void);
char *read_file(FILE *filename_fd, int from);

char *LiDE_ver;
