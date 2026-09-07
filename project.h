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

#define MAX_CLIENT 2
#define ADDRESS 2130706433 // 127.0.0.1
#define PORT 8080

#define BUFFER_SIZE 512
#define MAX_CMD 8

#endif
