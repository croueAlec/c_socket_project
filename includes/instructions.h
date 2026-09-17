#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "project.h"

typedef enum INSTRUCTION_TYPE
{
	NONE = 0,
	CLOSE,
	END,
	LOGIN,
	WAIT,
	UNWAIT,
	ACTION,
	INSTRUCTION_COUNT, // keep this instruction before SERVER_SIDE and CLIENT_SIDE
	SERVER_SIDE = 0b10000000,
	CLIENT_SIDE = 0b01000000,
} t_inst_type;

typedef struct instruction
{
	t_inst_type	 type;
	t_game_state state;
	char		 message[BUFFER_SIZE + 1];

} t_instruction;

typedef int (*inst_cmd_handler)(t_instruction *instruction);

typedef struct instruction_command
{
	t_inst_type		 command_type;
	inst_cmd_handler handler;

} t_inst_cmd;

extern const t_inst_cmd commands[INSTRUCTION_COUNT];

void send_client_instruction(t_net *network, t_inst_type type, const char *message, t_game_state *state);
void send_server_instruction(t_net *network, t_inst_type type, const char *message, t_game_state *state);

int send_instruction(t_net *network, t_instruction *instruction);
int handle_instruction(t_instruction *instruction);
int receive_instruction(t_net *network, const struct pollfd *client);

#endif
