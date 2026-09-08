#include "project.h"

// int client_list[MAX_CLIENT];

void init_server(t_net *network)
{

	int				   server_fd = UNDEFINED_FD;
	struct sockaddr_in address;
	int				   opt = 1;

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(PORT);

	if (bind(server_fd, (struct sockaddr *)&address,
			 sizeof(address)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	memcpy(&network->server_address, &address, sizeof(address));
	network->network_fd = server_fd;
}

void handle_clients(t_net *network)
{
	int		  new_socket;
	socklen_t addrlen = sizeof(network->server_address);

	if ((new_socket = accept(network->network_fd, (struct sockaddr *)&network->server_address,
							 &addrlen)) < 0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}

	char read_buffer[BUFFER_SIZE] = {0};
	while (recv(new_socket, &read_buffer, BUFFER_SIZE - 1, 0) > 0) {
		printf("%s\n", read_buffer);
		bzero(read_buffer, BUFFER_SIZE);
	}

	close(new_socket);
	new_socket = UNDEFINED_FD;
}

int main(int argc, const char *argv[])
{
	t_net network = {0};
	common();

	init_server(&network);

	handle_clients(&network);

	close(network.network_fd);
	network.network_fd = UNDEFINED_FD;

	(void)argc;
	(void)argv;
	return 0;
}
