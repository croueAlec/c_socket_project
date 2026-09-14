#include "project.h"
#include "client.h"

void close_client(t_net *network)
{
	send(network->network_fd, "", sizeof(""), 0);
	close(network->network_fd);
	network->network_fd = UNDEFINED_FD;
	return;
}

int establish_connection(t_net *network)
{
	int				   status, client_fd;
	struct sockaddr_in address;
	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}

	address.sin_family = AF_INET;
	address.sin_port = htons(PORT);

	if (inet_pton(AF_INET, "127.0.0.1", &address.sin_addr) <= 0) {
		printf(
			"\nInvalid address/ Address not supported \n");
		return -1;
	}

	if ((status = connect(client_fd, (struct sockaddr *)&address,
						  sizeof(address))) < 0) {
		printf("\nConnection Failed \n");
		return -1;
	}

	memcpy(&network->server_address, &address, sizeof(address));
	network->network_fd = client_fd;

	return 0;
}
