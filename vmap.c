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
	char send_string[13] = "Hello World!";
	char read_string[101];
	puts(send_string);

	int scan_socket = scan_port(9999);
	printf("Socket file descriptor: %d\n",scan_socket);
	write(scan_socket, &send_string, 20);
	read(scan_socket, read_string,100);
	read_string[100] == '\0';
	puts(read_string);

	close(scan_socket);
	return 0;
}

int scan_port(int port)
{
	int new_socket;
	errno = 0; // Errno must be locked for multithreading 
	if ( (new_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror(strerror(errno));
		close(new_socket);
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in bind_address;
	memset(&bind_address, 0, sizeof(bind_address));
	bind_address.sin_family = AF_INET;
	bind_address.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // Change this to scan other devices
	bind_address.sin_port = htons(port);
	
	if (connect(new_socket, (struct sockaddr *) &bind_address, sizeof(bind_address)) < 0)
	{
		perror("Connect failed: ");
		perror(strerror(errno));
		close(new_socket);
		exit(EXIT_FAILURE);
	}
	return new_socket;
}

