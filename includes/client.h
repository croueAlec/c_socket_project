#ifndef CLIENT_H
#define CLIENT_H

#define COMMAND_NAME_LENGTH 32

extern bool			logged_in;
extern bool			is_player_1;
extern t_game_state state;

typedef struct command_name
{
	const char letter[COMMAND_NAME_LENGTH];
	const char string[COMMAND_NAME_LENGTH];
} t_command_name;

extern const t_command_name command_names[];

/* Utils */
void clean_string(char *str);

/* Sockets */
void close_client(t_net *network);
int	 establish_connection(t_net *network);

/* Commands */
void get_user_string(t_net *network, char *message, size_t size);
bool handle_cmd(t_net *network, char *cmd);
void quit(t_net *network);
void action(t_net *network);
void login(t_net *network);

#endif
