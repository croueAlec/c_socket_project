#include "project.h"
#include "client.h"

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

void quit(t_net *network)
{
	send_client_instruction(network, CLOSE, "Close", &state);
	printf("Quitting...\n");
}

void action(t_net *network)
{
	send_client_instruction(network, ACTION, "Action", &state);
}

void print_pending_instruction(t_instruction *instruction)
{
	printf("Type : %d\n", instruction->type);
	printf("Message : %s\n", instruction->message);
	printf("Player 1 name : %s\n", instruction->state.pl_1.name);
}

void login(t_net *network)
{
	state.exists = true;
	// player *me = (is_player_1) ? &state.pl_1 : &state.pl_2;
	player *me = &state.pl_1;

	get_user_string(network, me->name, MAX_NAME_LENGTH);

	send_client_instruction(network, LOGIN, "Login", &state);
}

void send_client_instruction(t_net *network, t_inst_type type, const char *message, t_game_state *state)
{
	t_instruction instruction = {0};
	memcpy(&instruction.message, "This is a message", strlen("This is a message"));

	if (message == NULL) {
		;
	}

	instruction.type = type;
	memcpy(&instruction.state, state, sizeof(t_game_state));

	switch (type) {
	case END:
		printf("'End game' instruction sent\n");
		break;

	case ACTION:
		printf("'Action' instruction sent\n");
		break;

	case LOGIN:
		printf("'Login' instruction sent\n");

	default:
		break;
	}

	printf("bytes sent : %d to server fd %d\n", send_instruction(network, &instruction), network->network_fd);
	(void)state;

	return;
}
