#include "project.h"
#include "client.h"

const t_command_name command_names[] = {
	{"q", "quit",	CLOSE },
	{"a", "action", ACTION},
	{"l", "login",	LOGIN },
	{"",  "",		NONE  },
};

static bool compare_commands(const char *cmd, size_t index)
{
	if (strcmp(cmd, command_names[index].letter) == 0)
		return true;
	else if (strcmp(cmd, command_names[index].string) == 0)
		return true;
	else
		return false;
}

static char *getstr(char *line)
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

void get_user_string(t_net *network, char *message, size_t size)
{
	if (message == NULL)
		return;

	char *user_message = NULL;
	printf("Input user message : ");

	user_message = getstr(user_message);
	if (user_message == NULL) {
		fatal_error("Malloc error : get_user_string()");
	} else if (strlen(user_message) == 0) {
		printf("input cancelled : 0 bytes sent\n");
		return;
	}

	char *nl = strchr(user_message, '\n');
	*nl = '\0';

	size = (size < strlen(user_message)) ? size : strlen(user_message);

	memcpy(message, user_message, size);
	free(user_message);
	(void)network;
}

bool handle_cmd(t_net *network, char *cmd)
{
	clean_string(cmd);

	t_inst_type cmd_id = NONE;
	for (size_t i = 0; command_names[i].type != NONE; i++) {
		if (strlen(cmd) == 0) {
			cmd_id = CLOSE;
			break;
		}

		if (compare_commands(cmd, i)) {
			cmd_id = command_names[i].type;
			break;
		}
	}

	switch (cmd_id) {
	default:
	case NONE:
		printf("invalid command : %s\n", cmd);
		return true;

	case CLOSE: // quit (close)
		quit(network);
		return false;

	case ACTION: // action
		action(network);
		return true;

	case LOGIN: // login
		login(network);
		return true;
	}

	return true;
}
