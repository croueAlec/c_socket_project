#ifndef PROJECT_H
#define PROJECT_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <poll.h>

#define MAX_CLIENT 2
#define ADDRESS 2130706433 // 127.0.0.1
#define PORT 8080

#define BUFFER_SIZE 512
#define MAX_CMD 8

#define UNDEFINED_FD -2

enum PACKET_TYPE
{
	NAMECARD = 1,
	VAL = 2,
};

typedef struct network

{
	struct sockaddr_in server_address;
	int				   network_fd;
} t_net;

typedef struct namecard
{
	char name[BUFFER_SIZE];
} t_namecard;

typedef struct packet
{
	uint8_t type;

	union
	{
		t_namecard namecard;
		int		   val;
	} data;

} t_packet;

void fatal_error(const char *error_message);
void read_packet(t_packet *packet);
void common(void);

#endif
