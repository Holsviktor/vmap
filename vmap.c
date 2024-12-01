#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>

// Function declarations.
int init_socket();

int main()
{
	int scan_socket = init_socket();
	printf("Socket file descriptor: %d\n",scan_socket);

	return 0;
}

int init_socket()
{
	int new_socket;
	errno = 0; // Errno must be locked for multithreading 
	if ( (new_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror(strerror(errno));
		close(new_socket);
		exit(EXIT_FAILURE);
	}
	return new_socket;
}
