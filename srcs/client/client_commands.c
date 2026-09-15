#include "project.h"
#include "client.h"

const t_command_name command_names[] = {
	{"q", "quit"  },
	{"a", "action"},
	{"l", "login" },
	{"",  ""	  },
};

static bool compare_commands(const char *cmd, size_t index)
{
	size_t len = strlen(cmd);

	if (strncmp(cmd, command_names[index].letter, len) == 0)
		return true;
	else if (strncmp(cmd, command_names[index].string, len) == 0)
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

	int cmd_id = -1;
	for (size_t i = 0; strlen(command_names[i].letter); i++) {
		if (strlen(cmd) == 0) {
			cmd_id = 0;
			break;
		}

		if (compare_commands(cmd, i)) {
			cmd_id = i;
			break;
		}
	}

	switch (cmd_id) {
	case -1:
		printf("invalid command : %s\n", cmd);
		return true;

	case 0: // quit
		quit(network);
		return false;
		break;

	case 1: // action
		action(network);
		return true;

	case 2: // login
		login(network);
		return true;
	}

	return true;
}
