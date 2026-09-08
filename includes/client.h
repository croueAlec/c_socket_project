#ifndef CLIENT_H
#define CLIENT_H

void clean_string(char *str);
void close_client(t_net *network);
int	 establish_connection(t_net *network);

#endif
