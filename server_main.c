#include "project.h"

// int client_list[MAX_CLIENT];

void fatal_error(const char *error_message)
{
	perror(error_message);
	exit(errno);
}

int main(int argc, const char *argv[])
{
	int				   server_fd, new_socket;
	struct sockaddr_in address;
	int				   opt = 1;
	socklen_t		   addrlen = sizeof(address);
	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(PORT);

	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr *)&address,
			 sizeof(address)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
							 &addrlen)) < 0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}

	char read_buffer[BUFFER_SIZE] = {0};
	while (recv(new_socket, &read_buffer, BUFFER_SIZE - 1, 0) > 0) {
		printf("%s\n", read_buffer);
		bzero(read_buffer, BUFFER_SIZE);
	}

	// closing the connected socket
	close(new_socket);

	// closing the listening socket
	close(server_fd);

	(void)argc;
	(void)argv;
	return 0;
}
