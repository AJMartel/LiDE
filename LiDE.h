#define CHUNK_SIZE 1024
char buffer[CHUNK_SIZE];

void print_help(void);
char *read_file(FILE *filename_fd, int from);
