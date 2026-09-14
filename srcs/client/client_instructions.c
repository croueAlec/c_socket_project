#include "project.h"

int wait_command(t_instruction *instruction)
{
	printf("This is the 'Wait' client side command\n");

	(void)instruction;
	return WAIT;
}

const t_inst_cmd commands[INSTRUCTION_COUNT] = {
	{WAIT, wait_command},
	{NONE, NULL		   },
};

void send_client_instruction(t_net *network, t_inst_type type, const char *message, t_game_state *state)
{
	t_instruction instruction = {0};

	if (message == NULL) {
		;
	}

	instruction.type = type;

	switch (type) {
	case END:
		printf("'End game' instruction sent\n");
		break;

	case ACTION:
		printf("'Action' instruction sent\n");
		break;

	default:
		break;
	}

	send_instruction(network, &instruction);
	(void)state;

	return;
}
