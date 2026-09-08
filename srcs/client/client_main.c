#include "project.h"
#include "client.h"

const char commands[8][10] = {
	"q",
	"quit",
	"s",
	"send",
	"v",
	"val",
	"",
};

char *getstr(char *line)
{
	size_t size = 0;

	int cmd_fd = dup(STDIN_FILENO);
	if (cmd_fd < 0)
		fatal_error("Fatal error : dup()");

	FILE *dup_stdin = fdopen(cmd_fd, "r");
	if (dup_stdin == NULL)
		fatal_error("Fatal error : fdopen()");

	if (getline(&line, &size, dup_stdin) == -1)
		line = calloc(1, sizeof(char));

	fclose(dup_stdin);
	cmd_fd = UNDEFINED_FD;
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

bool handle_cmd(t_net *network, char *cmd)
{
	clean_string(cmd);

	int cmd_id = -1;
	for (size_t i = 0; commands[i][0]; i++) {
		if (strlen(cmd) == 0) {
			cmd_id = 0;
			break;
		}

		if (strncmp(cmd, commands[i], strlen(commands[i])) == 0) {
			cmd_id = i;
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

	return true;
}

void loop(t_net *network)
{
	int	  alive = true;
	char *buffer = NULL;

	while (alive) {
		printf("Commands : q(uit), s(end), v(al)\n> ");
		buffer = getstr(buffer);
		if (handle_cmd(network, buffer) == false)
			break;
	}
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
