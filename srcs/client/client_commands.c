#include "project.h"
#include "client.h"

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

void get_user_message(t_net *network, char *message)
{
	if (message == NULL)
		return;

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

	memcpy(message, user_message, strlen(user_message));
	free(user_message);
	(void)network;
}

bool handle_cmd(t_net *network, char *cmd)
{
	clean_string(cmd);

	int cmd_id = -1;
	for (size_t i = 0; command_names[i][0]; i++) {
		if (strlen(cmd) == 0) {
			cmd_id = 0;
			break;
		}

		if (strncmp(cmd, command_names[i], strlen(command_names[i])) == 0) {
			cmd_id = i;
			break;
		}
	}
	switch (cmd_id) {
	case -1:
		printf("invalid command : %s\n", cmd);
		free(cmd);
		return true;

	case 0: // quit
	case 1:
		send_client_instruction(network, CLOSE, "Close", NULL);
		printf("Quitting...\n");
		free(cmd);
		return false;
		break;

	case 2: // send
	case 3:
		get_user_message(network, NULL);
		free(cmd);
		return true;

	case 4: // val
	case 5:
		send_client_instruction(network, ACTION, "Action", NULL);
		// get_user_message(network, true);
		free(cmd);
		return true;
	}

	return true;
}
