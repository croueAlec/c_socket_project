#include "project.h"
#include "client.h"

int request_wait(t_instruction *instruction)
{
	printf("This is the 'Wait' client side command\n");

	(void)instruction;
	return WAIT;
}

const t_inst_req requests[INSTRUCTION_COUNT] = {
	{WAIT, request_wait},
	{NONE, NULL		   },
};

void order_quit(t_net *network)
{
	send_client_order(network, CLOSE, "Close", &state);
	printf("Quitting...\n");
}

void order_action(t_net *network)
{
	send_client_order(network, ACTION, "Action", &state);
}

void print_pending_order(t_instruction *instruction)
{
	printf("Type : %d\n", instruction->type);
	printf("Message : %s\n", instruction->message);
	printf("Player 1 name : %s\n", instruction->state.pl_1.name);
}

void order_login(t_net *network)
{
	state.exists = true;
	// player *me = (is_player_1) ? &state.pl_1 : &state.pl_2;
	player *me = &state.pl_1;

	get_user_string(network, me->name, MAX_NAME_LENGTH);

	send_client_order(network, LOGIN, "Login", &state);
}

void send_client_order(t_net *network, t_inst_type type, const char *message, t_game_state *state)
{
	t_instruction instruction = {0};
	memcpy(&instruction.message, "This is a message", strlen("This is a message"));

	if (message == NULL) {
		printf("'%s' order sent", message);
	}

	instruction.type = type | CLIENT_SIDE;
	memcpy(&instruction.state, state, sizeof(t_game_state));

	printf("bytes sent : %d to server fd %d\n", send_instruction(network, &instruction), network->network_fd);

	return;
}
