#include "project.h"
#include "server.h"

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

int accept_clients(t_net *network, int nfds)
{
	int new_client_fd = UNDEFINED_FD;
	do {
		new_client_fd = accept(network->network_fd, NULL, NULL); // check accept() parameters
		if (new_client_fd < 0) {
			if (errno != EWOULDBLOCK) {
				fatal_error("Fatal error : accept()");
			}
			break;
		}

		printf("new client added : %d\n", new_client_fd);
		clients[nfds].fd = new_client_fd;
		clients[nfds].events = POLLIN;
		nfds++;
	} while (new_client_fd != -1);

	return nfds;
}

/**
 * @brief Closes a client's fd and moves the last client pollfd in it's place, erasing it's previous spot
 *
 * @param clients the Clients array
 * @param client_index the closing Client's index
 * @param nfds the number of open sockets (including the server socket)
 * @return int (nfds - 1)
 */
int close_client(struct pollfd *clients, int client_index, int nfds)
{
	nfds--;

	printf("closing client %d\n", clients[client_index].fd);
	close(clients[client_index].fd);

	memcpy(&clients[client_index], &clients[nfds], sizeof(struct pollfd));

	bzero(&clients[nfds], sizeof(struct pollfd));
	clients[nfds].fd = UNDEFINED_FD;

	return nfds;
}
