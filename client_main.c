#include "project.h"

const char commands[8][10] = {
	"q",
	"quit",
	"s",
	"send",
	"",
};

typedef struct network
{
	struct sockaddr_in server_address;
	int				   network_socket;
} t_net;

void fatal_error(const char *error_message)
{
	perror(error_message);
	exit(errno);
}

void closing_client(t_net *network)
{
	send(network->network_socket, "", sizeof(""), 0);
	close(network->network_socket);
	network->network_socket = -2;
	return;
}

void lowercase_string(char *str)
{
	for (size_t i = 0; str && str[i]; i++) {
		str[i] = tolower(str[i]);
	}
}

char *getstr(char *line)
{
	size_t size = 0;
	getline(&line, &size, stdin);
	return line;
}

void get_user_message(t_net *network)
{
	char *user_message = NULL;
	printf("Input user message : ");

	user_message = getstr(user_message);
	if (user_message == NULL)
		fatal_error("Malloc error : get_user_message()");

	char *nl = strchr(user_message, '\n');
	*nl = '\0';

	printf("bytes sent : %ld\n", send(network->network_socket, user_message, strlen(user_message), 0));
	free(user_message);
	printf("message sent\n");
}

bool handle_cmd(t_net *network, char *buffer)
{
	lowercase_string(buffer);
	// should trim the strings

	int cmd = -1;
	for (size_t i = 0; commands[i][0]; i++) {
		if (strlen(buffer) == 0) {
			cmd = 0;
			break;
		}

		if (strncmp(buffer, commands[i], strlen(commands[i])) == 0) {
			cmd = i;
			break;
		}
	}
	switch (cmd) {
	case -1:
		printf("invalid command : %s\n", buffer);
		free(buffer);
		return true;

	case 0:
	case 1:
		printf("Quitting...\n");
		free(buffer);
		return false;
		break;

	case 2:
	case 3:
		get_user_message(network);
		free(buffer);
		return true;
	}

	return true;
}

void loop(t_net *network)
{
	int	  alive = true;
	char *buffer = NULL;
	while (alive) {
		printf("Commands : q(uit), s(end)\n> ");
		buffer = getstr(buffer);
		if (handle_cmd(network, buffer) == false)
			break;
	}
}

int establish_connection(t_net *network)
{
	int				   status, client_fd;
	struct sockaddr_in serv_addr;
	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
		printf(
			"\nInvalid address/ Address not supported \n");
		return -1;
	}

	if ((status = connect(client_fd, (struct sockaddr *)&serv_addr,
						  sizeof(serv_addr))) < 0) {
		printf("\nConnection Failed \n");
		return -1;
	}

	memcpy(&network->server_address, &serv_addr, sizeof(serv_addr));
	network->network_socket = client_fd;

	return 0;
}

int main(int argc, const char *argv[])
{
	printf("Starting client\n");
	t_net network = {0};

	if (establish_connection(&network) == -1)
		return 1;

	loop(&network);

	closing_client(&network);

	(void)argc;
	(void)argv;
	return 0;
}
