#include "project.h"

int send_instruction(t_net *network, t_instruction *instruction)
{
	int bytes_sent = send(network->network_fd, instruction, sizeof(t_instruction), 0);

	if (bytes_sent < 0) {
		fatal_error("Fatal error : send()");
	}
	(void)instruction;

	return bytes_sent;
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
	while (requests[i].command_type != instruction->type) {
		printf("trying command %d for type %d\n", requests[i].command_type, instruction->type);
		if (requests[i].command_type == NONE)
			return -1; // error
		i++;
	}

	return (requests[i].handler(instruction));
}

int receive_instruction(t_net *network, const struct pollfd *client)
{
	t_instruction instruction = {0};
	char		  buffer[sizeof(t_instruction)] = {0};
	int			  bytes_received = 0;

	do {
		bytes_received = recv(client->fd, &buffer[bytes_received], sizeof(t_instruction), 0);
		if (bytes_received < 0) {
			fatal_error("Fatal error : receive_instruction()");
		}
	} while (errno != EAGAIN);

	memcpy(&instruction, buffer, sizeof(instruction));

	printf("instruction size : %ld\tbytes received :%d\n", sizeof(t_instruction), bytes_received);

	(void)network;
	return (handle_instruction(&instruction));
}
