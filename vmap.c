#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


// Function declarations.
int scan_port(void* port_pointer);

int main()
{
	int min_port;
	int max_port;

	min_port = 1;
	max_port = 100;

	pthread_t thread_ids[max_port - min_port];
	int* ports[max_port - min_port];
	for (int i = 0; i <= max_port-min_port; i++)
	{
		ports[i] = malloc(sizeof(int));
		*ports[i] = i + min_port;
		if (pthread_create(&thread_ids[i], NULL, scan_port, (void *) ports[i]) != 0)
		{
			printf("Failed to create thread %d", i+min_port);
		}
	}


	for (int i = 0; i <= max_port-min_port; i++)
	{
		errno = 0;
		if (pthread_join(thread_ids[i],NULL) != 0 && errno != 0)
		{
			perror("join: ");
		}
		free((void *) ports[i]);
		ports[i] = NULL;
	}
	return 0;
}

int scan_port(void* port_pointer)
{
	int port = *(int *) port_pointer;
	int new_socket;
	errno = 0;
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
		//printf("Could not connect to port %d\n", port);
		close(new_socket);
		return -1;
	}
	char send_string[13] = "Hello World!";
	char read_string[101];

	errno = 0;
	if (write(new_socket, &send_string, 20) == -1)
	{
		perror("Write: ");
		close(new_socket);
		return -1;
	}
	errno = 0;
	if (read(new_socket, read_string,100) == -1)
	{
		perror("Read: ");
		close(new_socket);
		return -1;
	}
	read_string[100] = '\0';
	printf("Port %d: ", port);
	puts(read_string);
	close(new_socket);
	return 1;
}

