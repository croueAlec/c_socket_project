#ifndef CLIENT_H
#define CLIENT_H

extern const char command_names[8][10];

/* Utils */
void clean_string(char *str);

/* Sockets */
void close_client(t_net *network);
int	 establish_connection(t_net *network);

/* Commands */
char *getstr(char *line);
bool  handle_cmd(t_net *network, char *cmd);

#endif
