#include "project.h"

int count = 0;

struct pollfd clients[MAX_CLIENT] = {0};

void init_server(t_net *network)
{

	int				   server_fd = UNDEFINED_FD;
	struct sockaddr_in address;
	int				   opt = 1;

	if ((server_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) < 0) {
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

	if (listen(server_fd, MAX_CLIENT) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	clients[0].fd = server_fd;
	clients[0].events = POLLIN;

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

	char read_buffer[BUFFER_SIZE + 24] = {0};
	while (recv(new_socket, &read_buffer, BUFFER_SIZE - 1, 0) > 0) {
		t_packet packet = {0};
		memcpy(&packet, read_buffer, (sizeof(packet)));
		read_packet(&packet);
		// printf("%s\n", read_buffer);
		bzero(read_buffer, BUFFER_SIZE);
	}

	close(new_socket);
	new_socket = UNDEFINED_FD;
}

int accept_clients(t_net *network, int nfds)
{
	printf("client search : begin\n");
	int new_client_fd = UNDEFINED_FD;
	do {
		printf("client search : new\n");
		new_client_fd = accept(network->network_fd, NULL, NULL); // check accept() parameters
		if (new_client_fd < 0) {
			if (errno != EWOULDBLOCK) {
				fatal_error("Fatal error : accept()");
			}
			break;
		}

		printf("client search : new added : %d\n", new_client_fd);
		clients[nfds].fd = new_client_fd;
		clients[nfds].events = POLLIN;
		nfds++;
	} while (new_client_fd != -1);

	printf("client search : end\n");

	return nfds;
}

void tmp_read(t_net *network, const struct pollfd *client)
{
	char buffer[512 * 2];
	int	 bytes_received = recv(client->fd, buffer, sizeof(buffer), 0);
	if (bytes_received < 0) {
		fatal_error("Fatal error : recv() tmp_error()");
	} else if (bytes_received == 0) {
		printf("connection closed\n");
		return;
	}

	printf("bytes received : %d\n", bytes_received);
	printf("[%s]\n", buffer);
	(void)network;
}

void loop(t_net *network)
{
	int current_size = 0;
	int nfds = 1;
	do {
		current_size = nfds;
		int rc = poll(clients, nfds, (30 * 60 * 3000)); // timeout == 3 minutes
		if (rc < 0) {
			fatal_error("Fatal error : poll()");
		} else if (rc == 0) {
			fatal_error("Fatal error : poll() timeout");
		}

		for (int i = 0; i < current_size; i++) {
			if (clients[i].revents == 0)
				continue;

			if (clients[i].revents != POLLIN) {
				printf("  Error! revents = %d\n", clients[i].revents);
				fatal_error("Fatal error : revents");
			}

			if (clients[i].fd == network->network_fd) { // incoming connection(s)
				printf(" Reading server fd\n");
				nfds = accept_clients(network, nfds);
			} else {
				printf("  Descriptor %d is readable\n", clients[i].fd);
				tmp_read(network, &clients[i]);
			}
		}

	} while (true);
}

int main(int argc, const char *argv[])
{
	t_net network = {0};

	common();
	printf("Booting server\n");

	init_server(&network);

	// handle_clients(&network);
	loop(&network);

	close(network.network_fd);
	network.network_fd = UNDEFINED_FD;

	(void)argc;
	(void)argv;
	return 0;
}
