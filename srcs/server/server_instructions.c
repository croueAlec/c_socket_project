#include "project.h"

int action_request(t_instruction *instruction)
{
	printf("This is the 'Action' server side command\n");

	(void)instruction;
	return ACTION;
}

int login_request(t_instruction *instruction)
{
	printf("This is the 'Login' server side command\n");
	printf("Message : %s\n", instruction->message);
	printf("Username : %s\n", instruction->state.pl_1.name);

	(void)instruction;
	return LOGIN;
}

int close_request(t_instruction *instruction)
{
	printf("This is the 'Close' server side command\n");

	(void)instruction;
	return CLOSE;
}

const t_inst_req requests[INSTRUCTION_COUNT] = {
	{CLOSE,	 close_request },
	{LOGIN,	 login_request },
	{ACTION, action_request},
	{NONE,	 NULL		   },
};

void send_server_instruction(t_net *network, t_inst_type type, const char *message, t_game_state *state)
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

	case WAIT:
		printf("'Wait' instruction sent\n");
		break;

	case UNWAIT:
		printf("'Unwait' instruction sent\n");
		break;

	default:
		break;
	}

	printf("bytes sent : %d\n", send_instruction(network, &instruction));
	(void)state;

	return;
}
