#ifndef SERVER_H
#define SERVER_H

#include "project.h"

extern int			 count;
extern struct pollfd clients[MAX_CLIENT];

/* Sockets */
void init_server(t_net *network);
int	 accept_clients(t_net *network, int nfds);
void close_client(struct pollfd *clients, int client_index, int nfds);

#endif
