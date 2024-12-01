#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

// Function declarations.
int scan_port(int port);

int main()
{

	int scan_socket;
	if ( (scan_socket = scan_port(22) ) == -1)
	{
		puts("No contact with port 22\n");
	}
	if ( (scan_socket = scan_port(21) ) == -1)
	{
		puts("No contact with port 21\n");
	}
	return 0;
}

int scan_port(int port)
{
	int new_socket;
	errno = 0; // Errno must be locked for multithreading 
	if ( (new_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		close(new_socket);
		return -1;
	}

	struct sockaddr_in bind_address;
	memset(&bind_address, 0, sizeof(bind_address));
	bind_address.sin_family = AF_INET;
	bind_address.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // Change this to scan other devices
	bind_address.sin_port = htons(port);
	
	errno = 0;
	if (connect(new_socket, (struct sockaddr *) &bind_address, sizeof(bind_address)) < 0)
	{
		sprintf(stderr, "Could not connect to port %d", port);
		close(new_socket);
		return -1;
	}
	char send_string[13] = "Hello World!";
	char read_string[101];
	errno = 0;
	if (write(new_socket, &send_string, 20) == -1)
	{
		perror("Write: ");
		return -1;
	}
	errno = 0;
	if (read(new_socket, read_string,100) == -1)
	{
		perror("Read: ");
		return -1;
	}
	read_string[100] == '\0';
	puts(read_string);

	close(new_socket);
	return 1;
}

