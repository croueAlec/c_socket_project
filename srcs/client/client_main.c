#include "project.h"

const char commands[8][10] = {
	"q",
	"quit",
	"s",
	"send",
	"v",
	"val",
	"",
};

void close_client(t_net *network)
{
	send(network->network_fd, "", sizeof(""), 0);
	close(network->network_fd);
	network->network_fd = UNDEFINED_FD;
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

void get_user_message(t_net *network, bool is_value)
{
	char *user_message = NULL;
	printf("Input user message : ");

	user_message = getstr(user_message);
	if (user_message == NULL) {
		fatal_error("Malloc error : get_user_message()");
	} else if (strlen(user_message) == 0) {
		send(network->network_fd, "", 0, 0);
		printf("input cancelled : 0 bytes sent\n");
		return;
	}

	char *nl = strchr(user_message, '\n');
	*nl = '\0';

	t_packet packet = {0};
	if (is_value == false) {
		printf("sending namecard\n");
		packet.type = NAMECARD;
		memcpy(packet.data.namecard.name, user_message, strlen(user_message));
	} else {
		printf("sending val\n");
		packet.type = VAL;
		packet.data.val = atoi(user_message);
	}

	printf("bytes sent : %ld\n", send(network->network_fd, &packet, sizeof(packet), 0));
	free(user_message);
	printf("message sent\n");
}

bool handle_cmd(t_net *network, char *cmd, int cmd_fd)
{
	if (network == NULL)
		lowercase_string(cmd);
	// should trim the strings

	int cmd_id = -1;
	for (size_t i = 0; commands[i][0]; i++) {
		if (strlen(cmd) == 0) {
			cmd = 0;
			break;
		}

		if (strncmp(cmd, commands[i], strlen(commands[i])) == 0) {
			cmd = i;
			break;
		}
	}
	switch (cmd_id) {
	case -1:
		printf("invalid command : %s\n", cmd);
		free(cmd);
		return true;

	case 0:
	case 1:
		printf("Quitting...\n");
		free(cmd);
		return false;
		break;

	case 2:
	case 3:
		get_user_message(network, false);
		free(cmd);
		return true;

	case 4:
	case 5:
		get_user_message(network, true);
		free(cmd);
		return true;
	}

	(void)cmd_fd;
	return true;
}

void loop(t_net *network)
{
	int	  alive = true;
	char *buffer = NULL;
	int	  cmd_fd = UNDEFINED_FD;

	while (alive) {
		if (cmd_fd == UNDEFINED_FD) {
			cmd_fd = dup(STDIN_FILENO);
			if (cmd_fd < 0)
				fatal_error("Fatal error : dup()");
		}

		printf("Commands : q(uit), s(end), v(al)\n> ");
		buffer = getstr(buffer);
		if (handle_cmd(network, buffer, cmd_fd) == false)
			break;
	}
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

int main(int argc, const char *argv[])
{
	common();
	printf("Starting client\n");
	t_net network = {0};

	if (establish_connection(&network) == -1)
		return 1;

	loop(&network);

	close_client(&network);

	(void)argc;
	(void)argv;
	return 0;
}
