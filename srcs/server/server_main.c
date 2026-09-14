#include "project.h"
#include "server.h"

int count = 0;

struct pollfd clients[MAX_CLIENT] = {0};

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
				if (receive_instruction(network, &clients[i]) == CLOSE)
					close_client(clients, i, nfds);
				break;
			}
		}

	} while (true);
}

int main(int argc, const char *argv[])
{
	t_net network = {0};

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
