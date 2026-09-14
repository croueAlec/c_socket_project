#include "project.h"

int send_instruction(t_net *network, t_instruction *instruction)
{
	if (send(network->network_fd, instruction, sizeof(instruction), 0) < 0) {
		fatal_error("Fatal error : send()");
	}

	return 0;
}

/* static void set_instruction_message(t_instruction *instruction, const char *message)
{
	memcpy(instruction->message, message, strlen(message));
} */

/* static void set_game_state(t_instruction *instruction, const t_game_state *state)
{
	if (state == NULL)
		bzero(&instruction->state, sizeof(t_game_state));
	else
		memcpy(&instruction->state, state, sizeof(t_game_state));
} */

/* void print_game_state(t_game_state *state)
{
	if (strlen(state->pl_1.name)) {
		printf("Player 1 name : %s\n", state->pl_1.name);
		printf("Character name : %s", state->pl_1.character.name);
	}

	if (strlen(state->pl_2.name)) {
		printf("Player 2 name : %s\n", state->pl_2.name);
		printf("Character name : %s", state->pl_2.character.name);
	}
} */

/* bool print_instruction(t_instruction *instruction)
{
	switch (instruction->type) {
	case CLOSE:
		printf("Instruction 'CLOSE' received\n");
		return true;
	case END:
		printf("Instruction 'End' received\n");
		break;
	case WAIT:
		printf("Instruction 'Wait' received\n");
		break;
	case UNWAIT:
		printf("Instruction 'Unwait' received\n");
		break;

	default:
		break;
	}

	if (strlen(instruction->message))
		printf("message : %s\n", instruction->message);

	if (instruction->state.exists == true)
		print_game_state(&instruction->state);

	return false;
} */

/* int handle_server_instruction(t_instruction *instruction)
{
	print_instruction(instruction);

	switch (instruction->type) {
	case CLOSE:
		return CLOSE;

	default:
		break;
	}

	return NONE;
} */

int handle_instruction(t_instruction *instruction)
{
	size_t i = 0;
	while (commands[i].command != instruction->type) {
		if (commands[i].command != NONE)
			return -1; // error
		i++;
	}

	return (commands[i].handler(instruction));
}

bool receive_instruction(t_net *network, const struct pollfd *client)
{
	t_instruction instruction = {0};

	int bytes_received = recv(client->fd, &instruction, sizeof(instruction), 0);
	if (bytes_received < 0) {
		fatal_error("Fatal error : receive_instruction()");
	}

	printf("instruction size : %ld\tbytes received :%d\n", sizeof(instruction), bytes_received);

	(void)network;
	return (handle_instruction(&instruction));
	// return (handle_server_instruction(&instruction));
}
